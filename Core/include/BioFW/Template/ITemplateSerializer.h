/*
 * ITemplateSerializer.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef ITEMPLATESERIALIZER_H_
#define ITEMPLATESERIALIZER_H_
#include <memory>

namespace BioFW {

	template <class TTemplate>
	class ITemplateSerializerSimple {
		public:
			typedef std::shared_ptr <ITemplateSerializerSimple> Ptr;
			typedef TTemplate Template;
			//typedef TPersistentTemplate PersistentTemplate;
			//typedef typename TPersistentTemplate::Ptr PersistentTemplatePtr;

			virtual ~ITemplateSerializerSimple(){}

			virtual void serialize(const Template & from, IPersistentTemplate::Ptr to) = 0;
			virtual Template deserialize(IPersistentTemplate::Ptr from) = 0;
	};

	template <class TTemplate, class TPersistentTemplate>
	class ITemplateSerializer {
		public:
			typedef std::shared_ptr <ITemplateSerializer> Ptr;
			typedef TTemplate Template;
			typedef TPersistentTemplate PersistentTemplate;
			typedef typename TPersistentTemplate::Ptr PersistentTemplatePtr;

			virtual ~ITemplateSerializer(){}

			virtual PersistentTemplatePtr serialize(const Template & from) = 0;
			virtual Template deserialize(PersistentTemplatePtr from) = 0;
	};

} /* namespace BioFW */
#endif /* ITemplateSerializer_H_ */
