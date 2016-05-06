#include "stdafx.h"
#include "Launcher.h"


using namespace std;

#define CONTROL_LAYOUT_OUT 68
#define BUTTON_WIDTH 48
#define BUTTON_HEIGHT 48
#define LABLE_WIDTH 48
#define LABLE_HEIGHT 15
#define FONT_SIZE 12
#define BORD_WIDTH 5
#define LYT_WIDTH 68

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
			PostQuitMessage(0);
		}
		else if (msg.pSender->GetName() == _T("minbtn")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if(msg.pSender->GetName() == _T("btlz")) {
			MessageBox(NULL, _T("Record"), _T("message"), MB_OK);
		}
		else if(msg.pSender->GetName() == _T("btpmbh")) {
			MessageBox(NULL, _T("Record Capture"), _T("message"), MB_OK);
		}
		else if(msg.pSender->GetName() == _T("btlzqy")) {
			MessageBox(NULL, _T("Record Area"), _T("message"), MB_OK);
		}
		else if(msg.pSender->GetName() == _T("btopen")) {
			MessageBox(NULL, _T("open"), _T("message"), MB_OK);
		}
		else if(msg.pSender->GetName() == _T("btencode")) {
			MessageBox(NULL, _T("coding"), _T("message"), MB_OK);
		}
		else if(msg.pSender->GetName() == _T("btvoice")) {
			MessageBox(NULL, _T("sound"), _T("message"), MB_OK);
		}
		else if(msg.pSender->GetName() == _T("btlzyx")) {
			MessageBox(NULL, _T("Record Game"), _T("message"), MB_OK);
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
		
		if (m_Nbmp == 1)
			MapInit();
		int xPos = m_MenuPt.x;
		int n = (xPos - BORD_WIDTH) / LYT_WIDTH;
		
		char * strPath = m_filePath[n];
		int a = 0;
	
	}
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
	if (m_Nbmp == 1)
		MapInit();
	CHorizontalLayoutUI* cListLyt = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("ListLayout")));

	int xPos = m_MenuPt.x;
	int n = (xPos - BORD_WIDTH) / LYT_WIDTH;
	CVerticalLayoutUI* cLyt = new CVerticalLayoutUI;

	cLyt = m_AllLyt[n];
	cListLyt->Remove(cLyt);
	m_AllLyt.erase(m_AllLyt.begin() + n);
	m_filePath.erase(m_filePath.begin() + n);
	m_Nbmp++;
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

	Init();
	return 0;
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

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0)
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
	if (m_Nbmp == 1)
		MapInit();
	if (m_AllLyt.size() >= 7){
		MessageBox(NULL, _T("No more than 7 files you drop!"), _T("message"), MB_OK);
		return 0;
	}	
	//获取拖动文件松开鼠标时的x坐标
	POINT* ptDropPos = new POINT;
	DragQueryPoint(hDrop, ptDropPos);	//把文件拖动到的位置存到ptDropPos中
	int iDropPos = ptDropPos->x;
	delete(ptDropPos);

	WORD wNumFilesDropped = DragQueryFile(hDrop, -1, NULL, 0);
	WORD wPathnameSize = 0;
	LPSTR lpFileName = NULL;
	WCHAR * pFilePathName = NULL;
	wstring strFirstFile = L"";
	HICON hIcon = NULL;

	char strTmp[20] = { 0 };
	_itoa(m_Nbmp, strTmp, 2);
	strcat_s(strTmp, "tem.bmp");
	LPCSTR pBmpFilename = strTmp;
	m_Nbmp++;
	//there may be many, but we'll only use the first
	if (wNumFilesDropped > 0)
	{
		wPathnameSize = DragQueryFile(hDrop, 0, NULL, 0);
		wPathnameSize++;
		pFilePathName = new WCHAR[wPathnameSize];
		if (NULL == pFilePathName)
		{
			_ASSERT(0);
			DragFinish(hDrop);
			return 0;
		}
		lpFileName = (LPSTR)pFilePathName;

		::ZeroMemory(pFilePathName, wPathnameSize);
		DragQueryFile(hDrop, 0, lpFileName, wPathnameSize);
		hIcon = QueryFileIcon((LPCTSTR)lpFileName);
		HBITMAP IconHbmp = IconToBitmap(hIcon);
		SaveBmp(IconHbmp, pBmpFilename);

		AddLayout(iDropPos, pBmpFilename, lpFileName);
		//	Vacated_position(iDropPos);
		remove(pBmpFilename);
		delete(pFilePathName);
	}
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
	/*case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;*/
	case WM_DROPFILES:	    lRes = OnDropFiles(uMsg, HDROP(wParam), lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
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
	/*static int i = 0;
	if (0 == i)
	{
		AddToMap(_T("btlz"));
		i++;
	}*/
}

