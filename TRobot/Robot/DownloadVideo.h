#ifndef __DOWNLOADVIDEO_H__
#define __DOWNLOADVIDEO_H__


#include "Device.h"
//#include "QMSqlite.h"

#include <vector>
#include <string>
#

typedef struct  
{
	std::vector<std::string> strVector;
	size_t tChannel;
}SDK_DOWNLOAD_INFO, *LPSDK_DOWNLOAD_INFO;

class DownloadVideo
{
public:
	DownloadVideo();
	~DownloadVideo();
	//����
	void GetDownLoadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj);
	//ֹͣ����
	bool StopDownLoad(Device *dObj);
private:
	//����������ļ���С�����ļ���С
	int getDownloadPos(Device *dObj, int dDownLoadSize, int nTotalSize);
	//���������ٶ�
	int CalDownLoadSpeed();
	//������ʱ��
	time_t DownloadTime();
	//ʣ��ʱ��
	time_t RemianTime();
};

#endif 


