/*
 * IDatabaseSubsetCreator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IDATABASESUBSETCREATOR_H_
#define IDATABASESUBSETCREATOR_H_
#include <memory>
#include "BioFW/Database/Database.h"

namespace BioFW {

	template<class TRecord>
	class IDatabaseSubsetCreator {
		public:
			typedef std::shared_ptr <IDatabaseSubsetCreator> Ptr;
			virtual ~IDatabaseSubsetCreator(){

			}

			virtual Database<TRecord> getDatabaseSubset(std::string subsetName) = 0;
	};
}

#endif /* IDATABASESUBSETCREATOR_H_ */
