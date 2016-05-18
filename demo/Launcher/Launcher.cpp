#include "stdafx.h"
#include "Launcher.h"

#include "ostream"

#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "string"

#include "ostreamwrapper.h"
#include "istreamwrapper.h"
#include <fstream>

using namespace std;

#define CONTROL_LAYOUT_OUT 68
#define BUTTON_WIDTH 32
#define BUTTON_HEIGHT 32
#define LABLE_WIDTH 48
#define LABLE_HEIGHT 15
#define FONT_SIZE 12
#define BORD_WIDTH 35
#define LYT_WIDTH 135
#define LYT_HEIGHT 110


#define BMPBITCOUNT 32


Launcher::Launcher()
{
}

Launcher::~Launcher()
{
}

void Launcher::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK) {
		if (msg.pSender->GetName() == _T("closebtn")) {
			SaveLytToJsonFile();
			PostQuitMessage(0);
		}
		else if (msg.pSender->GetName() == _T("minbtn")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else {
			m_xyPos = { msg.ptMouse.x, msg.ptMouse.y };
			OpenExeFile(m_xyPos.x, m_xyPos.y);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_MENU)
	{
		PopMenu(msg);
	}
	else if (msg.sType == _T("menu_Delete")) {
		DeleteLyt();
	}
	else if (msg.sType == _T("menu_Open")) {
		OpenExeFile(m_MenuPt.x, m_MenuPt.y);
	}
	else if (msg.sType == _T("menu_Rename")){
		//DeleteLyt();
	}
	else if (msg.sType == DUI_MSGTYPE_TABSELECT){
		OnMouseMove(msg.ptMouse.x, msg.ptMouse.y);
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED){
		CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
		for (UINT j = 0; j < m_AllLyt.size(); j++)
		{
			cLyt = m_AllLyt[j].Layout;
			cLyt->SetBkColor(NULL);

		}
	}
}

void Launcher::SaveLytToJsonFile()
{
	rapidjson::Document document;
	document.Parse("test.json");
	ofstream ofs("test.json");
	rapidjson::OStreamWrapper osw(ofs);

	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);
	const char* strFilePath = NULL;
	for (UINT i = 0; i < m_AllLyt.size(); i++)
	{
		strFilePath = m_AllLyt[i].FilePath;

		char strPath[100] = { 0 };
		char strExName[100] = { 0 };
		char strType[16] = { 0 };
		_splitpath_s(strFilePath, NULL, 0, strPath, 100, strExName, 100, strType, 16);
	
		rapidjson::Value array(rapidjson::kArrayType);

		rapidjson::Value file_path(rapidjson::kObjectType);
		rapidjson::Value file_name(rapidjson::kObjectType);
		rapidjson::Value file_rename(rapidjson::kObjectType);

		file_name.SetString(strExName, allocator);	
		file_path.SetString(strPath, allocator);
		file_rename.SetString(strFilePath, allocator);
	
		array.PushBack(file_path, allocator);
		array.PushBack(file_rename, allocator);

		root.AddMember(file_name, array, allocator);		
	}

	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
	root.Accept(writer);
	
}

void Launcher::PopMenu(TNotifyUI& msg)
{
 	CMenuWnd* pMenu = new CMenuWnd();
	if (pMenu == NULL) { return; }
	m_MenuPt = { msg.ptMouse.x, msg.ptMouse.y };
	POINT pt = m_MenuPt;
	::ClientToScreen(m_hWnd, &pt);
	pMenu->Init(msg.pSender, pt);
}

void Launcher::DeleteLyt()
{
	CHorizontalLayoutUI* cListLyt = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("ListLayout")));
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	POINT pt = m_MenuPt;
	int xPos = pt.x;
	int yPos = pt.y;
	
	UINT n = xPos / LYT_WIDTH + (yPos - BORD_WIDTH) / LYT_HEIGHT * 4;
	if (n > m_AllLyt.size() || m_AllLyt.size() == 0)
		return;
	cLyt = m_AllLyt[n].Layout;
	cListLyt->Remove(cLyt);
	m_AllLyt.erase(m_AllLyt.begin() + n);
}

