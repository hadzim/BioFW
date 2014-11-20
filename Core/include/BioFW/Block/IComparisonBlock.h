/*
 * IComparisonBlock.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef ICOMPARISONBLOCK_H_
#define ICOMPARISONBLOCK_H_
#include "BioFW/Block/ITemplateExtractingBlock.h"
#include "BioFW/Progress.h"


namespace BioFW {
	template <class TBiometricSample>
    class IComparisonBlock : public ITemplateExtractingBlock<TBiometricSample>, public IProgressReporter {
    	public:
		typedef std::shared_ptr <IComparisonBlock> Ptr;
		typedef size_t TemplateIndex;

		virtual ~IComparisonBlock(){

		}

		/// clear all pushed templates -> all indexes are since then invalid
		virtual void resetTemplates() = 0;

        /// push template to actual buffer -> returns template index that is then used for matching
        virtual TemplateIndex pushTemplate(IPersistentTemplate::Ptr t) = 0;

        /// set actual input data
        virtual void setInputData(const TBiometricSample & inputData) = 0;

        /// compute matching score of actual input and template in buffer with given index
        virtual ComparisonScore computeComparisonScore(TemplateIndex templateIndex) = 0;
    };


}  // namespace BioFW

namespace BioFW {
	template <class TBiometricSample, class TTemplate>
    class IMemoryComparisonBlock : public IMemoryTemplateExtractingBlock<TBiometricSample, TTemplate>, public IProgressReporter {
    	public:

		typedef TTemplate Template;
		typedef TBiometricSample Sample;

		typedef size_t TemplateIndex;

		virtual ~IMemoryComparisonBlock(){

		}

		/// clear all pushed templates -> all indexes are since then invalid
		virtual void resetTemplates() = 0;

        /// push template to actual buffer -> returns template index that is then used for matching
        virtual TemplateIndex pushTemplate(Template t) = 0;

        /// set actual input data
        virtual void setInputData(const Sample & inputData) = 0;

        /// compute matching score of actual input and template in buffer with given index
        virtual ComparisonScore computeComparisonScore(TemplateIndex templateIndex) = 0;
    };


}  // namespace BioFW


#endif /* ICOMPARISONBLOCK_H_ */
