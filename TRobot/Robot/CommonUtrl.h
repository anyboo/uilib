#pragma once

#include <string>
#include <vector>
#include "AbstractVendor.h"

#define oneDay		(24 * 60 * 60)
#define oneHour		(60 * 60)
#define oneMinute	(60)

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
	vector<time_range> MakeTimeRangeList(const time_range& range);
	void InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd);
	time_t MakeTimestampByJTime(JTime jTime);
	string MakeStrTimeByTimestamp(time_t time);

private:
	CCommonUtrl();
	~CCommonUtrl();

	static CCommonUtrl* m_instance;
	
};

