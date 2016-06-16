#pragma once
#include <DuiLib/UIlib.h>
#include "Setting.h"
#include "MyHandle.h"


class CMainWnd :
	public WindowImplBase
{
public:
	CMainWnd();
	~CMainWnd();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnMin(TNotifyUI& msg);
	void OnClose(TNotifyUI& msg);
	

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void BitmapToJpg(HBITMAP hbmpImage, int width, int height, const std::wstring& path);

	LRESULT OnDropFiles(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	STDSTRING				m_WriteablePath;
	Config					m_config;
	CMyHandle				m_MyHandle;
	vector<LayOut_Info>		m_AllLyt;	
	UINT					m_begin = 1;
	POINT					m_MenuPos;
	UINT					m_LoadNum;
	STDSTRING				m_BmpNameHead;
};

