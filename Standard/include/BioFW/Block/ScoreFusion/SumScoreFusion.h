/*
 * SVMScoreFusion.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef SumSCOREFUSION_H_
#define SumSCOREFUSION_H_
#include "ScoreFusion.h"

namespace BioFW {

	class SumScoreFusion : public ScoreFusion {
		public:
			SumScoreFusion();
			virtual ~SumScoreFusion();

			virtual ComparisonScore resolve(const ComparisonScore::Subscores & matchingSubscores);

	};

} /* namespace BioFW */
#endif /* SVMSCOREFUSION_H_ */
