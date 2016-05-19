#pragma once
#include <DuiLib/UIlib.h>

class CMainWnd : 
	public CWindowWnd, public INotifyUI, public CNotifyPump
{
public:
	CMainWnd();
	~CMainWnd();

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);

	void Notify(TNotifyUI& msg);

	void OnRecord(TNotifyUI& msg);
	void OnScreenRecord(TNotifyUI& msg);
	void OnAreaRecord(TNotifyUI& msg);
	void OnLocation(TNotifyUI& msg);
	void OnEncode(TNotifyUI& msg);
	void OnVoice(TNotifyUI& msg);
	void OnAbout(TNotifyUI& msg);
	void OnSettingDlg(TNotifyUI& msg);
	void OnMin(TNotifyUI& msg);
	void OnClose(TNotifyUI& msg);
private:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	DUI_DECLARE_MESSAGE_MAP();
	
	//void SetLytBkColor(LPCTSTR pLytName, int nPosX);

	/*
	void ChangePage();

	void SetPause();

	void SetGoon();

	void ScreenToSound();

	void SoundToScreen();

	void SetSaveDir();

	void OpenRecordFile();

	bool SetArea(int iAreaX, int iAreaY);

	void SaveSet();

	void OpenSetPage();
	*/

private:

	enum{
		PAGE_RECORD,
		PAGE_RECORDING,
		STATE_RECORDSCREEN,
		STATE_RECORDSOUND,
	};

	CPaintManagerUI m_pm;
	CHorizontalLayoutUI* m_pSelectPage;
	CHorizontalLayoutUI* m_pPage1;
	CControlUI* m_pPage2;
	CVerticalLayoutUI* m_JustScreenLyt[6];
	/*
	int m_nPageState = PAGE_RECORD;
	int m_nRecordState = STATE_RECORDSCREEN;
	int m_nRecordSecond = 0;
	int m_bPauseState = 0;
	*/

	DuiLib::CDuiString configfile; 
	RECT m_rArea;

	/*
	bool m_bScreenRecord = true;
	bool m_bSoundRecord = false;
	
	int m_iCode = CODE_GIF;
	bool m_bSysSound = true;
	bool m_bMcf = false;
	
	LPCTSTR m_pCodeName = NULL;
	LPCTSTR m_pLzPageName = NULL;
	*/
	//CMainWndWork* m_cRcdWk;
};

