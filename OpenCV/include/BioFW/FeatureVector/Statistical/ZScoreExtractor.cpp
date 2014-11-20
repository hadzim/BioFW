/*
 * ZScoreExtractor.cpp
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#include "ZScoreExtractor.h"
#include <opencv2/opencv.hpp>

namespace BioFW {

	ZScoreExtractor::ZScoreExtractor() {

	}

	ZScoreExtractor::~ZScoreExtractor() {

	}

	ZScoreExtractor::FeatureVector ZScoreExtractor::extractFeatureVector(const Sample & input){
		cv::Mat tmpMat =  cv::Mat::zeros(1, cols, CV_64FC1);
		input.mat().reshape(1,1).convertTo(tmpMat, CV_64FC1);

		tmpMat = (tmpMat - this->mean) / this->stddev;

		return MatFV(tmpMat);
	}

	void ZScoreExtractor::train(const Samples & inputs, const Labels & labels){
		if (inputs.empty()) throw std::runtime_error("Cannot compute z score of empty vector");

		this->cols = inputs.at(0).mat().total();
		this->mean = cv::Mat::zeros(1, cols, CV_64FC1);
		this->stddev = cv::Mat::zeros(1, cols, CV_64FC1);

		cv::Mat tmpMat = mean.clone();

		//mean
		double scale = 1.0 / (inputs.size() * 1.0);
		for (auto sample : inputs){
			sample.mat().reshape(1,1).convertTo(tmpMat, CV_64FC1, scale);
			mean += tmpMat;
		}

		//stddev
		for (auto sample : inputs){
			sample.mat().reshape(1,1).convertTo(tmpMat, CV_64FC1);
			tmpMat = tmpMat - mean;
			cv::pow(tmpMat, 2, tmpMat);
			this->stddev += tmpMat * scale;
		}
		cv::sqrt(tmpMat, tmpMat);
	}

} /* namespace BioFW */