void Launcher::OpenExeFile(int xPos, int yPos)
{	
	UINT n = xPos / LYT_WIDTH + (yPos - BORD_WIDTH) / LYT_HEIGHT * 4;
	if (m_AllLyt.size() == 0 || m_AllLyt.size() - 1 < n)
		return;
	if (xPos < 5 || yPos < BORD_WIDTH || n > m_AllLyt.size() - 1 || xPos - 16 < 0 || (xPos - 16) % LYT_WIDTH > 95
		|| yPos - BORD_WIDTH < 0 || (yPos - BORD_WIDTH) % LYT_HEIGHT > 90)
	{
		return;
	}
	const char* str = m_AllLyt[n].FilePath;

	LPCTSTR strFilePath = NULL;
#ifdef _UNICODE
	wchar_t strFilePathTmp[100] = { 0 };
	c2w(strFilePathTmp, 100, str);
	strFilePath = strFilePathTmp;	
#else	
	strFilePath = str;
#endif
	
	HINSTANCE hIns = ShellExecute(NULL, _T("Open"), strFilePath, NULL, NULL, SW_SHOWNORMAL);
	int dret = (int)hIns;
	if (dret < 32)
		MessageBox(NULL, _T("Open file Failure!"), _T("message"), MB_OK);
}

void Launcher::OnMouseMove(int xPos, int yPos)
{
	UINT n = xPos / LYT_WIDTH + (yPos - BORD_WIDTH) / LYT_HEIGHT * 4;	
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	cLyt = m_AllLyt[n].Layout;
	cLyt->SetBkColor(0xFFFF9999);
}

void Launcher::c2w(wchar_t *pwstr, size_t len, const char *str)
{
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, NULL, 0);
		if (n >= len)n = len - 1;
		MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, pwstr, (int)n);
		pwstr[n] = 0;
	}
}

void Launcher::w2c(char *pcstr, const wchar_t *pwstr, int len)
{
	int nlength = wcslen(pwstr);
	int nbytes = WideCharToMultiByte(0, 0, pwstr, nlength, NULL, 0, NULL, NULL);
	if (nbytes>len)   nbytes = len;
	WideCharToMultiByte(0, 0, pwstr, nlength, pcstr, nbytes, NULL, NULL);
}

LRESULT Launcher::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("launcher.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	if (m_Nbmp == 1)
		MapInit();
	Init();
	return 0;
}

void Launcher::Init()
{
}

LRESULT Launcher::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT Launcher::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT Launcher::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this)) {
		RECT rcSizeBox = m_pm.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top) {
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
			_tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
			_tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
			return HTCAPTION;
	}
	return HTCLIENT;
}

LRESULT Launcher::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT Launcher::OnDropFiles(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled)
{
	//获取拖动文件松开鼠标时的x坐标
	POINT* ptDropPos = new POINT;
	DragQueryPoint(hDrop, ptDropPos);	//把文件拖动到的位置存到ptDropPos中
	int iDropPos = ptDropPos->x;
	int yDropPos = ptDropPos->y;
	delete(ptDropPos);

	WORD wNumFilesDropped = DragQueryFile(hDrop, -1, NULL, 0);

	if (m_AllLyt.size() + wNumFilesDropped > 21){
		MessageBox(NULL, _T("can't drop too many files!"), _T("message"), MB_OK);
		return 0;
	}

	for (unsigned short i = 0; i < wNumFilesDropped; i++)
	{
		HICON hIcon = NULL;
		char strTmp[20] = { 0 };
		_itoa(m_Nbmp, strTmp, 10);
		strcat_s(strTmp, "tem.bmp");
		m_Nbmp++;

		LPCTSTR pBmpFilename = NULL;
#ifdef _UNICODE
		wchar_t strFileName[MAX_PATH] = { 0 };
		wchar_t pBmpFilenameTmp[100] = { 0 };
		c2w(pBmpFilenameTmp, 100, strTmp);
		pBmpFilename = pBmpFilenameTmp;
		DragQueryFile(hDrop, i, strFileName, MAX_PATH);
#else
		char strFileName[MAX_PATH] = { 0 };
		pBmpFilename = strTmp;
		DragQueryFile(hDrop, i, strFileName, MAX_PATH);
#endif
		

		hIcon = QueryFileIcon(strFileName);
		HBITMAP IconHbmp = IconToBitmap(hIcon);
		SaveBmp(IconHbmp, strTmp);		
		AddLayout(iDropPos, yDropPos, pBmpFilename, strFileName);
	}
	return 0;
}

