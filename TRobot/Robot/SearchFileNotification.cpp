#include "SearchFileNotification.h"


SearchFileNotification::SearchFileNotification(NOTIFICATION_TYPE eNotify, int nData)
{
	m_eNotify = eNotify;
	switch (eNotify)
	{
	case Notification_Type_Search_File_Process:
		m_nSearchPos = nData;
		break;

	case Notification_Type_Search_File_TotalSize:
		m_nTotal = nData;
		break;

	case Notification_Type_Search_File_Finish:
		break;

	case Notification_Type_Search_File_Failure:
		break;

	default:
		break;
	}
}


SearchFileNotification::~SearchFileNotification()
{
}


NOTIFICATION_TYPE SearchFileNotification::GetNotify()
{
	return m_eNotify;
}

int SearchFileNotification::GetData()
{
	switch (m_eNotify)
	{
	case Notification_Type_Search_File_Process:
		return m_nSearchPos;
		break;

	case Notification_Type_Search_File_TotalSize:
		return m_nTotal;
		break;

	case Notification_Type_Search_File_Finish:
		return SEARCHFILE_DEFAULT;
		break;

	case Notification_Type_Search_File_Failure:
		return SEARCHFILE_DEFAULT;
		break;

	default:
		return SEARCHFILE_DEFAULT;
		break;
	}

	return 0;
}
