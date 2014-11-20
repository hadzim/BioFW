/*
 * ITemplateExtractingBlock.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef ITEMPLATEEXTRACTINGBLOCK_H_
#define ITEMPLATEEXTRACTINGBLOCK_H_

#include "BioFW/Block/IBlock.h"
#include "BioFW/Template/IPersistentTemplate.h"
#include "BioFW/Training.h"

namespace BioFW {
	template <class TBiometricSample>
	class ITemplateExtractingBlock : public IBlock {
		public:
			virtual ~ITemplateExtractingBlock(){

			}
			virtual IPersistentTemplate::Ptr extract(const TBiometricSample & biometricSample) = 0;

	};

	template <class TBiometricSample, class TTemplate>
	class IMemoryTemplateExtractingBlock : public IBlock {
		public:
			typedef TTemplate Template;
			typedef TBiometricSample Sample;

			virtual ~IMemoryTemplateExtractingBlock(){

			}
			virtual Template extract(const Sample & biometricSample) = 0;

	};

	template <class TBiometricSample>
	class ITrainingBlock {
		public:

			typedef std::shared_ptr <ITrainingBlock> Ptr;
			typedef TBiometricSample Sample;
			typedef std::vector<TBiometricSample> Samples;

			virtual ~ITrainingBlock(){

			}

			virtual void train(const Samples & biometricSamples, const Labels & labels) = 0;

	};

} /* namespace BioFW */
#endif /* ITEMPLATEEXTRACTINGBLOCK_H_ */
