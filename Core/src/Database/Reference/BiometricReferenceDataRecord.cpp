#include "BioFW/Database/Reference/BiometricReferenceDataRecord.h"
/*
 * BiometricReferenceDataRecord.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Database/Reference/BiometricReferenceDataRecord.h"

namespace BioFW {

	BiometricReferenceDataRecord::BiometricReferenceDataRecord(const SampleID & sampleID, const BiometricID & bioID, IPersistentTemplate::Ptr t)
		: Record(sampleID, bioID), persistentTemplate(t){

	}

	IPersistentTemplate::Ptr BiometricReferenceDataRecord::getPersistentTemplate(){
		return this->persistentTemplate;
	}
	const IPersistentTemplate::Ptr BiometricReferenceDataRecord::getPersistentTemplate() const {
		return this->persistentTemplate;
	}

} /* namespace BioFW */
