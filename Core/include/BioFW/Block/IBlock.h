/*
 * IBlock.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IBLOCK_H_
#define IBLOCK_H_

namespace BioFW {

	class IBlock {
		public:
			typedef std::string Name;

			virtual ~IBlock(){

			}

			virtual Name getName() const = 0;
	};


}  // namespace BioFW


#endif /* IBLOCK_H_ */
