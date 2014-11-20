/*
 * RecordDatabaseSubsetCreator.h
 *
 *  Created on: 28.1.2014
 *      Author: JV
 */

#ifndef RECORDDATABASESUBSETCREATOR_H_
#define RECORDDATABASESUBSETCREATOR_H_
#include <set>
#include <string>
#include "BioFW/Database/Subset/IDatabaseSubsetCreator.h"

namespace BioFW {

		template<class TRecord>
		class RecordDatabaseSubsetCreator: public IDatabaseSubsetCreator<TRecord> {
			public:
				typedef Database<TRecord> Db;
				typedef std::map<std::string, Db> Subsets;

				RecordDatabaseSubsetCreator(Db & fullDb) :
						fullDatabase(fullDb) {

				}
				virtual ~RecordDatabaseSubsetCreator() {
				}

				Db getDatabaseSubset(std::string subsetName) {
					try {
						if (subsets.empty()) {
							subsets = this->createSubsets();
						}
						return subsets[subsetName];
					} catch (std::exception & e) {
						throw BioFWException("Database subset " + subsetName + " is not available:" + e.what());
					}
					return Db();
				}
			protected:
				virtual Subsets createSubsets() = 0;
			protected:
				Db & fullDatabase;
			private:
				Subsets subsets;

		};

}  // namespace BioFW

#endif /* RECORDDATABASESUBSETCREATOR_H_ */