//用于初始化时把已知的layout加进map
void Launcher::AddToMap(LPCTSTR LayoutName)
{
	CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(LayoutName));
	m_AllLyt.push_back(cLyt);
}


void Launcher::AddLayout(int nPosX, LPCTSTR pFileName, const char* strName)
{
	CVerticalLayoutUI* cLyt = new CVerticalLayoutUI;
	CButtonUI* cBtn = new CButtonUI;
	CLabelUI* Lab = new CLabelUI;
	CHorizontalLayoutUI* cListLyt = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("ListLayout")));
	cListLyt->Add(cLyt);
	cLyt->Add(cBtn);
	cLyt->Add(Lab);
	cLyt->SetContextMenuUsed(true);

	cBtn->SetFixedWidth(BUTTON_WIDTH);
	cBtn->SetFixedHeight(BUTTON_HEIGHT);
	cBtn->SetContextMenuUsed(true);
	cBtn->SetBkImage(pFileName);
	//	cBtn->SetBkColor(0xFFFFFFFF);


	LPCTSTR str = strName;
	char tmp[100] = { 0 };
	memcpy(tmp, str, strlen(str));
	Lab->SetText(str);
	Lab->SetFont(FONT_SIZE);
	Lab->SetFixedWidth(LABLE_WIDTH);
	Lab->SetFixedHeight(LABLE_HEIGHT);

	UINT num = (nPosX - BORD_WIDTH) / LYT_WIDTH + 1;
	if (num > m_AllLyt.size()){
		m_AllLyt.push_back(cLyt);
		m_filePath.push_back(tmp);
	}		
	else{
		m_AllLyt.insert(m_AllLyt.begin() + num - 1, cLyt);
		m_filePath.push_back(tmp);
	}

	vector<CVerticalLayoutUI*>::iterator it = m_AllLyt.begin();
	for (; it != m_AllLyt.end(); it++)
	{
		cListLyt->Remove((*it), true);
	}

	for (UINT i = 0; i < m_AllLyt.size(); i++)
	{
		cLyt = m_AllLyt[i];
		cLyt->SetFixedWidth(BUTTON_WIDTH);
		RECT rect;
		rect.left = rect.right = 10;
		rect.bottom = rect.top = 5;
		cLyt->SetPadding(rect);
		cListLyt->Add(cLyt);
	}
}

//把icon转成HBITMAP
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
		bi.biBitCount = 32;
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


HBITMAP TransparentImage(HBITMAP hBitmap)
{
	CImage Image;
	Image.Attach(hBitmap);
	int nPitch = Image.GetPitch(), nBPP = Image.GetBPP();
	LPBYTE lpBits = reinterpret_cast<LPBYTE>(Image.GetBits());

	for (int yPos = 0; yPos < Image.GetHeight(); yPos++)
	{
		LPBYTE lpBytes = lpBits + (yPos * nPitch);
		PDWORD lpLines = reinterpret_cast<PDWORD>(lpBytes);
		for (int xPos = 0; xPos < Image.GetWidth(); xPos++)
		{
			if (nBPP == 32 && lpLines[xPos] >> 24 != 0x000000FF)
			{
				lpLines[xPos] |= 0xFFFFFFFF;
			}
		}
	}

	return Image.Detach();
}


//把hbitmap文件保存成bmp图片
BOOL Launcher::SaveBmp(HBITMAP hBitmap, LPCSTR FileName)
{
	hBitmap = TransparentImage(hBitmap);
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
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL)     *     GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else  if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;

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
	fh = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
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
