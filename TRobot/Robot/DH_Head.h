#ifndef __DH_HEAD_H__
#define __DH_HEAD_H__

#include <iostream>
#include <string>
#include <map>
#include<vector>

#include <assert.h> 
#include <tchar.h>

// Json 
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ostreamwrapper.h"
#include "istreamwrapper.h"

using namespace rapidjson;

//Database
#include "QMSqlite.h"

#define MAX_SEARCH_COUNT 1000
#define ONE_DAY		 (24 * 60 * 60)
#define ONE_HOUR	 (60 * 60)
#define ONE_MINUTE	 (60)

//������Ƶ�ļ���Ϣ
struct RecordFile
{
	RecordFile()
	{
		channel = 0;
		size = 0;
		pPrivateData = nullptr;
		PrivateDataDataSize = 0;
	}

	~RecordFile()
	{
		if (nullptr != pPrivateData)
		{
			delete pPrivateData;
			pPrivateData = nullptr;
			PrivateDataDataSize = 0;
		}
	}

	RecordFile(const RecordFile& other)
	{
		channel = other.channel;
		size = other.size;
		name = other.name;
		beginTime = other.beginTime;
		endTime = other.endTime;
		pPrivateData = nullptr;
		PrivateDataDataSize = 0;
		setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
	}

	RecordFile& operator= (const RecordFile& other)
	{
		if (&other == this)
		{
			return *this;
		}
		channel = other.channel;
		size = other.size;
		name = other.name;
		beginTime = other.beginTime;
		endTime = other.endTime;
		setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
		return *this;
	}

	void setPrivateData(void* pData, int size)
	{
		if (nullptr != pPrivateData)
		{
			delete pPrivateData;
			pPrivateData = nullptr;
			PrivateDataDataSize = 0;
		}


		if (size > 0)
		{
			pPrivateData = new char[size];
			PrivateDataDataSize = size;
			memcpy(pPrivateData, pData, size);
		}
	}
	//ȡ��˽������
	void* getPrivateData() const
	{
		return pPrivateData;
	}
	__int32 getPrivateDataSize() const
	{
		return PrivateDataDataSize;
	}

	__int32 channel;      //ͨ��
	__int64 size;         //�ļ���С(byte)
	std::string  name;    //�ļ�����
	__time64_t beginTime; //����ʱ��
	__time64_t endTime;   //����ʱ��
	char* pPrivateData;   //˽������
	__int32 PrivateDataDataSize;//˽�����ݴ�С
};

#endif