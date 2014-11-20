/*
 * RecordSelection.h
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#ifndef RECORDSELECTION_H_
#define RECORDSELECTION_H_

#include <map>
#include "BioFW/Exception.h"
#include "BioFW/Database/BiometricID.h"
#include "BioFW/Database/IRecord.h"

namespace BioFW {

	template <class TRecord>
    class RecordSelection {
    	public:

			typedef typename TRecord::List Records;

			const Records & getRecordsByBiometricID(const BiometricID & biometricID) const {
				return this->recordsByBiometricID.at(biometricID);
			}

			const Records & getRecords() const {
				return this->records;
			}

			const BiometricID::Set & getBiometricIDs() const {
				return this->bioIDs;
			}

			void addRecord(const TRecord & r) {
				this->records.push_back(r);
				this->recordsByBiometricID[r.getBiometricID()].push_back(r);
				this->bioIDs.insert(r.getBiometricID());
			}

			 TRecord & getRecord(const BiometricID & biometricID, const SampleID & sampleID) const {
				for (typename Records::iterator i = recordsByBiometricID[biometricID].begin(); i != recordsByBiometricID[biometricID].end(); i++){

					if ((*i).getSampleID() == sampleID) {
						return *i;
					}
				}
				throw BioFWException("Record not exists");
			}
    	private:
			std::map <BiometricID, Records> recordsByBiometricID;
			Records records;
			BiometricID::Set bioIDs;

    };
}


#endif /* RECORDSELECTION_H_ */
