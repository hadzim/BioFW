/*
 * Exception.h
 *
 *  Created on: 23.1.2014
 *      Author: JV
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "Core.h"
#include <Poco/Exception.h>
#define Foundation


namespace BioFW {

	POCO_DECLARE_EXCEPTION(Core_API, BioFWException, Poco::Exception)

}  // namespace BioFW

#endif /* EXCEPTION_H_ */
