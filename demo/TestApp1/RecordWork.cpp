#include "stdafx.h"
#include "RecordWork.h"


CMainWndWork::CMainWndWork()
{
}


CMainWndWork::~CMainWndWork()
{
}

void CMainWndWork::OnGetScreen()
{

}

void CMainWndWork::OnRecord(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{
	char buf[1024];
#if 1
	sprintf_s(buf, "bScreen: %d\nbSound: %d\nrArea.right: %d\niCode: %d\nbSysSound:%d\nbMicrophone:%d\ncSaveDir:%s",
		bScreenRecord, bSoundRecord, rArea.right, iCode, bSysSound, bMicrophone, cSaveDir);
#else
	sprintf_s(buf, "aaaaa");
#endif
	OutputDebugString(buf);
}

void CMainWndWork::OnPause(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{

}

void CMainWndWork::OnGoon(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{

}

void CMainWndWork::OnStop(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{

}

void CMainWndWork::OnSetChange(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{
	int i = 0;
	char buf[1024];
#if 1
	sprintf_s(buf, "change:%d\nbScreen: %d\nbSound: %d\nrArea.right: %d\niCode: %d\nbSysSound:%d\nbMicrophone:%d\ncSaveDir:%s",
		i++, bScreenRecord, bSoundRecord, rArea.right, iCode, bSysSound, bMicrophone, cSaveDir);
#else
	sprintf_s(buf, "aaaaa");
#endif
//	OutputDebugString(buf);
	DUI__Trace(buf);

}

