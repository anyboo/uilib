#pragma once
#include <map>

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

	HICON QueryFileIcon(LPCTSTR lpszFilePath);
	void MapInit();
	void AddToMap(LPCTSTR LayoutName);
	void LayMove(CVerticalLayoutUI* cLyt, int nMove);
	void Vacated_position(int iPos);
	void AddNewLayout(int nPosX, LPCTSTR pFileName);
	HBITMAP IconToBitmap(HICON hIcon, SIZE* pTargetSize = NULL);
	BOOL SaveBmp(HBITMAP hBitmap, LPCSTR FileName);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CPaintManagerUI m_pm;
	map<CVerticalLayoutUI*, RECT> m_layoutPos;
};

