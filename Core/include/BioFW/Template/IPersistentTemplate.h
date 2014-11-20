/*
 * IIPersistentTemplate.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IIPersistentTemplate_H_
#define IIPersistentTemplate_H_
#include <memory>
#include <string>

namespace BioFW {

	class IPersistentTemplate {
		public:
			typedef std::shared_ptr <IPersistentTemplate> Ptr;

			virtual ~IPersistentTemplate();

			typedef std::string Name;
			//virtual std::string getName() const = 0;

			/// <summary>
			/// get subtemplate
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			virtual IPersistentTemplate::Ptr getSubtemplate(const Name & name) const = 0;
			/// <summary>
			/// create subtemplate and returns it
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			virtual void addSubtemplate(const Name & name, IPersistentTemplate::Ptr templ) = 0;
	};

} /* namespace BioFW */
#endif /* IIPersistentTemplate_H_ */
