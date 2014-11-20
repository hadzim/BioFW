/*
 * MatProvider.cpp
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#include "MatProvider.h"

namespace BioFW {

	MatProvider::MatProvider() {

	}
	MatProvider::MatProvider(const cv::Mat & m_) : m(m_){

	}

	MatProvider::~MatProvider() {

	}

	cv::Mat & MatProvider::mat() {
		return this->m;
	}
	const cv::Mat & MatProvider::mat() const {
		return this->m;
	}

} /* namespace BioFW */
