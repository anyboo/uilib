#include "SendDataNotification.h"


SendDataNotification::SendDataNotification(DOWNLOADFILEINFO DownloadFileInfo):
m_DownloadFileInfo(DownloadFileInfo)
{

}

SendDataNotification::~SendDataNotification()
{

}

DOWNLOADFILEINFO SendDataNotification::GetData() const
{
	return m_DownloadFileInfo;
}