/*
 * Template.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef TEMPLATE_H_
#define TEMPLATE_H_
#include "BioFW/Template/ITemplate.h"
#include <memory>

namespace BioFW {

		template <class TFeatureVector>
		class Template : public ITemplate<TFeatureVector>  {
			public:

				typedef std::shared_ptr <Template> Ptr;

				//typedef TFeatureVector FeatureVector;
				//typedef std::vector<FeatureVector> FeatureVectors;
				typedef typename ITemplate<TFeatureVector>::const_iterator const_iterator;

				//Template();
				virtual ~Template(){}

		        virtual void addFeatureVector(TFeatureVector featureVector){
		        	this->featureVectors.push_back(featureVector);
		        }

		        virtual const_iterator begin() const {
		        	return this->featureVectors.begin();
		        }
		        virtual const_iterator end() const {
		        	return this->featureVectors.end();
		        }
			private:
		        typename ITemplate<TFeatureVector>::FeatureVectors featureVectors;

		};

} /* namespace BioFW */
#endif /* TEMPLATE_H_ */
