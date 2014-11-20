/*
 * IDatabaseCreator.h
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#ifndef IDATABASECREATOR_H_
#define IDATABASECREATOR_H_
#include "BioFW/Database/Database.h"


namespace BioFW {
	template <class TRecord>
	class IDatabaseCreator {

		public:

			typedef Database<TRecord> Db;

			virtual ~IDatabaseCreator() {

			}
			virtual Database<TRecord> createDatabase() = 0;

	};

}

#endif /* IDATABASECREATOR_H_ */
