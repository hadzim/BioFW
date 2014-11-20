/*
 * BiometricID.h
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#ifndef BIOMETRICID_H_
#define BIOMETRICID_H_

#include <string>
#include <vector>
#include <set>

namespace BioFW {

	class BiometricID {
		public:

			typedef std::vector <BiometricID> List;
			typedef std::set <BiometricID> Set;

			typedef std::string PersonID;
			typedef std::string BioProperty;

			BiometricID(const PersonID & personId = "<unknown>", const BioProperty & bioProperty = "<unknown>");

	        /// person ID (unique person identifier: John Doe)
			const PersonID & getPersonID() const;

			/// which biometric is stored (Fingerprint: LeftIndex, Left Iris, Face, ...)
			const BioProperty & getBiometricProperty() const;

			bool operator <(const BiometricID & bioID) const;
			bool operator ==(const BiometricID & bioID) const;
			bool operator !=(const BiometricID & bioID) const;

			std::string toString() const;
			static BiometricID create(std::string serialized);
		private:
			PersonID personID;
			BioProperty biometricProperty;
	};

} /* namespace BioFW */
#endif /* BIOMETRICID_H_ */

