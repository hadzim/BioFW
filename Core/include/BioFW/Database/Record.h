/*
 * Record.h
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <string>
#include "BioFW/Database/BiometricID.h"
#include "BioFW/Database/IRecord.h"

namespace BioFW {

	class Record : public IRecord {
		public:

			//typedef std::shared_ptr <Record> Ptr;
			typedef std::vector <Record> List;


			Record(const SampleID & sampleID, const BiometricID & bioID);

			virtual const SampleID & getSampleID() const;
			virtual const BiometricID & getBiometricID() const;

			bool operator <(const Record & r) const;
			bool operator ==(const Record & r) const;
			bool operator !=(const Record & r) const;
		private:
			SampleID sampleID;
			BiometricID bioID;
	};

} /* namespace BioFW */
#endif /* IRECORD_H_ */
