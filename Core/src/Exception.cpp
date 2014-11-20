#include "BioFW/Exception.h"
#include <typeinfo>
namespace BioFW {

	POCO_IMPLEMENT_EXCEPTION(BioFWException, Poco::Exception, "BioFrameworkException")

}  // namespace BioFW
