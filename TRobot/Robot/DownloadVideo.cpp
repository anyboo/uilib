#include "DownloadVideo.h"


DownloadVideo::DownloadVideo()
{
}


DownloadVideo::~DownloadVideo()
{
}


void DownloadVideo::GetDownLoadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj)
{
	if (NULL != dObj)
	{
		std::vector<SDK_DOWNLOAD_INFO>::iterator itSDI;
		for (itSDI != SDIObj.begin(); itSDI != SDIObj.end(); ++itSDI)
		{
			std::vector<std::string>::iterator itStr;
			for (itStr != itSDI->strVector.begin(); itStr != itSDI->strVector.end(); ++itStr)
			{
				//�������̣߳������ص���100��ʱ��رմ������߳�
				dObj->Download(itSDI->tChannel, *itStr);
			}
		}
	}// if
}

bool DownloadVideo::StopDownLoad(Device *dObj)
{
	return dObj->StopDownload();
}

//����������ļ���С�����ļ���С
int DownloadVideo::getDownloadPos(Device *dObj, int dDownLoadSize, int nTotalSize)
{
	if (NULL != dObj)
	{

	}

	return 0;
}

//���������ٶ�
int CalDownLoadSpeed()
{

	return 0;
}

//������ʱ��
time_t DownloadTime()
{
	return 0;
}

//ʣ��ʱ��
time_t RemianTime()
{
	return 0;
}
