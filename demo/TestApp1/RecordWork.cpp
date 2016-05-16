#include "stdafx.h"
#include "RecordWork.h"


CRecordWork::CRecordWork()
{
}


CRecordWork::~CRecordWork()
{
}

void CRecordWork::OnGetScreen()
{

}

void CRecordWork::OnRecord(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
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

void CRecordWork::OnPause(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{

}

void CRecordWork::OnGoon(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{

}

void CRecordWork::OnStop(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
{

}

void CRecordWork::OnSetChange(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir)
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

