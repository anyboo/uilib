#pragma once
//Code model
#define CODE_GIF 1
#define CODE_MP4 2
#define CODE_MKV 3
#define CODE_M4V 4
#define CODE_MOV 5
#define CODE_WMV 6
#define CODE_FLV 7
#define CODE_AVI 8
#define CODE_TS  9
#define CODE_VOB 10

class CRecordWork
{
public:
	CRecordWork();
	~CRecordWork();

	void OnGetScreen();

	void OnRecord(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir);

	void OnPause(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir);

	void OnGoon(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir);

	void OnStop(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir);

	void OnSetChange(bool bScreenRecord, bool bSoundRecord, RECT rArea, int iCode, bool bSysSound, bool bMicrophone, char* cSaveDir);

};

