/*
 * ScoreNormalizingProcessingBlock.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef SCORENORMALIZINGPROCESSINGBLOCK_H_
#define SCORENORMALIZINGPROCESSINGBLOCK_H_
#include "BioFW/Block/IProcessingBlock.h"
#include "IScoreNormalizer.h"

namespace BioFW {
	template<class TBiometricSample, class TScoreNormalizer>
	class ScoreNormalizingProcessingBlock: public IProcessingBlock<TBiometricSample> {
		public:
			typedef TScoreNormalizer ScoreNormalizer;
			typedef typename TScoreNormalizer::Ptr ScoreNormalizerPtr;
			typedef size_t TemplateIndex;
		public:
			ScoreNormalizingProcessingBlock(typename IProcessingBlock<TBiometricSample>::Ptr internalBlock, ScoreNormalizerPtr scoreNormalizer, std::string name) :
					internalBlock(internalBlock), scoreNormalizer(scoreNormalizer), name(name) {
			}

			ScoreNormalizingProcessingBlock(typename IProcessingBlock<TBiometricSample>::Ptr internalBlock, ScoreNormalizerPtr scoreNormalizer) :
					internalBlock(internalBlock), scoreNormalizer(scoreNormalizer) {

				this->name.append("{");
				this->name.append(this->internalBlock->getName());
				this->name.append(" + ");
				this->name.append(this->scoreNormalizer->getName());
				this->name.append("}");
			}

			typename IProcessingBlock<TBiometricSample>::Ptr getInternalBlock() {
				return this->internalBlock;
			}

			void resetTemplates() {
				this->internalBlock->resetTemplates();
			}

			TemplateIndex pushTemplate(IPersistentTemplate::Ptr t) {
				return this->internalBlock->pushTemplate(t);
			}

			void setInputData(const TBiometricSample & inputData) {
				this->internalBlock->setInputData(inputData);
			}

			ComparisonScore computeComparisonScore(TemplateIndex templateIndex) {
				return this->scoreNormalizer->normalize(this->internalBlock->computeComparisonScore(templateIndex));
			}

			IPersistentTemplate::Ptr extract(const TBiometricSample & biometricSample) {
				return this->internalBlock->extract(biometricSample);
			}

			std::string getName() const {
				return this->name;
			}

			ScoreNormalizerPtr getScoreNormalizer() {
				return this->scoreNormalizer;
			}


			typedef typename ITrainingBlock<TBiometricSample>::Samples Samples;

			void train(const Samples & biometricSamples, const Labels & labels) {

				this->internalBlock->train(biometricSamples, labels);

				MethodResults r = evaluateBlock<TBiometricSample>(this->internalBlock, biometricSamples, labels);

				this->scoreNormalizer->train(r);
			}

		private:

			typename IProcessingBlock<TBiometricSample>::Ptr internalBlock;
			ScoreNormalizerPtr scoreNormalizer;
			std::string name;

	};

	template<class TBiometricSample>
	class IScoreNormalizingProcessingBlock: public ScoreNormalizingProcessingBlock<TBiometricSample, IScoreNormalizer> {
		public:
			IScoreNormalizingProcessingBlock(typename IProcessingBlock<TBiometricSample>::Ptr internalBlock, IScoreNormalizer::Ptr scoreNormalizer, std::string name) :
					ScoreNormalizingProcessingBlock<TBiometricSample, IScoreNormalizer>(internalBlock, scoreNormalizer, name) {
			}

			IScoreNormalizingProcessingBlock(typename IProcessingBlock<TBiometricSample>::Ptr internalBlock, IScoreNormalizer::Ptr scoreNormalizer) :
					ScoreNormalizingProcessingBlock<TBiometricSample, IScoreNormalizer>(internalBlock, scoreNormalizer) {
			}
	};

} /* namespace BioFW */
#endif /* SCORENORMALIZINGPROCESSINGBLOCK_H_ */
