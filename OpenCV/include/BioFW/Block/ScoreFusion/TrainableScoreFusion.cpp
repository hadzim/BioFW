/*
 * TrainableScoreFusion.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "TrainableScoreFusion.h"

namespace BioFW {

	TrainableScoreFusion::TrainableScoreFusion(std::string name, IScoreNormalizer::Ptr scoreNormalizer_) :
			name(name), isTrained_(false), scoreNormalizer_(scoreNormalizer_) {

	}

	TrainableScoreFusion::~TrainableScoreFusion() {

	}

	bool TrainableScoreFusion::isTrained() {
		return isTrained_;
	}

	static ComparisonScore::Subscores scores2subscores(const Result::Scores & s) {
		ComparisonScore::Subscores res;
		for (auto rs : s) {
			res[rs.first] = rs.second.getScore();
		}
		return res;
	}

	static cv::Mat score2mat(const ComparisonScore::Subscores & s) {
		cv::Mat mm(1, s.size(), CV_32FC1);
		int index = 0;
		for (auto score : s) {
			mm.at<float>(index++) = (float) score.second;
		}
		return mm;
	}

	void TrainableScoreFusion::train(const TrainingData & results) {
		//prepare matrices
		cv::Mat response(results.getResults().size(), 1, CV_32FC1);
		cv::Mat data(0, results.getMethods().size(), CV_32FC1);

		int index = 0;

		for (auto r : results.getResults()) {

			data.push_back(score2mat(scores2subscores(r.getComparisonScores())));

			response.at<float>(index) = r.isGenuine() ? 1.0f : -1.0f;

			index++;
		}

		this->isTrained_ = this->internalTrain(data, response);
	}

	ComparisonScore TrainableScoreFusion::resolve(const ComparisonScore::Subscores & subscores) {
		if (this->isTrained()) {
			return ComparisonScore(this->internalResolve(score2mat(subscores)), subscores);
		} else {
			throw std::runtime_error("Score fusion not trained");
			//return ComparisonScore::invalid();
		}
	}

	std::string TrainableScoreFusion::getName() const {
		return this->name;
	}

	IScoreNormalizer::Ptr TrainableScoreFusion::scoreNormalizer(){
		return this->scoreNormalizer_;
	}

} /* namespace BioFW */
