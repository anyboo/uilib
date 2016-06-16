#ifndef Log_INCLUDED
#define Log_INCLUDED
#include "Poco/Logger.h"
using  Poco::Logger;                    // for global decorator 

namespace Loggering {
#define logger_handle (Logger::get("logger"))

	void Logger_initiation();  // initiation
	void Setup_logger();       // init only once 

}

#endif