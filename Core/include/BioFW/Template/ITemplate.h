/*
 * ITemplate.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef ITEMPLATE_H_
#define ITEMPLATE_H_
#include <memory>
#include <vector>

namespace BioFW {
	template <class TFeatureVector>
	class ITemplate {
		public:
			typedef std::shared_ptr <ITemplate> Ptr;

			typedef TFeatureVector FeatureVector;
			typedef std::vector<FeatureVector> FeatureVectors;
			typedef typename FeatureVectors::const_iterator const_iterator;

			virtual ~ITemplate(){}

	        virtual void addFeatureVector(TFeatureVector featureVector) = 0;

	        virtual const_iterator begin() const = 0;
	        virtual const_iterator end() const = 0;
	};
}  // namespace BioFW



#endif /* ITEMPLATE_H_ */
