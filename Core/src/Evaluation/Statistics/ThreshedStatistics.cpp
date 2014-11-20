/*
 * ThreshedStatistics.cpp
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Statistics/ThreshedStatistics.h"
#include <iostream>

namespace BioFW {
	namespace Statistics {

		ThreshedStatistics::ThreshedStatistics(BasicStatistics statistics, ThreshedStatisticsSettings threshSettings) :
				statistics(statistics), threshSettings(threshSettings) {
			this->refresh();
		}
		ThreshedStatistics::ThreshedStatistics(BasicStatistics statistics_, IThreshFinder::Ptr threshFinder) :
				statistics(statistics_), threshSettings(threshFinder->findThresh(statistics_)){
			this->refresh();
		}

		const Extremes & ThreshedStatistics::getOverlapArea() const {
			return this->overlapArea;
		}
		double ThreshedStatistics::getTrueAcceptance() const {
			return this->trueAcceptance;
		}
		double ThreshedStatistics::getFalseAcceptance() const {
			return this->falseAcceptance;
		}
		double ThreshedStatistics::getTrueRejection() const {
			return this->trueRejection;
		}
		double ThreshedStatistics::getFalseRejection() const {
			return this->falseRejection;
		}

		double ThreshedStatistics::getFAR() const {
			return this->getFalseAcceptance() / (this->getFalseAcceptance() + this->getTrueRejection());
		}

		double ThreshedStatistics::getFRR() const {
			return this->getFalseRejection() / (this->getFalseRejection() + this->getTrueAcceptance());
		}

		ThreshedStatisticsSettings ThreshedStatistics::getThreshSettings() const {
			return this->threshSettings;
		}
		void ThreshedStatistics::setThreshSettings(const ThreshedStatisticsSettings & value) {
			this->threshSettings = value;
			this->refresh();
			ThreshChanged.notify(this, threshSettings);
		}

		double ThreshedStatistics::getThresh() const {
			return this->getThreshSettings().getThresh();
		}
		ThreshedStatisticsSettings::Direction ThreshedStatistics::getDirection() const {
			return this->getThreshSettings().getDirection();
		}

		void ThreshedStatistics::addScoreToStatistics(const ComparisonScore & ms, bool isGenuine) {
			if (!ms.isValid()) {
				return;
			}

			//std::cout << "add " << (isGenuine ? "G" : "I") << " with score " << ms.getScore() << std::endl;

			if (isGenuine) {
				if (this->getDirection() == ThreshedStatisticsSettings::AcceptBelowThresh) {
					if (ms.getScore() < this->getThresh()) {
						this->trueAcceptance++;
					} else {
						this->falseRejection++;
					}
				} else {
					if (ms.getScore() > this->getThresh()) {
						this->trueAcceptance++;
					} else {
						this->falseRejection++;
					}
				}
			} else {
				if (this->getDirection() == ThreshedStatisticsSettings::AcceptBelowThresh) {
					if (ms.getScore() < this->getThresh()) {
						this->falseAcceptance++;
					} else {
						this->trueRejection++;
					}
				} else {
					if (ms.getScore() > this->getThresh()) {
						this->falseAcceptance++;
					} else {
						this->trueRejection++;
					}
				}
			}
		}
		void ThreshedStatistics::refresh() {
			this->clear();

			//std::cout << "update with thresh: " << this->getThresh() << " and accept " << (this->getDirection() == ThreshedStatisticsSettings::AcceptBelowThresh ? "bellow" : "above") << std::endl;

			for (auto score : this->statistics.getMethodResults().getGenuineScores()) {
				this->addScoreToStatistics(score, true);
			}
			for (auto score : this->statistics.getMethodResults().getImpostorScores()) {
				this->addScoreToStatistics(score, false);
			}

			if (this->getDirection() == ThreshedStatisticsSettings::AcceptBelowThresh) {
				this->overlapArea = Extremes(this->statistics.getImpostorExtremes().getMin(), this->statistics.getGenuineExtremes().getMax());
			} else {
				this->overlapArea = Extremes(this->statistics.getGenuineExtremes().getMin(), this->statistics.getImpostorExtremes().getMax());
			}

			//std::cout << "update done: ta: " << this->trueAcceptance << " tr: " << this->trueRejection << " fa: " << falseAcceptance << " fr: " << falseRejection << std::endl;
		}
		void ThreshedStatistics::clear() {
			this->overlapArea = Extremes();
			this->falseAcceptance = 0;
			this->falseRejection = 0;
			this->trueAcceptance = 0;
			this->trueRejection = 0;
		}

	} /* namespace Statistics */
} /* namespace BioFW */
