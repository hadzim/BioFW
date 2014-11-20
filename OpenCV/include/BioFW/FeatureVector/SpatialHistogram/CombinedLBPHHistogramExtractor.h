/*
 * CombinedLBPHHistogramExtractor.h
 *
 *  Created on: 10.3.2014
 *      Author: JV
 */

#ifndef COMBINEDLBPHHISTOGRAMEXTRACTOR_H_
#define COMBINEDLBPHHISTOGRAMEXTRACTOR_H_

namespace BioFW {

	template<class TBiometricSample = MatSample, class TFeatureVector = MatFV>
		class CombinedLBPHHistogramExtractor: public SpatialHistogramExtractor<TBiometricSample, TFeatureVector> {
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

				CombinedLBPHHistogramExtractor( int gridx=8, int gridy=8, double threshold = DBL_MAX)
		        : SpatialHistogramExtractor<TBiometricSample, TFeatureVector>(gridx,gridy,threshold, 8+8+16+16+16,CV_8U,0){

				}

				virtual ~CombinedLBPHHistogramExtractor() {
				}

				typedef NormComparator<cv::NORM_HAMMING2, TFeatureVector, TFeatureVector> DefaultComparator;

			protected:
				virtual void oper(const cv::Mat & src, cv::Mat & hist) {
				int t = 0; // experimenting with cslbp threshold
				    // calculate patterns
				    // hardcoded 1 pixel radius
				    for(int i=1;i<src.rows-1;i++) {
				        for(int j=1;j<src.cols-1;j++) {
				            //
				            // 7 0 1
				            // 6 c 2
				            // 5 4 3
				            //
				            uchar c   = src.at<uchar>(i,j);
				            uchar n[8]= {
				                src.at<uchar>(i-1,j),
				                src.at<uchar>(i-1,j+1),
				                src.at<uchar>(i,j+1),
				                src.at<uchar>(i+1,j+1),
				                src.at<uchar>(i+1,j),
				                src.at<uchar>(i+1,j-1),
				                src.at<uchar>(i,j-1),
				                src.at<uchar>(i-1,j-1)
				            };
				            unsigned code = 0;
				            unsigned offset = 0;
				            // save 4 bits ( 1 for each of 4 possible diagonals )
				            //  _ / | / _
				            //    / | /
				            // this is the "central symmetric LBP" idea, from
				            // "Description of Interest Regions with Center-Symmetric Local Binary Patterns"
				            // (http://www.ee.oulu.fi/mvg/files/pdf/pdf_750.pdf).
				            //
				            code = 0;
				            code |= (n[0]-n[4]>t) << 0;
				            code |= (n[1]-n[5]>t) << 1;
				            code |= (n[2]-n[6]>t) << 2;
				            code |= (n[3]-n[7]>t) << 3;
				            hist.at<uchar>(code + offset) += 1;
				            offset += 16;

				            // save 4 bits ( 1 for each of 4 possible diagonal edges )
				            //    / /
				            //    / /
				            //
				            code = 0;
				            code |= (n[0]-n[2]>t) << 0;
				            code |= (n[2]-n[4]>t) << 1;
				            code |= (n[4]-n[6]>t) << 2;
				            code |= (n[6]-n[0]>t) << 3;
				            hist.at<uchar>(code + offset) += 1;
				            offset += 16;

				            //// save 4 bits ( 1 for each of 4 possible outer edges )
				            ////  _ _
				            //// |   |
				            //// |_ _|
				            ////
				            code = 0;
				            code |= (n[7]-n[1]>t) << 0;
				            code |= (n[1]-n[3]>t) << 1;
				            code |= (n[3]-n[5]>t) << 2;
				            code |= (n[5]-n[7]>t) << 3;
				            hist.at<uchar>(code + offset) += 1;
				            offset += 16;

				            // save 3 bits of max neighbour value ( it's index )
				            code = 0;
				            int m=-1;
				            for ( int k=0; k<8; k++ )
				            {
				                if (n[k] > m) { m=n[k]; code=k; }
				            }
				            hist.at<uchar>(code + offset) += 1;
				            offset += 8;

				            // save 3 bits (MSB) of center value:
				            code = ((c >> 5) & 0x07);
				            hist.at<uchar>(code + offset) += 1;
				            offset += 8;
				        }
				    }

				}
		};


} /* namespace BioFW */
#endif /* COMBINEDLBPHHISTOGRAMEXTRACTOR_H_ */
