/*
 * BiometricReferenceDataRecord.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef BIOMETRICREFERENCEDATARECORD_H_
#define BIOMETRICREFERENCEDATARECORD_H_
#include "BioFW/Database/Record.h"
#include "BioFW/Template/IPersistentTemplate.h"
#include "BioFW/Database/Database.h"

namespace BioFW {

	class BiometricReferenceDataRecord : public Record {
		public:
			typedef std::vector <BiometricReferenceDataRecord> List;

			BiometricReferenceDataRecord(const SampleID & sampleID, const BiometricID & bioID, IPersistentTemplate::Ptr t);

			IPersistentTemplate::Ptr getPersistentTemplate();
			const IPersistentTemplate::Ptr getPersistentTemplate() const;
		private:
			IPersistentTemplate::Ptr persistentTemplate;
	};

	typedef Database<BiometricReferenceDataRecord> BiometricReferenceDatabase;


} /* namespace BioFW */
#endif /* BIOMETRICREFERENCEDATARECORD_H_ */
