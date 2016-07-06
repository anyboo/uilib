#pragma once

// System
#include <Windows.h>
#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <tchar.h>
#include <map>
#include <assert.h> 
#include <ctime>
#include "io.h"
#include "direct.h"

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

#define Test_Bug
#define Test_Filename

#define Vendor_JXJ	"º—–≈Ω›"
#define Vendor_DZP	"µœ÷«∆’"

#define MAX_SEARCH_COUNT 1000
#define ONE_DAY		 (24 * 60 * 60)
#define ONE_HOUR	 (60 * 60)
#define ONE_MINUTE	 (60)

typedef enum
{
	Err_No = 0,
	Err_LoginSuccess, // Login Success
	Err_LoginFail,	// Login Fail
	Err_DownloadSuccess, // Download Success
}eErrCode;


#include "j_sdk.h"

class CCommonUtrl
{
public:
	CCommonUtrl();
	~CCommonUtrl();

	static CCommonUtrl& getInstance();

	// File Name & File Folder
	std::string MakeDownloadFileFolder(const std::string basePath, const std::string& startTimeZero, const std::string& endTimeZero, const std::string& venderName, int channel);
	void MakeFolder(std::string fileName);

	// Video Time Range(JTime - JxjVendor)
	std::vector<time_range> MakeTimeRangeList(const time_range& range);
	void InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd);
	time_t MakeTimestampByJTime(JTime jTime);
	std::string MakeStrTimeByTimestamp(time_t time);
	
	// Json File Handle
	void SaveSearchFileListToFile(const std::vector<RecordFile>& files);
	std::vector<RecordFile> LoadSearchFileListFromFile();

};


