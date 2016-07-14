#ifndef __SEARCHFILENOTIFICATION_H__
#define __SEARCHFILENOTIFICATION_H__

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

#define SEARCHFILE_DEFAULT -1

typedef enum
{
	Notification_Type_None = 0,
	Notification_Type_Network_status_Connect,
	Notification_Type_Network_status_Disconnect,
	Notification_Type_Search_Device_Finish,
	Notification_Type_Search_File_Process,
	Notification_Type_Search_File_TotalSize,
	Notification_Type_Search_File_Finish,
	Notification_Type_Search_File_Cancel,
	Notification_Type_Download_File_Process,
	Notification_Type_Download_File_Finish,
	Notification_Type_Download_File_Cancel,
	Notification_Type_Device_Manage_Cancel,
	Notification_Type_Port_Scan_Finish,
	Notification_Type_Express,
}NOTIFICATION_TYPE;

class SearchFileNotification :public Notification
{
public:
	typedef AutoPtr<SearchFileNotification> Ptr;

	SearchFileNotification(NOTIFICATION_TYPE eNotify, int nData);
	~SearchFileNotification();

	NOTIFICATION_TYPE GetNotify();
	int GetData();
private:
	NOTIFICATION_TYPE m_eNotify;
	int m_nSearchPos;
	int m_nTotal;

};

#endif 

