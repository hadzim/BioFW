/*
 * TrainAndTestDatabaseSubsetCreator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef TRAINANDTESTDATABASESUBSETCREATOR_H_
#define TRAINANDTESTDATABASESUBSETCREATOR_H_
#include "BioFW/Database/Subset/DatabaseSubsetCreator.h"

namespace BioFW {

		template <class TRecord>
		class TrainAndTestDatabaseSubsetCreator : public DatabaseSubsetCreator<TRecord> {
			public:
				TrainAndTestDatabaseSubsetCreator(Database<TRecord> & fullDatabase,  SubsetOrderMode orderMode = DbOrder, int trainProportion = 1, int testProportion = 1) : DatabaseSubsetCreator<TRecord>(fullDatabase){
					this->orderMode = orderMode;
					{
						SubsetRatio r;
						r.bin = TrainSubsetName();
						r.ratio = trainProportion;
						this->subsetRatios.push_back(r);
					}
					{
						SubsetRatio r;
						r.bin = TestSubsetName();
						r.ratio = testProportion;
						this->subsetRatios.push_back(r);
					}
				}
				virtual ~TrainAndTestDatabaseSubsetCreator(){

				}

				static std::string TrainSubsetName(){
					return "train";
				}
				static std::string TestSubsetName(){
					return "test";
				}

				Database<TRecord> getTrainSubset() {
					return this->getDatabaseSubset(TrainSubsetName());
				}
				Database<TRecord> getTestSubset() {
					return this->getDatabaseSubset(TestSubsetName());
				}
		};


} /* namespace BioFW */
#endif /* TRAINANDTESTDATABASESUBSETCREATOR_H_ */
