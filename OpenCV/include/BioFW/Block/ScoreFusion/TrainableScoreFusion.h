/*
 * TrainableScoreFusion.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef TRAINABLESCOREFUSION_H_
#define TRAINABLESCOREFUSION_H_

#include <opencv2/opencv.hpp>
#include "BioFW/Block/ScoreFusion/IScoreFusion.h"

namespace BioFW {

	class TrainableScoreFusion: public IScoreFusion {

		public:
			typedef std::shared_ptr<TrainableScoreFusion> Ptr;
			TrainableScoreFusion(std::string name, IScoreNormalizer::Ptr scoreNormalizer_);
			virtual ~TrainableScoreFusion();

		public:

			void train(const TrainingData & trainingData);

			ComparisonScore resolve(const ComparisonScore::Subscores & comparisonScore);

			std::string getName() const;

			IScoreNormalizer::Ptr scoreNormalizer();

		protected:

			virtual bool internalTrain(cv::Mat & inputs, cv::Mat & outputs) = 0;

			virtual float internalResolve(const cv::Mat & input) = 0;

			bool isTrained();



		private:
			std::string name;
			bool isTrained_;
			IScoreNormalizer::Ptr scoreNormalizer_;

	};

} /* namespace BioFW */
#endif /* TRAINABLESCOREFUSION_H_ */
