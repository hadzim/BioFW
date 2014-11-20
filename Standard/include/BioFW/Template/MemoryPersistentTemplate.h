/*
 * MemoryPersistentTemplate.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef MEMORYPERSISTENTTEMPLATE_H_
#define MEMORYPERSISTENTTEMPLATE_H_

#include "BioFW/Template/IPersistentTemplate.h"
#include <string>
#include <map>

namespace BioFW {

		class MemoryPersistentTemplate : public IPersistentTemplate {
			public:
				MemoryPersistentTemplate();
				virtual ~MemoryPersistentTemplate();

				//virtual std::string getName() const;

				virtual IPersistentTemplate::Ptr getSubtemplate(const Name & name) const ;
				virtual void addSubtemplate(const Name & name, IPersistentTemplate::Ptr templ);

			private:
				//std::string name;
				std::map <std::string, IPersistentTemplate::Ptr> subtemplates;

		};


} /* namespace BioFW */
#endif /* MEMORYPERSISTENTTEMPLATE_H_ */
