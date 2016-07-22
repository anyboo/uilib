
#include "CommonUtrl.h"
#include <Poco/SingletonHolder.h>

using namespace rapidjson;

CCommonUtrl::CCommonUtrl()
{
}

CCommonUtrl::~CCommonUtrl()
{
}

CCommonUtrl& CCommonUtrl::getInstance()
{
	static Poco::SingletonHolder<CCommonUtrl> shCommonUtrl;
	return *shCommonUtrl.get();
}

std::string CCommonUtrl::MakeFileName(int channel, const std::string& startTime, const std::string& endTime, const std::string& extensions)
{
	std::string strFileName;

	strFileName += "channel";
	strFileName += std::to_string(channel);
	strFileName += "-";
	strFileName += startTime.data();
	strFileName += "-";
	strFileName += endTime.data();
	strFileName.append(extensions);

	return strFileName;
}
std::string CCommonUtrl::MakeDownloadFileFolder(const std::string basePath, const std::string& startTimeZero, const std::string& endTimeZero, const std::string& venderName, int channel)
{
	std::string strPath = basePath;
	strPath.append("\\");
	strPath += startTimeZero.data();
	strPath.append("-");
	strPath += endTimeZero.data();
	strPath.append("\\");
	strPath += venderName.data();
	strPath.append("\\");
	strPath.append("通道");
	strPath += std::to_string(channel);
	strPath.append("\\");

	MakeFolder(strPath);

	return strPath;
}

void CCommonUtrl::MakeFolder(std::string fileName)
{
	char *tag;
	for (tag = (char *)fileName.c_str(); *tag; tag++)
	{
		if (*tag == '\\')
		{
			char buf[1000], path[1000];
			strcpy(buf, fileName.c_str());
			buf[strlen(fileName.c_str()) - strlen(tag) + 1] = NULL;
			strcpy(path, buf);
			if (access(path, 6) == -1)
			{
				mkdir(path);
			}
		}
	}
}

std::vector<time_range> CCommonUtrl::MakeTimeRangeList(const time_range& range)
{
	time_t timeStart = range.start;
	time_t timeEnd = range.end;
	std::vector<time_range> timeRangeList;

	ComTime jStartTime, jStopTime;

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

	if (timeEnd - timeStart <= ONE_DAY)
	{
		if (jStartTime.date == jStopTime.date)
		{
			timeRangeList.push_back(range);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - jStartTime.hour) * ONE_HOUR + (59 - jStartTime.minute) * ONE_MINUTE + (59 - jStartTime.second);
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
		int day = (diff / ONE_DAY) + (diff % ONE_DAY > 0 ? 1 : 0);

		if (jStartTime.hour == 0 && jStartTime.minute == 0 && jStartTime.second == 0)
		{
			for (size_t i = 0; i < day - 1; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
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
			time_t diff = (23 - jStartTime.hour) * ONE_HOUR + (59 - jStartTime.minute) * ONE_MINUTE + (59 - jStartTime.second);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			timeStart = timeStart + diff + 1;
			for (size_t i = 0; i < day - 2; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
			}

			if (timeEnd > timeStart + ONE_DAY - 1)
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
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

std::string CCommonUtrl::MakeStrTimeByTimestamp(std::time_t time)
{
	std::string strTime = "20160101000000";
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime((char *)strTime.data(), strTime.length()+1, "%Y%m%d%H%M%S", &ttime);

	return strTime;
}

void CCommonUtrl::SaveSearchFileListToFile(const std::vector<RecordFile>& files, const std::string& VenderName)
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
		std::string fileKey = VenderName;
		Value key(fileKey.c_str(), fileKey.length(), alloc);

		RecordFile file = files[i];
		Value name(file.name.c_str(), file.name.length(), alloc);
		Value channel(std::to_string(file.channel).c_str(), std::to_string(file.channel).length(), alloc);
		Value beginTime(CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.beginTime).c_str(), CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.beginTime).length(), alloc);
		Value endTime(CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.endTime).c_str(), CCommonUtrl::getInstance().MakeStrTimeByTimestamp(file.endTime).length(), alloc);
		Value size(std::to_string(file.size).c_str(), std::to_string(file.size).length(), alloc);

		Value a(kArrayType);
		a.PushBack(name, alloc).PushBack(channel, alloc).PushBack(beginTime, alloc).PushBack(endTime, alloc).PushBack(size, alloc);
		root.AddMember(key.Move(), a.Move(), alloc);
	}

	Writer<OStreamWrapper> writer(osw);
	root.Accept(writer);
}

std::vector<RecordFile> CCommonUtrl::LoadSearchFileListFromFile()
{
	std::vector<RecordFile> files;

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

void CCommonUtrl::WriteFileListToDB(RECORD_FILE_LIST& recordFiles)
{
	//获取指针
	QMSqlite *pDb = QMSqlite::getInstance();
	////删除表
	//pDb->dropTable(DROP_SEARCH_VIDEO_TABLE);
	////创建记录表
	//pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);
	//一次插入所有数据
	std::vector<writeSearchVideo> RecordList;
	for (size_t i = 0; i < recordFiles.size(); i++)
	{
		writeSearchVideo sr;
		RecordFile record = recordFiles[i];
		//文件名称
		sr.set<0>(record.name);
		//通道号
		sr.set<1>(record.channel);
		//开始时间
		sr.set<2>(record.beginTime);
		//结束时间
		sr.set<3>(record.endTime);
		sr.set<4>(record.size);
		RecordList.push_back(sr);
	}

	if (RecordList.size() > 0)
	{
		std::string sql(INSERT_SEARCH_VIDEO);
		pDb->writeDataByVector(sql, RecordList);
	}
}

std::string CCommonUtrl::GetCurTime()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));

	std::string curTime(tmp);
	curTime = "[" + curTime;
	curTime += "] ";

	return curTime;
}