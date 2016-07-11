#ifndef __DOWNLOADVIDEO_H__
#define __DOWNLOADVIDEO_H__


#include <vector>
#include <string>

#include "Device.h"

#include "AcquireDataRunable.h"
#include "DownloadFileNotificationQueue.h"

#include "Poco/ThreadPool.h"
using Poco::ThreadPool;


typedef struct _DOWNLOADID
{
	int nID;
	std::string strFileName;
}DOWNLOADID, *LPDOWNLOADID;


typedef struct  
{
	std::vector<DOWNLOADID> vecInfo;
	size_t tChannel;
}SDK_DOWNLOAD_INFO, *LPSDK_DOWNLOAD_INFO;

class DownloadVideo
{
public:
	DownloadVideo();
	~DownloadVideo();
	//����
	void SetDownloadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj);
	//���
	void GetDownloadInfo();
	//ֹͣ����
	bool StopDownLoad(Device *dObj);

	void getFlieSizeTest(DWORD &dwDownLoadSize, DWORD &dwTotalSize);

private:
	//����������ļ���С�����ļ���С
	int getDownloadPos(Device *dObj, DWORD dwDownLoadSize, DWORD dwTotalSize);
	//���������ٶ�
	int CalDownLoadSpeed();
	//������ʱ��
	time_t DownloadTime();
	//ʣ��ʱ��
	time_t RemianTime();

private:
	AcquireDataRunable m_AcquireDataRunable;
};

#endif 


