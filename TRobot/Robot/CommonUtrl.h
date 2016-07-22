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

// Self
#include "AbstractVendor.h"
#include "QMSqlite.h"
#include "QMFileSqlite.h"
#include "TestWindows.h"

// Json
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ostreamwrapper.h"
#include "istreamwrapper.h"

// Exception
#include "LoginException.h"
#include "SearchFileException.h"


#define Test_Bug
#define Test_Filename

#define Vendor_DH	"大华"
#define Vendor_JXJ	"佳信捷"
#define Vendor_DZP	"迪智浦"
#define Vendor_HK	"海康"

#define Vendor_DH_Abbr		"DH"
#define Vendor_JXJ_Abbr		"JXJ"
#define Vendor_DZP_Abbr		"DZP"
#define Vendor_HK_Abbr		"HK"

#define MAX_SEARCH_COUNT 1000
#define ONE_DAY		 (24 * 60 * 60)
#define ONE_HOUR	 (60 * 60)
#define ONE_MINUTE	 (60)

typedef struct __ComTime
{
	uint8_t year;					//从1900开始, J_SDK_DEF_BASE_YEAR
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t weekday;				//详情见JWeekDay
}ComTime;

class CCommonUtrl
{
public:
	CCommonUtrl();
	~CCommonUtrl();

	static CCommonUtrl& getInstance();

	// File Name & File Folder
	std::string MakeFileName(int channel, const std::string& startTime, const std::string& endTime, const std::string& extensions);
	std::string MakeDownloadFileFolder(const std::string basePath, const std::string& startTimeZero, const std::string& endTimeZero, const std::string& venderName, int channel);
	void MakeFolder(std::string fileName);

	// Video Time Range(JTime - JxjVendor)
	std::vector<time_range> MakeTimeRangeList(const time_range& range);
	std::string MakeStrTimeByTimestamp(time_t time);
	
	// Json File Handle
	void SaveSearchFileListToFile(const std::vector<RecordFile>& files, const std::string& VenderName);
	std::vector<RecordFile> LoadSearchFileListFromFile();

	// DB Operation
	void WriteFileListToDB(RECORD_FILE_LIST& recordFiles);

	// Search File to RecordFile
	RecordFile MakeDBFileToRecordFile(const readSearchVideo& rsv);

	std::string GetCurTime();

};

