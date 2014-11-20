/*
 * CombinedLBPHHistogramExtractor.h
 *
 *  Created on: 10.3.2014
 *      Author: JV
 */

#ifndef CLBPDistCOMBINEDLBPHHISTOGRAMEXTRACTOR_H_
#define CLBPDistCOMBINEDLBPHHISTOGRAMEXTRACTOR_H_

#include "opencv2/contrib/contrib.hpp"
#include "SpatialHistogramExtractor.h"

namespace BioFW {

	//template<class TBiometricSample = MatSample, class TFeatureVector = MatFV>
		class CLBPDistHistogramExtractor: public SpatialHistogramExtractor<MatSample, MatFV> {
			public:



			CLBPDistHistogramExtractor(int gridx = 8, int gridy = 8, double threshold = DBL_MAX)
		        : SpatialHistogramExtractor<MatSample, MatFV>(gridx,gridy,threshold,0,CV_8U,0), _numBits(4+1) // added 1 center bits
		          , _scaleFac(0.5f){

				}

				virtual ~CLBPDistHistogramExtractor() {
				}

				typedef NormComparator<cv::NORM_L2SQR, MatFV, MatFV> DefaultComparator;

			protected:

				static void pattern(const cv::Mat & src, cv::Mat & lbp) {
				    // calculate patterns
				    int radius = 1;
				    int t = 0; // threshold
				    for(int i=radius;i<src.rows-radius;i++)
				    {
				        for(int j=radius;j<src.cols-radius;j++)
				        {
				            int k = 0;
				            //
				            // 7 0 1
				            // 6 c 2
				            // 5 4 3
				            //
				            // center and ring of neighbours:
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
				            // save 4 bits ( 1 for each of 4 possible diagonals )
				            //  _\|/_
				            //   /|\
				            // this is the "central symmetric LBP" idea, from :
				            // "Description of Interest Regions with Center-Symmetric Local Binary Patterns"
				            // (http://www.ee.oulu.fi/mvg/files/pdf/pdf_750.pdf).
				            //
				            uchar & code = lbp.at<uchar>(i,j);
				            code += ((n[0]-n[4])>t)<<k++;
				            code += ((n[1]-n[5])>t)<<k++;
				            code += ((n[2]-n[6])>t)<<k++;
				            code += ((n[3]-n[7])>t)<<k++;
				            // 1 bit of center value
				            code += (c > ((n[0]+n[1]+n[2]+n[3]+n[4]+n[5]+n[6]+n[7])>>3))<<k++;
				        }
				    }
				}



				virtual void oper(const cv::Mat & src, cv::Mat & hist) {
					cv::Size s = src.size();
					cv::Mat lbp = cv::Mat::zeros(s, CV_8U);

					    pattern(src,lbp);

					    // linear concatenation of all the bitplanes run through a distance transform
					    //
					    // resizing output dist_img by 0.5
					    //
					    cv::Mat dst,dt,rz;
					    for ( int i=0; i<_numBits; i++ )
					    {
					        cv::distanceTransform( (lbp&(1<<i)), dt, CV_DIST_L2, 3 );
					        cv::resize(dt, rz, cv::Size(), _scaleFac,_scaleFac);
					        dst.push_back(rz.reshape(0,1));
					    }
					    hist = dst.reshape(0,1);
				}
			private:
				int _numBits;
			    float _scaleFac; //! (down)scale output bitplanes

		};


} /* namespace BioFW */
#endif /* COMBINEDLBPHHISTOGRAMEXTRACTOR_H_ */
