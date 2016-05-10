#pragma once

#include <wtypes.h>
#include <shellapi.h>

#define DIALOGUE_ADJ_LEFT -20
#define DIALOGUE_ADJ_TOP 12

#define LYT_TOP 35
#define LYT_BUTTON 100
#define LYT_LEFT 5
#define LYT_RIGHT 512

#define LYT_BK_COLOR 0xFFFF9999

#define PAGE_RECORD 1
#define PAGE_RECORDING 2
#define STATE_RECORDSCREEN 1
#define STATE_RECORDSOUND 2

#define SND_PER_MIN 60
#define MIN_PER_HOUR 60

class CRecord : public CWindowWnd, public INotifyUI
{
public:
	CRecord();
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);

	void Init();

	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnMouseMove(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled);

	void SetLytBkColor(LPCTSTR pLytName, int nPosX);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ChangePage();

	void SetPause();

	void SetGoon();

	void ScreenToSound();

	void SoundToScreen();

	void SetSaveDir();

	void OpenRecordFile();

	void OnTemer();

public:
	CPaintManagerUI m_pm;
	CHorizontalLayoutUI* m_pSelectPage;
	CHorizontalLayoutUI* m_pPage1;
	CControlUI* m_pPage2;
	CVerticalLayoutUI* m_JustScreenLyt[6];
	int m_nPageState = PAGE_RECORD;
	int m_nRecordState = STATE_RECORDSCREEN;
	int m_nRecordSecond = 0;
	int m_bPauseState = 0;
	TCHAR m_cSaveDir[MAX_PATH];
};

