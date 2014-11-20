/*
 * DatabaseOverview.h
 *
 *  Created on: 28.1.2014
 *      Author: JV
 */

#ifndef DATABASEOVERVIEW_H_
#define DATABASEOVERVIEW_H_
#include <iosfwd>
#include "BioFW/Database/Database.h"

namespace BioFW {
	namespace Utils {

		template <class TRecord>
		void dumpDatabaseOverview(std::ostream & o, const Database<TRecord> & db){
				o << "Db size: " << db.getCollections().getRecords().size() << std::endl;
				o << "Unique biometrics count: " << db.getCollections().getBiometricIDs().size() << std::endl;
		}

	} /* namespace Utils */
} /* namespace BioFW */
#endif /* DATABASEOVERVIEW_H_ */
