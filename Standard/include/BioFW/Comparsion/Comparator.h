/*
 * Comparator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_
#include "BioFW/Comparison/IFeatureVectorComparator.h"
#include "BioFW/Comparison/IComparator.h"
#include "BioFW/Comparsion/ScoreSelector/IScoreSelector.h"
#include "BioFW/Comparsion/ScoreSelector/ScoreSelector.h"

namespace BioFW {

		template <class TProbeFeatureVector, class TTemplate>
		class Comparator : public IComparator<TProbeFeatureVector, TTemplate> {
			public:
				typedef std::shared_ptr <Comparator> Ptr;
				typedef typename TTemplate::FeatureVector ReferenceFeatureVector;

				Comparator(
						typename IFeatureVectorComparator<TProbeFeatureVector, ReferenceFeatureVector>::Ptr featureComparator,
						IScoreSelector::Ptr scoreSelector)
					: featureComparator(featureComparator),
					  scoreSelector(scoreSelector){

				}

				Comparator(
						typename IFeatureVectorComparator<TProbeFeatureVector, ReferenceFeatureVector>::Ptr featureComparator)
					: featureComparator(featureComparator),
					  scoreSelector(new MinScoreSelector()){

				}

				virtual ~Comparator(){

				}

				virtual ComparisonScore computeComparisonScore(const TProbeFeatureVector & probe, const TTemplate & referenceTemplate){
					ComparisonScore::List comparisonScores;
					for (typename TTemplate::const_iterator ti = referenceTemplate.begin(); ti != referenceTemplate.end(); ti++) {
						comparisonScores.push_back(this->featureComparator->computeComparisonScore(probe, *ti));
					}
					return scoreSelector->selectScore(comparisonScores);
				}
			private:
				typename IFeatureVectorComparator<TProbeFeatureVector, ReferenceFeatureVector>::Ptr featureComparator;
				IScoreSelector::Ptr scoreSelector;
		};


} /* namespace BioFW */
#endif /* COMPARATOR_H_ */
