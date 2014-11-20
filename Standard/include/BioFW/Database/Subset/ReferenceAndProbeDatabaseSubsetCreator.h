/*
 * ReferenceAndProbeDatabaseSubsetCreator.h
 *
 *  Created on: 28.1.2014
 *      Author: JV
 */

#ifndef REFERENCEANDPROBEDATABASESUBSETCREATOR_H_
#define REFERENCEANDPROBEDATABASESUBSETCREATOR_H_
#include "BioFW/Database/Subset/RecordDatabaseSubsetCreator.h"
#include "BioFW/Database/Database.h"

namespace BioFW {

		template<class TRecord>
		class ReferenceAndProbeDatabaseSubsetCreator: public RecordDatabaseSubsetCreator<TRecord> {

			public:

				typedef typename RecordDatabaseSubsetCreator<TRecord>::Db Db;
				typedef typename RecordDatabaseSubsetCreator<TRecord>::Subsets Subsets;

				ReferenceAndProbeDatabaseSubsetCreator(Database<TRecord> & fullDatabase, int refTemplateCount) :
						RecordDatabaseSubsetCreator<TRecord>(fullDatabase), refTemplateCount(refTemplateCount) {
				}
				virtual ~ReferenceAndProbeDatabaseSubsetCreator() {

				}

				static std::string ReferenceSubsetName() {
					return "reference";
				}
				static std::string ProbeSubsetName() {
					return "probe";
				}

				Database<TRecord> getReferenceSubset() {
					return this->getDatabaseSubset(ReferenceSubsetName());
				}
				Database<TRecord> getProbeSubset() {
					return this->getDatabaseSubset(ProbeSubsetName());
				}



			protected:
				typedef typename TRecord::List Records;
				virtual Subsets createSubsets() {

					Subsets db;

					for (BiometricID::Set::const_iterator bid = this->fullDatabase.getCollections().getBiometricIDs().begin(); bid != this->fullDatabase.getCollections().getBiometricIDs().end(); bid++) {
						int act = 0;
						const Records & records = this->fullDatabase.getCollections().getRecordsByBiometricID(*bid);
						//int size = records.size();

						for (typename Records::const_iterator r = records.begin(); r != records.end(); r++) {
							if (act < refTemplateCount) {
								db[ReferenceSubsetName()].addRecord(*r);
							} else {
								db[ProbeSubsetName()].addRecord(*r);
							}
							act++;
						}
					}

					return db;
				}

			private:
				int refTemplateCount;

		};


} /* namespace BioFW */
#endif /* REFERENCEANDPROBEDATABASESUBSETCREATOR_H_ */
