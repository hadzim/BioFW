/*
 * Database.h
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#include "BioFW/Database/RecordSelection.h"


namespace BioFW {

	template <class TRecord>
	class Database {

		public:
	        const RecordSelection<TRecord> & getCollections() const{
	            return this->storage;
	        }

	        void addRecord(const TRecord & r) {
	            this->storage.addRecord(r);
			}

	        private:
	        	RecordSelection<TRecord> storage;
	};

}  // namespace BioFW

#endif /* DATABASE_H_ */