LRESULT Launcher::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right - rcWork.left;
	lpMMI->ptMaxSize.y = rcWork.bottom - rcWork.top;
	bHandled = FALSE;
	return 0;
}

LRESULT Launcher::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_DROPFILES:	    lRes = OnDropFiles(uMsg, HDROP(wParam), lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

HICON Launcher::QueryFileIcon(LPCTSTR lpszFilePath)
{
	HICON hIcon = NULL;
	SHFILEINFO FileInfo;
	DWORD_PTR dwRet = ::SHGetFileInfo(lpszFilePath, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
	if (dwRet)
	{
		hIcon = FileInfo.hIcon;
	}
	return hIcon;
}

void Launcher::MapInit()
{
	ifstream ifs("test.json");
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document d;
	d.ParseStream(isw);
	size_t file_size = isw.Tell();
	if (isw.Tell() == 0)
	return;

	rapidjson::Value::ConstMemberIterator itr = d.MemberBegin();
	for (; itr != d.MemberEnd(); itr++)
	{
		string TypeName = itr->name.GetString();
		const rapidjson::Value& a = d[TypeName.c_str()];
		assert(a.IsArray());
		string str = a[1].GetString();
		GetIcon(str.c_str());
	}	
}

void Launcher::GetIcon(const char* strPath)
{
	char strTmp[20] = { 0 };
	_itoa(m_Nbmp, strTmp, 10);
	strcat_s(strTmp, "tem.bmp");
	m_Nbmp++;	
	
	LPCTSTR strFilePath = NULL;
	LPCTSTR pBmpFilename = NULL;
#ifdef _UNICODE
	wchar_t strFilePathTmp[100] = { 0 };
	wchar_t pBmpFilenameTmp[100] = { 0 };
	c2w(strFilePathTmp, 100, strPath);
	c2w(pBmpFilenameTmp, 100, strTmp);
	strFilePath = strFilePathTmp;
	pBmpFilename = pBmpFilenameTmp;
#else	
	strFilePath = strPath;
	pBmpFilename = strTmp;
#endif
	HICON hIcon = QueryFileIcon(strFilePath);
	HBITMAP IconHbmp = IconToBitmap(hIcon);
	SaveBmp(IconHbmp, strTmp);
	
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	InitLayOut(cLyt, pBmpFilename, strFilePath);

	LayOut_Info Lyt_info = { 0 };
	Lyt_info.Layout = cLyt;
	memcpy(Lyt_info.FilePath, strPath, strlen(strPath));

	m_AllLyt.push_back(Lyt_info);

	ShowLayOut();
}

void Launcher::AddToMap(LPCTSTR LayoutName)
{
}

void Launcher::AddLayout(int nPosX, int nPosY, LPCTSTR pFileName, LPCTSTR strName)
{	
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	InitLayOut(cLyt, pFileName, strName);
	Push_LayOut(nPosX, nPosY, cLyt, strName);
	ShowLayOut();
}

void Launcher::InitLayOut(CNewVerticalLayoutUI* cLyt, LPCTSTR pFileName, LPCTSTR strName)
{
	CNewButtonUI* cBtn = new CNewButtonUI;
	CLabelUI* Lab = new CLabelUI;
	CHorizontalLayoutUI* cListLyt = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("ListLayout")));
	cListLyt->Add(cLyt);
	cLyt->Add(cBtn);
	cLyt->Add(Lab);
	cBtn->SetContextMenuUsed(true);
	cBtn->SetFixedHeight(BUTTON_HEIGHT);
	cBtn->SetFixedWidth(BUTTON_WIDTH);
	cBtn->SetBkImage(pFileName);
	cBtn->SetToolTip(strName);
	RECT r;
	r.left = r.right = 20;
	r.bottom = r.top = 5;

	cBtn->SetPadding(r);

	char strFileName[100] = { 0 };
	char strExName[100] = { 0 };
	char strType[16] = { 0 };

	LPCTSTR strText = NULL;
#ifdef _UNICODE	
	w2c(strFileName, strName, 100);
	_splitpath_s(strFileName, NULL, 0, NULL, 0, strExName, 100, strType, 16);
	if (strcmp(strType, ".lnk") != 0 && strcmp(strType, ".exe") != 0){
		strcat_s(strExName, strType);
	}
	wchar_t str[100] = { 0 };
	c2w(str, 100, strExName);
	strText = str;
#else
	memcpy(strFileName, strName, 100);
	_splitpath_s(strFileName, NULL, 0, NULL, 0, strExName, 100, strType, 16);
	if (strcmp(strType, ".lnk") != 0 && strcmp(strType, ".exe") != 0){
		strcat_s(strExName, strType);
	}
	strText = strExName;
#endif

	Lab->SetText(strText);
	Lab->SetFont(FONT_SIZE);
	Lab->SetMultiLine(true);
	RECT rect;
	rect.left = rect.right = 5;
	rect.bottom = rect.top = 0;
	Lab->SetPadding(rect);
	Lab->SetToolTip(strName);
}

