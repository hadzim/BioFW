/*
 * DatabaseSubsetCreator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef DATABASESUBSETCREATOR_H_
#define DATABASESUBSETCREATOR_H_
#include "BioFW/Database/Subset/IDatabaseSubsetCreator.h"
#include <algorithm>
#include <vector>
#include <map>
#include "BioFW/Database/BiometricID.h"

namespace BioFW {

		struct SubsetRatio {
			public:
				typedef std::vector<SubsetRatio> List;

				std::string bin;
				double ratio;
		};

		enum SubsetSplitting {
			//RecordBased,
			BiometricIDBased
		};

		enum SubsetOrderMode {
			DbOrder, RandomOrder,
		};

		template<class TRecord>
		class DatabaseSubsetCreator: public IDatabaseSubsetCreator<TRecord> {
			public:

				DatabaseSubsetCreator(Database<TRecord> & fullDatabase) :
						fullDatabase(fullDatabase), splitting(BiometricIDBased), orderMode(DbOrder) {

				}
				virtual ~DatabaseSubsetCreator() {

				}

				Database<TRecord> getDatabaseSubset(std::string subsetName) {
					try {
						if (subsets.empty()) {
							this->createSubsets();
						}
						return subsets[subsetName];
					} catch (std::runtime_error & e) {
						throw BioFWException("Database subset " + subsetName + " is not available - " + e.what());
					}
					return Database<TRecord>();
				}

			private:
				BiometricID::List shuffleKeys() {
					BiometricID::List list;
					list.assign(fullDatabase.getCollections().getBiometricIDs().begin(), fullDatabase.getCollections().getBiometricIDs().end());
					if (orderMode == RandomOrder) {
						std::random_shuffle(list.begin(), list.end());
					}
					return list;
				}

				std::map<BiometricID, std::string> createKeys() {
					std::map<BiometricID, std::string> subsetKeys;
					BiometricID::List keys = this->shuffleKeys();

					double sum = 0;
					for (SubsetRatio::List::iterator sr = subsetRatios.begin(); sr != subsetRatios.end(); sr++) {
						sum += sr->ratio;
					}
					double bioIDCounter = 0;
					int actSubsetIndex = 0;
					double actSubsetCumulativeRatio = subsetRatios[actSubsetIndex].ratio;

					for (BiometricID::List::iterator bioID = keys.begin(); bioID != keys.end(); bioID++) {

						while ((bioIDCounter * sum / keys.size()) > actSubsetCumulativeRatio) {
							if (actSubsetIndex + 1 < subsetRatios.size()) {
								actSubsetIndex++;
								actSubsetCumulativeRatio += subsetRatios[actSubsetIndex].ratio;
							} else {
								break;
							}
						}
						subsetKeys.insert(std::make_pair(*bioID, subsetRatios[actSubsetIndex].bin));
						bioIDCounter++;
					}
					return subsetKeys;
				}

				void createSubsets() {
					std::map<BiometricID, std::string> subsetKeys = this->createKeys();

					for (typename TRecord::List::const_iterator r = this->fullDatabase.getCollections().getRecords().begin(); r != this->fullDatabase.getCollections().getRecords().end(); r++) {
						std::string key = subsetKeys[r->getBiometricID()];
						subsets[key].addRecord(*r);
					}
				}

			protected:

				Database<TRecord> & fullDatabase;

				SubsetSplitting splitting;
				SubsetOrderMode orderMode;

				SubsetRatio::List subsetRatios;
				std::map<std::string, Database<TRecord> > subsets;

		};


} /* namespace BioFW */
#endif /* DATABASESUBSETCREATOR_H_ */
