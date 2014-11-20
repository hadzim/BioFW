/*
 * WLDHistogramExtractor.h
 *
 *  Created on: 10.3.2014
 *      Author: JV
 */

#ifndef LBPaHISTOGRAMEXTRACTOR_H_
#define LBPaHISTOGRAMEXTRACTOR_H_
#include "SpatialHistogramExtractor.h"
#include "BioFW/Comparison/HistogramComparison.h"

namespace BioFW {

//------------------------------------------------------------------------------
// cv::elbp
//------------------------------------------------------------------------------
	template<typename _Tp> static
	inline void elbp_(cv::InputArray _src, cv::OutputArray _dst, int radius, int neighbors) {
		//get matrices
		cv::Mat src = _src.getMat();
		// allocate memory for result
		_dst.create(src.rows - 2 * radius, src.cols - 2 * radius, CV_32SC1);
		cv::Mat dst = _dst.getMat();
		// zero
		dst.setTo(0);
		for (int n = 0; n < neighbors; n++) {
			// sample points
			float x = static_cast<float>(-radius * sin(2.0 * CV_PI * n / static_cast<float>(neighbors)));
			float y = static_cast<float>(radius * cos(2.0 * CV_PI * n / static_cast<float>(neighbors)));
			// relative indices
			int fx = static_cast<int>(floor(x));
			int fy = static_cast<int>(floor(y));
			int cx = static_cast<int>(ceil(x));
			int cy = static_cast<int>(ceil(y));
			// fractional part
			float ty = y - fy;
			float tx = x - fx;
			// set interpolation weights
			float w1 = (1 - tx) * (1 - ty);
			float w2 = tx * (1 - ty);
			float w3 = (1 - tx) * ty;
			float w4 = tx * ty;
			// iterate through your data
			for (int i = radius; i < src.rows - radius; i++) {
				for (int j = radius; j < src.cols - radius; j++) {
					// calculate interpolated value
					float t = static_cast<float>(w1 * src.at < _Tp > (i + fy, j + fx) + w2 * src.at < _Tp > (i + fy, j + cx) + w3 * src.at < _Tp > (i + cy, j + fx) + w4 * src.at < _Tp > (i + cy, j + cx));
					// floating point precision, so check some machine-dependent epsilon
					dst.at<int>(i - radius, j - radius) += ((t > src.at < _Tp > (i, j)) || (std::abs(t - src.at < _Tp > (i, j)) < std::numeric_limits<float>::epsilon())) << n;
				}
			}
		}
	}

	static void elbp(cv::InputArray src, cv::OutputArray dst, int radius, int neighbors) {
		int type = src.type();
		switch (type) {
			case CV_8SC1:
				elbp_<char>(src, dst, radius, neighbors);
				break;
			case CV_8UC1:
				elbp_<unsigned char>(src, dst, radius, neighbors);
				break;
			case CV_16SC1:
				elbp_<short>(src, dst, radius, neighbors);
				break;
			case CV_16UC1:
				elbp_<unsigned short>(src, dst, radius, neighbors);
				break;
			case CV_32SC1:
				elbp_<int>(src, dst, radius, neighbors);
				break;
			case CV_32FC1:
				elbp_<float>(src, dst, radius, neighbors);
				break;
			case CV_64FC1:
				elbp_<double>(src, dst, radius, neighbors);
				break;
			default:
				std::string error_msg = cv::format("Using Original Local Binary Patterns for feature extraction only works on single-channel images (given %d). Please pass the image data as a grayscale image!", type);
				CV_Error(CV_StsNotImplemented, error_msg);
				break;
		}
	}

	static cv::Mat histc_(const cv::Mat& src, int minVal = 0, int maxVal = 255, bool normed = false) {
		cv::Mat result;
		// Establish the number of bins.
		int histSize = maxVal - minVal + 1;
		// Set the ranges.
		float range[] = { static_cast<float>(minVal), static_cast<float>(maxVal + 1) };
		const float* histRange = { range };
		// calc histogram
		cv::calcHist(&src, 1, 0, cv::Mat(), result, 1, &histSize, &histRange, true, false);
		// normalize
		if (normed) {
			result /= (int) src.total();
		}
		return result.reshape(1, 1);
	}