void Launcher::Push_LayOut(int xPos, int yPos, CNewVerticalLayoutUI* cLyt, LPCTSTR strName)
{
	LayOut_Info Lyt_info = { 0 };
	Lyt_info.Layout = cLyt;
	char strFilePath[100] = { 0 };
#ifdef _UNICODE
	w2c(strFilePath, strName, 100);
#else
	memcpy(strFilePath, strName, 100);
#endif

	memcpy(Lyt_info.FilePath, strFilePath, 100);
	UINT Xnum = xPos / LYT_WIDTH + 1;
	UINT Ynum = (yPos - BORD_WIDTH) / LYT_HEIGHT;
	UINT n = Ynum * 4 + Xnum;
	if (n > m_AllLyt.size()){
		m_AllLyt.push_back(Lyt_info);
	}
	else{
		m_AllLyt.insert(m_AllLyt.begin() + n - 1, Lyt_info);
	}
}

void Launcher::ShowLayOut()
{
	CHorizontalLayoutUI* cListLyt = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("ListLayout")));
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	vector<LayOut_Info>::iterator it = m_AllLyt.begin();
	for (; it != m_AllLyt.end(); it++)
	{
		cListLyt->Remove((it)->Layout, true);
	}

	for (UINT j = 0; j < m_AllLyt.size(); j++)
	{
		LPCTSTR strTool = NULL;
#ifdef UNICODE
		wchar_t strToolTmp[100] = { 0 };
		c2w(strToolTmp, 100, m_AllLyt[j].FilePath);
		strTool = strToolTmp;
#else
		strTool = m_AllLyt[j].FilePath;
#endif

		cLyt = m_AllLyt[j].Layout;
		cLyt->SetContextMenuUsed(true);
		cLyt->SetToolTip(strTool);
		
		RECT rect;
		rect.left = rect.right = 10;
		rect.bottom = rect.top = 5;
		cLyt->SetPadding(rect);
		cListLyt->Add(cLyt);
	}
}

