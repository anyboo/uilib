#pragma once
class CRecordWork
{
public:
	CRecordWork();
	~CRecordWork();

	void OnGetScreen();

	void OnRecord();

	void OnPause();

	void OnGoon();

	void OnStop();

public:
	bool m_bScreenRecord = true;
	bool m_bSoundRecord = false;
	RECT m_rArea;
	int m_iCode;
	bool m_bSysSound;
	bool m_bMcf;
};

