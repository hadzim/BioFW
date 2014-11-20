/*
 * SVMScoreFusion.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef SVMSCOREFUSION_H_
#define SVMSCOREFUSION_H_
#include "TrainableScoreFusion.h"

namespace BioFW {

	class SVMScoreFusion : public TrainableScoreFusion {
		public:
			SVMScoreFusion();
			virtual ~SVMScoreFusion();

		protected:

			virtual bool internalTrain(cv::Mat & inputs, cv::Mat & outputs);

			virtual float internalResolve(const cv::Mat & input);
		private:
			cv::SVM svm;
	};

} /* namespace BioFW */
#endif /* SVMSCOREFUSION_H_ */
