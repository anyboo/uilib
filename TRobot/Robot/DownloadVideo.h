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
	//输入
	void GetDownLoadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj);
	//停止下载
	bool StopDownLoad(Device *dObj);
private:
	//获得已下载文件大小和总文件大小
	int getDownloadPos(Device *dObj, int dDownLoadSize, int nTotalSize);
	//计算下载速度
	int CalDownLoadSpeed();
	//已下载时间
	time_t DownloadTime();
	//剩余时间
	time_t RemianTime();
};

#endif 


