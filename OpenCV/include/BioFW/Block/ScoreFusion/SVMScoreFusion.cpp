/*
 * SVMScoreFusion.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "SVMScoreFusion.h"
#include "BioFW/Block/ScoreNormalization/ZScoreNormalizer.h"
#include "BioFW/Block/ScoreNormalization/MinMaxScoreNormalizer.h"
#include "BioFW/Block/ScoreNormalization/AvgScoreNormalizer.h"

namespace BioFW {

	SVMScoreFusion::SVMScoreFusion() : TrainableScoreFusion("SVM", std::make_shared<AvgScoreNormalizer>()) {

	}

	SVMScoreFusion::~SVMScoreFusion() {

	}

	bool SVMScoreFusion::internalTrain(cv::Mat & inputs, cv::Mat & outputs) {
/*

		cv::SVMParams p;
		p.svm_type = CvSVM::NU_SVC;
		p.kernel_type = CvSVM::LINEAR;
		p.nu = 0.3;
		svm.clear();
		svm.train_auto(inputs, outputs, cols, rows, p);
		return true;*/

		cv::SVMParams params;
		params.svm_type = cv::SVM::C_SVC;
		params.kernel_type = cv::SVM::LINEAR;
		params.term_crit   = cv::TermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);
		svm.train(inputs, outputs, cv::Mat(), cv::Mat(), params);
		return true;
	}

	float SVMScoreFusion::internalResolve(const cv::Mat & input) {
		return svm.predict(input, true);
	}

} /* namespace BioFW */
