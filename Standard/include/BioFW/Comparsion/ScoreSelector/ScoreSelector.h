/*
 * MinScoreSelector.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef MINSCORESELECTOR_H_
#define MINSCORESELECTOR_H_
#include "IScoreSelector.h"

namespace BioFW {

		class MinScoreSelector: public IScoreSelector {
			public:
				virtual ComparisonScore selectScore(const ComparisonScore::List & scores);
		};

		class MaxScoreSelector: public IScoreSelector {
			public:
				virtual ComparisonScore selectScore(const ComparisonScore::List & scores);
		};


} /* namespace BioFW */
#endif /* MINSCORESELECTOR_H_ */
