

#include "CommonUtrl.h"
#include <Poco/SingletonHolder.h>

#define oneDay		(24 * 60 * 60)
#define oneHour		(60 * 60)
#define oneMinute	(60)

using namespace rapidjson;

CCommonUtrl::CCommonUtrl()
{
}

CCommonUtrl::~CCommonUtrl()
{
}

CCommonUtrl& CCommonUtrl::getInstance()
{
	static Poco::SingletonHolder<CCommonUtrl> sh;
	return *sh.get();
}

std::string CCommonUtrl::MakeFileName(int channel, const std::string& startTime, const std::string& endTime)
{
	std::string strFileName;

	strFileName += "channel";
	if (channel < 10)
	{
		strFileName += "0";
	}
	strFileName += std::to_string(channel);
	strFileName += "-";
	strFileName += startTime.data();
	strFileName += "-";
	strFileName += endTime.data();

	return strFileName;
}

std::string CCommonUtrl::MakeDownloadFileFolder(const std::string basePath,
	const std::string& startTimeZero, const std::string& endTimeZero,
	const std::string& venderName, int channel, const std::string& fileName, const std::string& fileType)
{
	std::string strPath = basePath;
	strPath.append("\\");
	strPath += startTimeZero.data();
	strPath.append("-");
	strPath += endTimeZero.data();
	strPath.append("\\");
	//CreateDirectory(strPath.c_str(), NULL);
	strPath += venderName.data();
	strPath.append("\\");
	//CreateDirectory(strPath.c_str(), NULL);
	strPath.append("Í¨µÀ");
	if (channel < 10)
	{
		strPath += "0";
	}
	strPath += std::to_string(channel);
	strPath.append("\\");
	//CreateDirectory(strPath.c_str(), NULL);
	strPath.append(fileName);
	strPath += fileType.data();

	return strPath;
}

std::vector<time_range> CCommonUtrl::MakeTimeRangeList(const time_range& range)
{
	time_t timeStart = range.start;
	time_t timeEnd = range.end;
	std::vector<time_range> timeRangeList;

	JTime jStartTime, jStopTime;
	InitSearchTime(jStartTime, jStopTime, timeStart, timeEnd);

	if (timeEnd - timeStart <= oneDay)
	{
		if (jStartTime.date == jStopTime.date)
		{
			timeRangeList.push_back(range);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - jStartTime.hour) * oneHour + (59 - jStartTime.minute) * oneMinute + (59 - jStartTime.second);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			rangeItem.start = timeStart + diff + 1;
			rangeItem.end = timeEnd;
			timeRangeList.push_back(rangeItem);
		}
	}
	else
	{
		time_t diff = timeEnd - timeStart;
		int day = (diff / oneDay) + (diff % oneDay > 0 ? 1 : 0);

		if (jStartTime.hour == 0 && jStartTime.minute == 0 && jStartTime.second == 0)
		{
			for (size_t i = 0; i < day - 1; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + oneDay - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + oneDay;
			}

			time_range rangeItem;
			rangeItem.start = timeStart;
			rangeItem.end = timeEnd;
			timeRangeList.push_back(rangeItem);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - jStartTime.hour) * oneHour + (59 - jStartTime.minute) * oneMinute + (59 - jStartTime.second);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			timeStart = timeStart + diff + 1;
			for (size_t i = 0; i < day - 2; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + oneDay - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + oneDay;
			}

			if (timeEnd > timeStart + oneDay - 1)
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + oneDay - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + oneDay;
				rangeItem.start = timeStart;
				rangeItem.end = timeEnd;
				timeRangeList.push_back(rangeItem);
			}
			else
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeEnd;
				timeRangeList.push_back(rangeItem);
			}
		}
	}

	return timeRangeList;
}

void CCommonUtrl::InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd)
{
	struct tm Tm;

	localtime_s(&Tm, (const time_t*)&timeStart);
	jStartTime.year = Tm.tm_year;
	jStartTime.month = Tm.tm_mon;
	jStartTime.date = Tm.tm_mday;
	jStartTime.hour = Tm.tm_hour;
	jStartTime.minute = Tm.tm_min;
	jStartTime.second = Tm.tm_sec;
	jStartTime.weekday = Tm.tm_wday;

	localtime_s(&Tm, (const time_t*)&timeEnd);
	jStopTime.year = Tm.tm_year;
	jStopTime.month = Tm.tm_mon;
	jStopTime.date = Tm.tm_mday;
	jStopTime.hour = Tm.tm_hour;
	jStopTime.minute = Tm.tm_min;
	jStopTime.second = Tm.tm_sec;
	jStopTime.weekday = Tm.tm_wday;
}

time_t CCommonUtrl::MakeTimestampByJTime(JTime jTime)
{
	struct tm ttime;
	ttime.tm_year = jTime.year;
	ttime.tm_mon = jTime.month - 1;
	ttime.tm_mday = jTime.date;
	ttime.tm_hour = jTime.hour;
	ttime.tm_min = jTime.minute;
	ttime.tm_sec = jTime.second;
	time_t time = mktime(&ttime);

	return time;
}

std::string CCommonUtrl::MakeStrTimeByTimestamp(std::time_t time)
{
	std::string strTime;
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime((char *)strTime.c_str(), 50, "%Y%m%d%H%M%S", &ttime);

	return strTime;
}

void CCommonUtrl::SaveSearchFileListToFile(const std::vector<Record>& files)
{
	Document document;
	std::string configfile = "SearchFileList.config";
	document.Parse(configfile.c_str());
	std::ofstream ofs(configfile);
	OStreamWrapper osw(ofs);
	Document::AllocatorType& alloc = document.GetAllocator();

	Value root(kObjectType);

	for (size_t i = 0; i < files.size(); i++)
	{
		std::string fileKey = "videoFile";
		Value key(fileKey.c_str(), fileKey.length(), alloc);

		Record file = files[i];
		Value name(file.name.c_str(), file.name.length(), alloc);
		Value channel(std::to_string(file.channel).c_str(), std::to_string(file.channel).length(), alloc);
		Value beginTime(CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.beginTime).c_str(), CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.beginTime).length(), alloc);
		Value endTime(CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.endTime).c_str(), CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.endTime).length(), alloc);
		Value size(std::to_string(file.size / 1024 / 1024).c_str(), std::to_string(file.size / 1024 / 1024).length(), alloc);

		Value a(kArrayType);
		a.PushBack(name, alloc).PushBack(channel, alloc).PushBack(beginTime, alloc).PushBack(endTime, alloc).PushBack(size, alloc);
		root.AddMember(key.Move(), a.Move(), alloc);
	}

	Writer<OStreamWrapper> writer(osw);
	root.Accept(writer);
}

std::vector<Record> CCommonUtrl::LoadSearchFileListFromFile()
{
	std::vector<Record> files;

	std::string configfile = "SearchFileList.config";
	std::ifstream ifs(configfile);
	IStreamWrapper isw(ifs);
	Document d;
	d.ParseStream(isw);
	size_t file_size = isw.Tell();
	if (isw.Tell() == 0)
	{
		return files;
	}

	typedef Value::ConstMemberIterator Iter;
	for (Iter it = d.MemberBegin(); it != d.MemberEnd(); it++)
	{
		std::string keyName = it->name.GetString();
		const Value& a = d[keyName.c_str()];

		assert(a.IsArray());
		if (!a.IsArray() || a.Size() < 5)
			continue;

		std::string fileName = a[0].GetString();
		std::string channel = a[1].GetString();
		std::string beginTime = a[2].GetString();
		std::string endTime = a[3].GetString();
		std::string size = a[4].GetString();
	}

	return files;
}