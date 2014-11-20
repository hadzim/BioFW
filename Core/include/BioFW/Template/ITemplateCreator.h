/*
 * ITemplateCreator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef ITEMPLATECREATOR_H_
#define ITEMPLATECREATOR_H_
#include <memory>

namespace BioFW {

	template <class TTemplate>
	class ITemplateCreator {
		public:
			typedef std::shared_ptr <ITemplateCreator> Ptr;

			typedef typename TTemplate::FeatureVector FeatureVector;

			virtual ~ITemplateCreator(){}

			virtual TTemplate createTemplate(FeatureVector featureVector) = 0;
			virtual TTemplate createEmptyTemplate() = 0;

			virtual void addToTemplate(TTemplate & to, FeatureVector featureVector) = 0;
	};

} /* namespace BioFW */
#endif /* ITEMPLATECREATOR_H_ */
