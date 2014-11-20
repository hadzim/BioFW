/*
 * ResultsOverview.h
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#ifndef RESULTSOVERVIEW_H_
#define RESULTSOVERVIEW_H_
#include "BioFW/Evaluation/Results.h"
#include <opencv2/opencv.hpp>


namespace BioFW {
	namespace Utils {

		cv::Mat renderGenuineImpostorGraph(const MethodResults & r);


	}  // namespace Utils



}  // namespace BioFW



#endif /* RESULTSOVERVIEW_H_ */
