#pragma once
#include <map>
#include <wtypes.h>
#include "resource.h"
#include <vector>
#include "atlimage.h"
#include <Shellapi.h>

#include "MenuWnd.h"

class Launcher : public CWindowWnd, public INotifyUI
{
public:
	Launcher();
	~Launcher();

protected:
	LPCTSTR GetWindowClassName() const { return _T("UILauncher"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init(){};
	void Notify(TNotifyUI& msg);

	void PopMenu(TNotifyUI& msg);
	void DeleteLyt();

	HICON QueryFileIcon(LPCTSTR lpszFilePath);
	void MapInit();
	void AddToMap(LPCTSTR LayoutName);
	void LayMove(CVerticalLayoutUI* cLyt, int nMove);
	void Vacated_position(int iPos);
	void AddLayout(int nPosX, LPCTSTR pFileName, const char* strName);
	HBITMAP IconToBitmap(HICON hIcon, SIZE* pTargetSize = NULL);
	BOOL SaveBmp(HBITMAP hBitmap, LPCSTR FileName);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CPaintManagerUI m_pm;
	POINT m_MenuPt;
	int m_Nbmp = 1;
	vector<CVerticalLayoutUI*> m_AllLyt;
	vector<char*> m_filePath;
};

