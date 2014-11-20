/*
 * IComparisonBlock.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IPROCBLOCK_H_
#define IPROCBLOCK_H_
#include "BioFW/Block/IComparisonBlock.h"
#include "BioFW/Block/ITemplateExtractingBlock.h"
#include "BioFW/Progress.h"


namespace BioFW {
	template <class TBiometricSample>
    class IProcessingBlock : public IComparisonBlock<TBiometricSample>, public ITrainingBlock<TBiometricSample> {
    	public:
			typedef size_t TemplateIndex;

		typedef std::shared_ptr <IProcessingBlock> Ptr;

		virtual ~IProcessingBlock(){

		}

    };

	template <class TBiometricSample>
    class IProcessingBlockWrapper : public IProcessingBlock<TBiometricSample> {
    	public:
			typedef std::shared_ptr <IProcessingBlockWrapper> Ptr;


			virtual ~IProcessingBlockWrapper(){

			}

			virtual typename IProcessingBlock<TBiometricSample>::Ptr getInternalBlock() = 0;
    };

}  // namespace BioFW


#endif /* ICOMPARISONBLOCK_H_ */
