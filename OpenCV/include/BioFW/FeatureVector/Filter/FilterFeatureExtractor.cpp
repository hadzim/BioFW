/*
 * FilterFeatureExtractor.cpp
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#include "FilterFeatureExtractor.h"
#include <cmath>
#include "BioFW/FeatureVector/Filter/helpers/common.h"

using std::sqrt;

namespace BioFW {

	FilterFeatureExtractor::FilterFeatureExtractor(const cv::Mat &realKernel, const cv::Mat &imagKernel, double imageScale) :
			realKernel(realKernel), imagKernel(imagKernel), imageScale(imageScale) {

	}

	FilterFeatureExtractor::~FilterFeatureExtractor() {

	}

	cv::Mat FilterFeatureExtractor::extractPhaseBinaryCode(const cv::Mat &inputImage) const {
		Matrix realResponse;
		Matrix imagResponse;
		cv::filter2D(inputImage, realResponse, CV_64F, realKernel);
		cv::filter2D(inputImage, imagResponse, CV_64F, imagKernel);

		cv::Size newSize(inputImage.cols * imageScale, inputImage.rows * imageScale);
		cv::resize(realResponse, realResponse, newSize);
		cv::resize(imagResponse, imagResponse, newSize);

		Matrix result(newSize.width * newSize.height * 2, 1);
		int i = 0;
		for (int r = 0; r < realResponse.rows; r++) {
			for (int c = 0; c < realResponse.cols; c++) {
				result(i++) = realResponse(r, c) > 0;
				result(i++) = imagResponse(r, c) > 0;
			}
		}

		return result;
	}

	cv::Mat FilterFeatureExtractor::extractAbsoluteResponse(const cv::Mat &inputImage) const {
		Matrix realResponse;
		Matrix imagResponse;
		cv::filter2D(inputImage, realResponse, CV_64F, realKernel);
		cv::filter2D(inputImage, imagResponse, CV_64F, imagKernel);
/*
		{
			cv::Mat kernel1, kernel2;
			cv::normalize(realKernel, kernel1, 0, 255, cv::NORM_MINMAX, CV_8UC1);
			cv::normalize(imagKernel, kernel2, 0, 255, cv::NORM_MINMAX, CV_8UC1);

			cv::imshow("real", kernel1);
			cv::imshow("imag", kernel2);
		}
		{
			cv::Mat ds1, dst2;
			cv::normalize(realResponse, ds1, 0, 255, cv::NORM_MINMAX, CV_8UC1);
			cv::normalize(imagResponse, dst2, 0, 255, cv::NORM_MINMAX, CV_8UC1);

			cv::imshow("real resp", ds1);
			cv::imshow("imag resp", dst2);

			cv::Mat orig;
			cv::normalize(inputImage, orig, 0, 255, cv::NORM_MINMAX, CV_8UC1);

			cv::imshow("orig", orig);

		}
*/
		cv::Size newSize(inputImage.cols * imageScale, inputImage.rows * imageScale);
		cv::resize(realResponse, realResponse, newSize);
		cv::resize(imagResponse, imagResponse, newSize);

		Matrix result(newSize);
		int i = 0;
		for (int r = 0; r < realResponse.rows; r++) {
			for (int c = 0; c < realResponse.cols; c++) {
				result(i++) = std::sqrt(realResponse(r, c) * realResponse(r, c) + imagResponse(r, c) * imagResponse(r, c));
			}
		}
/*
		{
			cv::Mat orig;
						cv::normalize(result, orig, 0, 255, cv::NORM_MINMAX, CV_8UC1);

						cv::imshow("result abs", orig);
						cv::waitKey(4000);
		}*/
		return result;
	}

} /* namespace BioFW */
