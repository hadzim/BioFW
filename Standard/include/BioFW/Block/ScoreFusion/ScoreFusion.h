/*
 * TrainableScoreFusion.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef SCOREFUSION_H_
#define SCOREFUSION_H_

#include "BioFW/Block/ScoreFusion/IScoreFusion.h"

namespace BioFW {

	class ScoreFusion: public IScoreFusion {

		public:
			typedef std::shared_ptr<ScoreFusion> Ptr;
			ScoreFusion(std::string name, IScoreNormalizer::Ptr scoreNormalizer_);
			virtual ~ScoreFusion();

		public:

			void train(const TrainingData & trainingData);


			std::string getName() const;

			IScoreNormalizer::Ptr scoreNormalizer();

		private:
			std::string name;
			IScoreNormalizer::Ptr scoreNormalizer_;
	};

} /* namespace BioFW */
#endif /* TRAINABLESCOREFUSION_H_ */
