#pragma once
#include "StdAfx.h"
#include "NewUICtrol.h"
class CMyHandle
{
public:
	CMyHandle();
	~CMyHandle();

public:
	STDSTRING GetFileSavePath();

	void PopMenu(TNotifyUI& msg, HWND& m_hWnd);
	void DeleteLyt(CPaintManagerUI& mpm, vector<LayOut_Info>& AllLyt);
	void OpenExeFile(int xPos, int yPos, vector<LayOut_Info>& AllLyt);
	void OnMouseMove(int xPos, int yPos, vector<LayOut_Info>& AllLyt);
	void OnMouseLeave(vector<LayOut_Info>& AllLyt);
	
	
	void AddLayout(int nPosX, int nPosY, STDSTRING pFileName, STDSTRING strPath, CPaintManagerUI& p_mp, vector<LayOut_Info>& AllLyt);
	void InitLayOut(CNewVerticalLayoutUI* cLyt, STDSTRING pFileName, STDSTRING strPath);
	void Push_LayOut(int xPos, int yPos, CNewVerticalLayoutUI* cLyt, STDSTRING strPath, vector<LayOut_Info>& AllLyt);
	void ShowLayOut(CPaintManagerUI& p_mp, vector<LayOut_Info>& AllLyt);
	void GetIcon(STDSTRING strPath, vector<LayOut_Info>& AllLyt, CPaintManagerUI& p_mp);

	HICON QueryFileIcon(STDSTRING lpszFilePath);
	HBITMAP IconToBitmap(HICON hIcon, SIZE* pTargetSize = NULL);
	BOOL SaveBmp(HBITMAP hBitmap, STDSTRING FileName);

private:
	STDSTRING				tmp;
	POINT					m_xyPos;
};

