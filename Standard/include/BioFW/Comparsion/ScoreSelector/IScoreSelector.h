/*
 * IScoreSelector.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef ISCORESELECTOR_H_
#define ISCORESELECTOR_H_
#include <memory>
#include "BioFW/Comparison/ComparisonScore.h"

namespace BioFW {

		class IScoreSelector {
			public:
				typedef std::shared_ptr <IScoreSelector> Ptr;

				virtual ~IScoreSelector();

				virtual ComparisonScore selectScore(const ComparisonScore::List & scores) = 0;
		};


} /* namespace BioFW */
#endif /* ISCORESELECTOR_H_ */
