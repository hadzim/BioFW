/*
 * GaborFeatureExtractor.cpp
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#include "GaborFeatureExtractor.h"
#include "BioFW/FeatureVector/Filter/helpers/gabor.h"

namespace BioFW {

	GaborFeatureExtractor::GaborFeatureExtractor(int frequency, int orientation, double scale) {
		Matrix realKernel, imagKernel;
		Face::LinAlg::Gabor::createWavelet(realKernel, imagKernel, frequency, orientation);
		filter = FilterFeatureExtractor::Ptr(new FilterFeatureExtractor(realKernel, imagKernel, scale));
	}

	GaborFeatureExtractor::~GaborFeatureExtractor() {

	}

	GaborFeatureExtractor::FeatureVector GaborFeatureExtractor::extractFeatureVector(const Sample & input){


		return MatFV(filter->extractAbsoluteResponse(input.mat()));
	}

} /* namespace BioFW */
