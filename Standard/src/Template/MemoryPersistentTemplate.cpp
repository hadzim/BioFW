/*
 * MemoryPersistentTemplate.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Template/MemoryPersistentTemplate.h"
#include "BioFW/Exception.h"
#include <iostream>

namespace BioFW {

		MemoryPersistentTemplate::MemoryPersistentTemplate(/*std::string name*/) /*: name(name)*/ {

		}

		MemoryPersistentTemplate::~MemoryPersistentTemplate() {

		}

	/*	std::string MemoryPersistentTemplate::getName() const {
			return this->name;
		}
*/
		IPersistentTemplate::Ptr MemoryPersistentTemplate::getSubtemplate(const Name & name) const {
			try {
				return this->subtemplates.at(name);
			} catch (...){

			}

			for (auto n : this->subtemplates){
				std::cout << "subtemplate: " << n.first << std::endl;
			}

			throw BioFWException("Non existing subtemplate " + name);
		}
		void MemoryPersistentTemplate::addSubtemplate(const Name & name, IPersistentTemplate::Ptr templ) {

			this->subtemplates[name] = templ;
		}


} /* namespace BioFW */