HBITMAP Launcher::IconToBitmap(HICON hIcon, SIZE* pTargetSize)
{
	ICONINFO info = { 0 };
	if (hIcon == NULL
		|| !GetIconInfo(hIcon, &info)
		|| !info.fIcon)
	{
		return NULL;
	}

	INT nWidth = 0;
	INT nHeight = 0;
	if (pTargetSize != NULL)
	{
		nWidth = pTargetSize->cx;
		nHeight = pTargetSize->cy;
	}
	else
	{
		if (info.hbmColor != NULL)
		{
			BITMAP bmp = { 0 };
			GetObject(info.hbmColor, sizeof(bmp), &bmp);

			nWidth = bmp.bmWidth;
			nHeight = bmp.bmHeight;
		}
	}

	if (info.hbmColor != NULL)
	{
		DeleteObject(info.hbmColor);
		info.hbmColor = NULL;
	}

	if (info.hbmMask != NULL)
	{
		DeleteObject(info.hbmMask);
		info.hbmMask = NULL;
	}

	if (nWidth <= 0
		|| nHeight <= 0)
	{
		return NULL;
	}

	INT nPixelCount = nWidth * nHeight;

	HDC dc = GetDC(NULL);
	INT* pData = NULL;
	HDC dcMem = NULL;
	HBITMAP hBmpOld = NULL;
	bool* pOpaque = NULL;
	HBITMAP dib = NULL;
	BOOL bSuccess = FALSE;

	do
	{
		BITMAPINFOHEADER bi = { 0 };
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = nWidth;
		bi.biHeight = -nHeight;
		bi.biPlanes = 1;
		bi.biBitCount = BMPBITCOUNT;
		bi.biCompression = BI_RGB;
		dib = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (VOID**)&pData, NULL, 0);
		if (dib == NULL) break;

		memset(pData, 0, nPixelCount * 4);

		dcMem = CreateCompatibleDC(dc);
		if (dcMem == NULL) break;

		hBmpOld = (HBITMAP)SelectObject(dcMem, dib);
		::DrawIconEx(dcMem, 0, 0, hIcon, nWidth, nHeight, 0, NULL, DI_MASK);

		pOpaque = new(std::nothrow) bool[nPixelCount];
		if (pOpaque == NULL) break;
		for (INT i = 0; i < nPixelCount; ++i)
		{
			pOpaque[i] = !pData[i];
		}

		memset(pData, 0, nPixelCount * 4);
		::DrawIconEx(dcMem, 0, 0, hIcon, nWidth, nHeight, 0, NULL, DI_NORMAL);

		BOOL bPixelHasAlpha = FALSE;
		UINT* pPixel = (UINT*)pData;
		for (INT i = 0; i < nPixelCount; ++i, ++pPixel)
		{
			if ((*pPixel & 0xff000000) != 0)
			{
				bPixelHasAlpha = TRUE;
				break;
			}
		}

		if (!bPixelHasAlpha)
		{
			pPixel = (UINT*)pData;
			for (INT i = 0; i < nPixelCount; ++i, ++pPixel)
			{
				if (pOpaque[i])
				{
					*pPixel |= 0xFF000000;
				}
				else
				{
					*pPixel &= 0x00FFFFFF;
				}
			}
		}

		bSuccess = TRUE;

	} while (FALSE);


	if (pOpaque != NULL)
	{
		delete[]pOpaque;
		pOpaque = NULL;
	}

	if (dcMem != NULL)
	{
		SelectObject(dcMem, hBmpOld);
		DeleteDC(dcMem);
	}

	ReleaseDC(NULL, dc);

	if (!bSuccess)
	{
		if (dib != NULL)
		{
			DeleteObject(dib);
			dib = NULL;
		}
	}

	return dib;
}

BOOL Launcher::SaveBmp(HBITMAP hBitmap, LPCSTR FileName)
{
//	hBitmap = TransparentImage(hBitmap);
	HDC     hDC;
	//当前分辨率下每象素所占字节数         
	int     iBits;
	//位图中每象素所占字节数         
	WORD     wBitCount;
	//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数             
	DWORD     dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构             
	BITMAP     Bitmap;
	//位图文件头结构         
	BITMAPFILEHEADER     bmfHdr;
	//位图信息头结构             
	BITMAPINFOHEADER     bi;
	//指向位图信息头结构                 
	LPBITMAPINFOHEADER     lpbi;
	//定义文件，分配内存句柄，调色板句柄             
	HANDLE     fh, hDib, hPal, hOldPal = NULL;

	//计算位图文件每个像素所占字节数             
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL)     *     GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else  if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = BMPBITCOUNT;

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//为位图内容分配内存             
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	//     处理调色板                 
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//     获取该调色板下新的像素值             
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	//恢复调色板                 
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//创建位图文件 
	LPCTSTR strFileName = NULL;
#ifdef _UNICODE
	wchar_t strT[100] = { 0 };
	c2w(strT, 100, FileName);
	strFileName = strT;
#else
	strFileName = FileName;
#endif
	
	fh = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)         return     FALSE;

	//     设置位图文件头             
	bmfHdr.bfType = 0x4D42;     //     "BM"             
	dwDIBSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
	//     写入位图文件头             
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//     写入位图文件其余内容             
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除                 
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return     TRUE;
}
