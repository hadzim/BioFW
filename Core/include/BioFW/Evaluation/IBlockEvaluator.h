/*
 * IBlockEvaluator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IBLOCKEVALUATOR_H_
#define IBLOCKEVALUATOR_H_
#include "BioFW/Progress.h"
#include <memory>
#include "BioFW/Database/Reference/BiometricReferenceDataRecord.h"
#include "BioFW/Evaluation/Results.h"


namespace BioFW {

	template <class TRecord>
	class IBlockEvaluator : public IProgressReporter {
		public:
			typedef std::shared_ptr <IBlockEvaluator> Ptr;

			virtual ~IBlockEvaluator(){

			}
	        /// extract tempaltes
	        /// get list of database records and create templates from each of them
	        virtual void extractTemplates(
	        		const typename TRecord::List & inputDatabase,
	        		BiometricReferenceDatabase & referenceDatabase
	        ) = 0;

	        /// evaluate records - compare each template against each input and save results
	        virtual void evaluateRecords(
	        		const typename TRecord::List & inputDatabase,
	        		const BiometricReferenceDatabase & referenceDatabase,
	        		Results & results
	        ) = 0;

	    };



}  // namespace BioFW

#endif /* IBLOCKEVALUATOR_H_ */
