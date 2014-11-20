/*
 * BasicStatistics.cpp
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Statistics/BasicStatistics.h"

namespace BioFW {
	namespace Statistics {

		BasicStatistics::BasicStatistics(const MethodResults & results) :
				methodResults(results) {
			refresh();
		}

		const MethodResults & BasicStatistics::getMethodResults() const{
			return this->methodResults;
		}

		const Extremes & BasicStatistics::getGlobalExtremes() const {
			return this->globalExtremes;
		}
		const Extremes & BasicStatistics::getGenuineExtremes() const {
			return this->genuineExtremes;
		}
		const Extremes & BasicStatistics::getImpostorExtremes() const {
			return this->impostorExtremes;
		}

		int BasicStatistics::getGenuineCount() const {
			return this->genuineAvg.count;
		}

		int BasicStatistics::getImpostorCount() const {
			return this->impostorAvg.count;
		}

		int BasicStatistics::getTotalCount() const {
			return this->totalAvg.count;
		}

		double BasicStatistics::getGenuineMean() const {
			return this->genuineAvg.currentAverage;
		}
		double BasicStatistics::getImpostorMean() const {
			return this->impostorAvg.currentAverage;
		}
		double BasicStatistics::getGlobalMean() const {
			return this->totalAvg.currentAverage;
		}

		void BasicStatistics::refresh() {
			 this->clear();

			for (auto score : this->methodResults.getGenuineScores()) {
				this->addScoreToStatistics(score, true);
			}
			for (auto score : this->methodResults.getImpostorScores()) {
				this->addScoreToStatistics(score, false);
			}
		}
		void BasicStatistics::clear() {
			this->impostorExtremes = Extremes();
			this->globalExtremes= Extremes();
			this->genuineExtremes = Extremes();

			this->totalAvg = Averager();
			this->impostorAvg = Averager();
			this->genuineAvg = Averager();
		}

		void BasicStatistics::addScoreToStatistics(const ComparisonScore & ms, bool isGenuine) {
			if (!ms.isValid()) {
				return;
			}
			double score = ms.getScore();

			if (isGenuine) {
				this->genuineAvg.addData(score);
				this->genuineExtremes.update(score);
			} else {
				this->impostorAvg.addData(score);
				this->impostorExtremes.update(score);
			}
			this->totalAvg.addData(score);
			this->globalExtremes.update(score);
		}

}
/* namespace Statistics */
} /* namespace BioFW */
