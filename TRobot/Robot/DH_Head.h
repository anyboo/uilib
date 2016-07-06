#ifndef __DH_HEAD_H__
#define __DH_HEAD_H__


#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h> 


// Json
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ostreamwrapper.h"
#include "istreamwrapper.h"

// Self
#include "AbstractVendor.h"
#include "QMSqlite.h"
#include "TestWindows.h"

using namespace rapidjson;

//DH SDK
#include "dhnetsdk.h"


#define MAX_SEARCH_COUNT 1000
#define ONE_DAY		 (24 * 60 * 60)
#define ONE_HOUR	 (60 * 60)
#define ONE_MINUTE	 (60)

#endif 