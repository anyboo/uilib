#pragma once
#include <map>
#include <wtypes.h>
#include "resource.h"
#include <list>
#include "atlimage.h"
#include <Shellapi.h>

#include "MenuWnd.h"

#include "NewUICtrol.h"

typedef struct
{
	CNewVerticalLayoutUI* Layout;
	STDSTRING FilePath;
	STDSTRING Display;
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

	void Notify(TNotifyUI& msg);

	void c2w(wchar_t *pwstr, size_t len, const char *str);
	void w2c(char *pcstr, const wchar_t *pwstr, int len);

	void SaveLytToJsonFile();
	void PopMenu(TNotifyUI& msg);
	void DeleteLyt();
	void OpenExeFile(int xPos, int yPos);
	void OnMouseMove(int xPos, int yPos);

	HICON QueryFileIcon(LPCTSTR lpszFilePath);
	void MapInit();
	void GetIcon(const char* strPath);

	void AddToMap(LPCTSTR LayoutName);
	void LayMove(CNewVerticalLayoutUI* cLyt, int nMove);
	void Vacated_position(int iPos);
	void AddLayout(int nPosX, int nPosY, LPCTSTR pFileName, LPCTSTR strName);

	void InitLayOut(CNewVerticalLayoutUI* cLyt, LPCTSTR pFileName, LPCTSTR strName);
	void Push_LayOut(int xPos, int yPos, CNewVerticalLayoutUI* cLyt, LPCTSTR strName);
	void ShowLayOut();

	HBITMAP IconToBitmap(HICON hIcon, SIZE* pTargetSize = NULL);
	BOOL SaveBmp(HBITMAP hBitmap, LPCSTR FileName);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CPaintManagerUI m_pm;
	POINT m_MenuPt;
	POINT m_xyPos;
	int m_Nbmp = 1;
	std::vector<LayOut_Info> m_AllLyt;
	HWND hListBoxWnd;
	STDSTRING WriteablePath;
};

