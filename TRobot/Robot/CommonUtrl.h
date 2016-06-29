#pragma once

#include <Windows.h>

// Jxj Sdk
#include "inc\mb_api.h"
#include "inc\JNetSDK.h"
#include "inc\stdint.h"
#include "inc\Jtype.h"
#include "inc\AVPlayer.h"
#include "JxjVendor.h"

#include "AbstractVendor.h"
#include <string>
#include <vector>

class CCommonUtrl
{
public:
	static CCommonUtrl* getInstance();

	// File Name & File Folder
	std::string MakeFileName(int channel, const std::string& startTime, const std::string& endTime);
	std::string MakeDownloadFileFolder(const std::string basePath,
		const std::string& startTimeZero, const std::string& endTimeZero,
		const std::string& venderName, int channel, const std::string& fileName, const std::string& fileType);

	// Video Time Range(JTime - JxjVendor)
	std::vector<time_range> MakeTimeRangeList(const time_range& range);
	void InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd);
	time_t MakeTimestampByJTime(JTime jTime);
	std::string MakeStrTimeByTimestamp(time_t time);

private:
	CCommonUtrl();
	~CCommonUtrl();

	static CCommonUtrl* m_instance;
	
};

