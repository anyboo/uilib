#pragma once
#include <map>
#include <wtypes.h>
#include "resource.h"
#include <vector>
#include "atlimage.h"
#include <Shellapi.h>

#include "MenuWnd.h"

typedef struct
{
	CVerticalLayoutUI* Layout;
	char FilePath[100];
}LayOut_Info;

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

	
	void SaveLytToJsonFile();
	void PopMenu(TNotifyUI& msg);
	void DeleteLyt();
	void OpenExeFile(int m_Point);

	HICON QueryFileIcon(LPCTSTR lpszFilePath);
	void MapInit();
	LPCSTR GetIcon(const char* strPath);

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
	LRESULT OnOpenFile(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled);
private:
	CPaintManagerUI m_pm;
	POINT m_MenuPt;
	int m_xPos;
	int m_Nbmp = 1;
	vector<LayOut_Info> m_AllLyt;
};

