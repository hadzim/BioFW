/*
 * IBlockTraining.h
 *
 *  Created on: 10.3.2014
 *      Author: JV
 */

#ifndef IBLOCKTRAINING_H_
#define IBLOCKTRAINING_H_

#include "BioFW/Training.h"

namespace BioFW {

	template <class TRecord>
	class IBlockTraining : public IProgressReporter {
		public:
			typedef std::shared_ptr <IBlockTraining> Ptr;

			virtual ~IBlockTraining(){

			}
	        /// extract tempaltes
	        /// get list of database records and create templates from each of them
	        virtual void train(
	        		const typename TRecord::List & inputDatabase
	        ) = 0;
	    };



}  // namespace BioFW



#endif /* IBLOCKTRAINING_H_ */
