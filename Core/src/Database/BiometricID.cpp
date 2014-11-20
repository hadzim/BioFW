/*
 * BiometricID.cpp
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */


#include <Poco/StringTokenizer.h>
#include "BioFW/Database/BiometricID.h"

namespace BioFW {

	BiometricID::BiometricID(const PersonID & personId, const BioProperty & bioProperty) :
			personID(personId), biometricProperty(bioProperty) {

	}

	/// person ID (unique person identifier: John Doe)
	const BiometricID::PersonID & BiometricID::getPersonID() const {
		return this->personID;
	}

	/// which biometric is stored (Fingerprint: LeftIndex, Left Iris, Face, ...)
	const BiometricID::BioProperty & BiometricID::getBiometricProperty() const {
		return this->biometricProperty;
	}

	bool BiometricID::operator <(const BiometricID & bioID) const {
		if (this->personID == bioID.personID){
			return this->biometricProperty < bioID.biometricProperty;
		}
		return this->personID < bioID.personID;
	}
	bool BiometricID::operator ==(const BiometricID & bioID) const {
		return this->personID == bioID.personID && this->biometricProperty == bioID.biometricProperty;
	}
	bool BiometricID::operator !=(const BiometricID & bioID) const {
		return !(*this == bioID);
	}

	std::string BiometricID::toString() const {
		return personID + "::" + biometricProperty;
	}
	BiometricID BiometricID::create(std::string serialized) {
		Poco::StringTokenizer t(serialized, "::");
		return BiometricID(t[0], t[1]);
	}

} /* namespace BioFW */
