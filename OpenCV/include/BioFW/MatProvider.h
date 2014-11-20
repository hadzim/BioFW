/*
 * MatProvider.h
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#ifndef MATPROVIDER_H_
#define MATPROVIDER_H_
#include <opencv2/opencv.hpp>

namespace BioFW {

	class MatProvider {
		public:
			MatProvider();
			MatProvider(const cv::Mat & m_);
			virtual ~MatProvider();

			cv::Mat & mat();
			const cv::Mat & mat() const;
		private:
			cv::Mat m;
	};

} /* namespace BioFW */
#endif /* MATPROVIDER_H_ */
