#include "BioFW/Database/Record.h"
/*
 * Record.cpp
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#include "BioFW/Database/Record.h"

namespace BioFW {

	Record::Record(const SampleID & sampleID, const BiometricID & bioID) :
			sampleID(sampleID), bioID(bioID) {

	}

	const SampleID & Record::getSampleID() const {
		return this->sampleID;
	}

	const BiometricID & Record::getBiometricID() const {
		return this->bioID;
	}

	bool Record::operator <(const Record & r) const {
		if (this->sampleID == r.sampleID){
			return this->bioID < r.bioID;
		}
		return this->sampleID < r.sampleID;
	}

	bool Record::operator ==(const Record & r) const {
		return this->sampleID == r.sampleID && this->bioID == r.bioID;
	}

	bool Record::operator !=(const Record & r) const {
		return !(*this == r);
	}

} /* namespace BioFW */
