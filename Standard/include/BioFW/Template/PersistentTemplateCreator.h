/*
 * PersistentTemplateCreator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef PERSISTENTTEMPLATECREATOR_H_
#define PERSISTENTTEMPLATECREATOR_H_

namespace BioFW {

		template <class TPersistentTemplate>
		class PersistentTemplateCreator : IPersistentTemplateCreator {
			public:
				virtual ~PersistentTemplateCreator(){}

				typename TPersistentTemplate::Ptr createPersistentTemplate(std::string name, BiometricID recordId) {
					typename TPersistentTemplate::Ptr t = new TPersistentTemplate(name);
					return t;
				}
				typename TPersistentTemplate::Ptr createPersistentTemplate(std::string name, IPersistentTemplate::Ptr parent) {
					TPersistentTemplate t = new TPersistentTemplate(name, parent);
					return t;
				}
		};

} /* namespace BioFW */
#endif /* PERSISTENTTEMPLATECREATOR_H_ */
