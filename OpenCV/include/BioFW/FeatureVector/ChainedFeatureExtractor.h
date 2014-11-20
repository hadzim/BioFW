/*
 * ChainedFeatureExtractor.h
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#ifndef CHAINEDFEATUREEXTRACTOR_H_
#define CHAINEDFEATUREEXTRACTOR_H_

namespace BioFW {

	template <class FeatureExtractor1, class FeatureExtractor2>
	class ChainedFeatureExtractor : public IFeatureVectorExtractor <typename FeatureExtractor1::Sample, typename FeatureExtractor2::FeatureVector> {
		public:
			typedef typename FeatureExtractor2::FeatureVector FeatureVector;
			typedef typename FeatureExtractor1::Sample Sample;

			ChainedFeatureExtractor(typename FeatureExtractor1::Ptr fe1, typename FeatureExtractor1::Ptr fe2) : fe1(fe1), fe2(fe2){

			}
			virtual ~ChainedFeatureExtractor(){

			}

			virtual FeatureVector extractFeatureVector(const Sample & input){
				typename FeatureExtractor2::Sample intermediate = fe1->extractFeatureVector(input);
				return fe2->extractFeatureVector(intermediate);
			}

			typedef std::vector <Sample> Samples;

			virtual void train(const Samples & inputs, const Labels & labels){
				fe1->train(inputs, labels);
				typename FeatureExtractor2::Samples samples2;
				for (auto s : inputs){
					samples2.push_back(fe1->extractFeatureVector(s));
				}
				fe2->train(samples2, labels);
			}
		private:
			typename FeatureExtractor1::Ptr fe1;
			typename FeatureExtractor2::Ptr fe2;
	};

} /* namespace BioFW */
#endif /* CHAINEDFEATUREEXTRACTOR_H_ */