	static cv::Mat histc(cv::InputArray _src, int minVal, int maxVal, bool normed) {
		cv::Mat src = _src.getMat();
		switch (src.type()) {
			case CV_8SC1:
				return histc_(cv::Mat_<float>(src), minVal, maxVal, normed);
				break;
			case CV_8UC1:
				return histc_(src, minVal, maxVal, normed);
				break;
			case CV_16SC1:
				return histc_(cv::Mat_<float>(src), minVal, maxVal, normed);
				break;
			case CV_16UC1:
				return histc_(src, minVal, maxVal, normed);
				break;
			case CV_32SC1:
				return histc_(cv::Mat_<float>(src), minVal, maxVal, normed);
				break;
			case CV_32FC1:
				return histc_(src, minVal, maxVal, normed);
				break;
			default:
				CV_Error(CV_StsUnmatchedFormats, "This type is not implemented yet.");
				break;
		}
		return cv::Mat();
	}

	static cv::Mat spatial_histogram(cv::InputArray _src, int numPatterns, int grid_x, int grid_y, bool /*normed*/) {
		cv::Mat src = _src.getMat();
		// calculate LBP patch size
		int width = src.cols / grid_x;
		int height = src.rows / grid_y;
		// allocate memory for the spatial histogram
		cv::Mat result = cv::Mat::zeros(grid_x * grid_y, numPatterns, CV_32FC1);
		// return matrix with zeros if no data was given
		if (src.empty())
			return result.reshape(1, 1);
		// initial result_row
		int resultRowIdx = 0;
		// iterate through grid
		for (int i = 0; i < grid_y; i++) {
			for (int j = 0; j < grid_x; j++) {
				cv::Mat src_cell = cv::Mat(src, cv::Range(i * height, (i + 1) * height), cv::Range(j * width, (j + 1) * width));
				cv::Mat cell_hist = histc(src_cell, 0, (numPatterns - 1), true);
				// copy to the result matrix
				cv::Mat result_row = result.row(resultRowIdx);
				cell_hist.reshape(1, 1).convertTo(result_row, CV_32FC1);
				// increase row count in result matrix
				resultRowIdx++;
			}
		}
		// return result as reshaped feature vector
		return result.reshape(1, 1);
	}

//------------------------------------------------------------------------------
// wrapper to cv::elbp (extended local binary patterns)
//------------------------------------------------------------------------------

	static cv::Mat elbp(cv::InputArray src, int radius, int neighbors) {
		cv::Mat dst;
		elbp(src, dst, radius, neighbors);
		return dst;
	}



	class LBPHistogramExtractor: public IFeatureVectorExtractor<MatFV, MatFV> {
		private:
			int _grid_x;
			int _grid_y;
			int _radius;
			int _neighbors;
			double _threshold;
		public:

			typedef HistogramComparator DefaultComparator;


			LBPHistogramExtractor(int radius_ = 1, int neighbors_ = 8, int gridx = 8, int gridy = 8, double threshold = DBL_MAX) :
					_grid_x(gridx), _grid_y(gridy), _radius(radius_), _neighbors(neighbors_), _threshold(threshold) {
			}
			virtual ~LBPHistogramExtractor() {

			}

			virtual FeatureVector extractFeatureVector(const Sample & input) {
				// calculate lbp image
				cv::Mat lbp_image = elbp(input.mat(), _radius, _neighbors);
				// get spatial histogram from this lbp image
				cv::Mat p = spatial_histogram(lbp_image, /* lbp_image */
					static_cast<int>(std::pow(2.0, static_cast<double>(_neighbors))), /* number of possible patterns */
					_grid_x, /* grid size x */
					_grid_y, /* grid size y */
				true);
				return MatFV(p);
			}

	};



} /* namespace BioFW */
#endif /* WLDHISTOGRAMEXTRACTOR_H_ */
