/*
 * IRecord.h
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#ifndef IRECORD_H_
#define IRECORD_H_
#include <string>
#include "BioFW/Database/BiometricID.h"
#include <memory>

namespace BioFW {

	typedef std::string SampleID;

	class IRecord {
		public:
			//typedef std::shared_ptr <IRecord> Ptr;
			typedef std::vector <IRecord> List;

			//typedef std::string SampleID;

			virtual ~IRecord();

			virtual const SampleID & getSampleID() const = 0;
			virtual const BiometricID & getBiometricID() const = 0;
	};

} /* namespace BioFW */
#endif /* IRECORD_H_ */
