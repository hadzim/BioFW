/*
 * IPersistentTemplateCreator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IPersistentTemplateCREATOR_H_
#define IPersistentTemplateCREATOR_H_
#include <string>
#include "BioFW/Database/BiometricID.h"
#include <memory>
#include "BioFW/Template/IPersistentTemplate.h"

namespace BioFW {

	class IPersistentTemplateCreator {
		public:
			typedef std::shared_ptr <IPersistentTemplateCreator> Ptr;

			virtual ~IPersistentTemplateCreator();

			/// create persistent template with given name
			virtual IPersistentTemplate::Ptr createPersistentTemplate(std::string name, BiometricID recordId) = 0;

			/// create persistent tempalte as subtemplate of parent
			virtual IPersistentTemplate::Ptr createPersistentTemplate(std::string name, IPersistentTemplate::Ptr parent) = 0;
	};

} /* namespace BioFW */
#endif /* IPersistentTemplateCREATOR_H_ */
