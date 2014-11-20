/*
 * WLDHistogramExtractor.h
 *
 *  Created on: 10.3.2014
 *      Author: JV
 */

#ifndef WLDHISTOGRAMEXTRACTOR_H_
#define WLDHISTOGRAMEXTRACTOR_H_

#include "SpatialHistogramExtractor.h"


namespace BioFW {

	//template<class TBiometricSample = MatSample, class TFeatureVector = MatFV>
	class WLDHistogramExtractor: public SpatialHistogramExtractor<MatSample, MatFV> {
		public:

			enum {
				size_center = 4,   // num bits from the center
				size_theta_n = 2,   // orientation channels used
				size_theta_w = 8,   // each theta orientation channel is 8*w
				size_zeta = 32,  // bins for zeta

				size_theta = 8 * size_theta_w,
				size_all = (1 << size_center) + size_zeta + size_theta_n * size_theta

			// 176 bytes per patch, * 8 * 8 = 11264 bytes per image.
			};

			WLDHistogramExtractor(int gridx = 8, int gridy = 8, double threshold = DBL_MAX) :
				SpatialHistogramExtractor<MatSample, MatFV>(gridx, gridy, threshold, size_all, CV_8U) {

			}
			virtual ~WLDHistogramExtractor() {
			}

			typedef NormComparator<cv::NORM_L1, MatFV, MatFV> DefaultComparator;

		protected:
			virtual void oper(const cv::Mat & src, cv::Mat & hist) {

				int radius = 1;
				for (int i = radius; i < src.rows - radius; i++) {
					for (int j = radius; j < src.cols - radius; j++) {
						// 7 0 1
						// 6 c 2
						// 5 4 3
						uchar c = src.at<uchar>(i, j);
						uchar n[8] = { src.at<uchar>(i - 1, j), src.at<uchar>(i - 1, j + 1), src.at<uchar>(i, j + 1), src.at<uchar>(i + 1, j + 1), src.at<uchar>(i + 1, j), src.at<uchar>(i + 1, j - 1), src.at<uchar>(i,
								j - 1), src.at<uchar>(i - 1, j - 1) };
						//// circular, radius 2 neighbourhood with *linear* interpolated corners
						//uchar n[8]= {
						//    src.at<uchar>(i-2,j),(
						//        src.at<uchar>(i-2,j+1)+
						//        src.at<uchar>(i-2,j+2)+
						//        src.at<uchar>(i-1,j+1)+
						//        src.at<uchar>(i-1,j+2)) / 4,
						//    src.at<uchar>(i,j+2),(
						//        src.at<uchar>(i+1,j+1)+
						//        src.at<uchar>(i+1,j+2)+
						//        src.at<uchar>(i+2,j+1)+
						//        src.at<uchar>(i+2,j+2)) / 4,
						//    src.at<uchar>(i+2,j),(
						//        src.at<uchar>(i+1,j-1)+
						//        src.at<uchar>(i+1,j-2)+
						//        src.at<uchar>(i+2,j-1)+
						//        src.at<uchar>(i+2,j-2)) / 4,
						//    src.at<uchar>(i,j-2), (
						//        src.at<uchar>(i-1,j-1)+
						//        src.at<uchar>(i-1,j-2)+
						//        src.at<uchar>(i-2,j-1)+
						//        src.at<uchar>(i-2,j-2)) / 4
						//};
						int p = n[0] + n[1] + n[2] + n[3] + n[4] + n[5] + n[6] + n[7];
						p -= c * 8;
						// (7), projected from [-pi/2,pi/2] to [0,size_zeta]
						double zeta = 0;
						if (p != 0)
							zeta = double(size_zeta) * (atan(double(p) / c) + M_PI * 0.5) / M_PI;
						hist.at<uchar>(int(zeta)) += 1;

						// (11), projected from [-pi/2,pi/2] to [0,size_theta]
						for (int i = 0; i < size_theta_n; i++) {
							double a = atan2(double(n[i] - n[(i + 4) % 8]), double(n[(i + 2) % 8] - n[(i + 6) % 8]));
							double theta = M_PI_4 * fmod((a + M_PI) / M_PI_4 + 0.5f, 8) * size_theta_w; // (11)
							hist.at<uchar>(int(theta) + size_zeta + size_theta * i) += 1;
						}

						// additionally, add some bits of the actual center value (MSB).
						int cen = c >> (8 - size_center);
						hist.at<uchar>(cen + size_zeta + size_theta * size_theta_n) += 1;
					}
				}

			}
	};

} /* namespace BioFW */
#endif /* WLDHISTOGRAMEXTRACTOR_H_ */
