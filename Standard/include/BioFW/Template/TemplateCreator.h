/*
 * TemplateCreator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef TEMPLATECREATOR_H_
#define TEMPLATECREATOR_H_
#include "BioFW/Template/ITemplateCreator.h"

namespace BioFW {

		template<class TTemplate>
		class TemplateCreator: public ITemplateCreator<TTemplate> {
			public:
				typedef typename TTemplate::FeatureVector FeatureVector;

				TTemplate createTemplate(FeatureVector featureVector) {
					TTemplate t;
					t.addFeatureVector(featureVector);
					return t;
				}

				void addToTemplate(TTemplate & to, FeatureVector featureVector) {
					to.addFeatureVector(featureVector);
				}

				TTemplate createEmptyTemplate() {
					return TTemplate();
				}

		};


} /* namespace BioFW */
#endif /* TEMPLATECREATOR_H_ */
