#include "stdafx.h"
#include "MyHandle.h"
#include "MenuWnd.h"

#define BUTTON_WIDTH					48
#define BUTTON_HEIGHT					48
#define LABLE_WIDTH						48
#define LABLE_HEIGHT					15
#define FONT_SIZE						12
#define BORD_WIDTH						35
#define LYT_WIDTH						135
#define LYT_HEIGHT						110
#define BMPBITCOUNT						32

CMyHandle::CMyHandle()
:m_BmpNameHead(_T("A"))
{
}


CMyHandle::~CMyHandle()
{
}


STDSTRING CMyHandle::GetFileSavePath()
{
#ifdef UNICODE
	wchar_t tmpPath[MAX_PATH] = { 0 };
#else
	char tmpPath[MAX_PATH] = { 0 };
#endif
	GetTempPath(MAX_PATH, tmpPath);
	STDSTRING strPath(tmpPath);
	return strPath;
}

void CMyHandle::PopMenu(TNotifyUI& msg, HWND& m_hWnd)
{
	CMenuWnd* pMenu = new CMenuWnd();
	if (pMenu == NULL) { return; }
	POINT pt = { msg.ptMouse.x, msg.ptMouse.y };
	m_xyPos = pt;
	::ClientToScreen(m_hWnd, &pt);
	pMenu->Init(msg.pSender, pt);
}

void CMyHandle::DeleteLyt(CPaintManagerUI& mpm, vector<LayOut_Info>& AllLyt)
{
	CHorizontalLayoutUI* cListLyt = static_cast<CHorizontalLayoutUI*>(mpm.FindControl(_T("ListLayout")));
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	int xPos = m_xyPos.x;
	int yPos = m_xyPos.y;

	UINT n = xPos / LYT_WIDTH + (yPos - BORD_WIDTH) / LYT_HEIGHT * 4;
	if (n > AllLyt.size() || AllLyt.size() == 0)
		return;
	cLyt = AllLyt[n].Layout;
	cListLyt->Remove(cLyt);
	AllLyt.erase(AllLyt.begin() + n);
}

void CMyHandle::OpenExeFile(int xPos, int yPos, vector<LayOut_Info>& AllLyt)
{
	UINT n = xPos / LYT_WIDTH + (yPos - BORD_WIDTH) / LYT_HEIGHT * 4;
	if (AllLyt.size() == 0 || AllLyt.size() - 1 < n)
		return;
	if (xPos < 5 || yPos < BORD_WIDTH || n >AllLyt.size() - 1 || xPos - 16 < 0 || (xPos - 16) % LYT_WIDTH > 95
		|| yPos - BORD_WIDTH < 0 || (yPos - BORD_WIDTH) % LYT_HEIGHT > 90)
	{
		return;
	}
	DUITRACE(_T("%s"), AllLyt[n].FilePath.c_str());
	HINSTANCE hIns = ShellExecute(NULL, _T("Open"), AllLyt[n].FilePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	int dret = (int)hIns;
	if (dret < 32)
		MessageBox(NULL, _T("无法打开"), _T("提示"), MB_OK);
}

void CMyHandle::OnMouseMove(int xPos, int yPos, vector<LayOut_Info>& AllLyt)
{
	UINT n = xPos / LYT_WIDTH + (yPos - BORD_WIDTH) / LYT_HEIGHT * 4;
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	if (n >= AllLyt.size()) return;
	cLyt = AllLyt[n].Layout;
	cLyt->SetBkColor(0xFFFF9999);
}

void CMyHandle::OnMouseLeave(vector<LayOut_Info>& AllLyt)
{
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	for (UINT j = 0; j < AllLyt.size(); j++)
	{
		cLyt = AllLyt[j].Layout;
		cLyt->SetBkColor(NULL);
	}
}

void CMyHandle::AddLayout(int nPosX, int nPosY, STDSTRING pFileName, STDSTRING strPath, CPaintManagerUI& CPaint, vector<LayOut_Info>& AllLyt)
{
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	InitLayOut(cLyt, pFileName, strPath);
	Push_LayOut(nPosX, nPosY, cLyt, strPath, AllLyt);
	ShowLayOut(CPaint, AllLyt);
}

void CMyHandle::InitLayOut(CNewVerticalLayoutUI* cLyt, STDSTRING pFileName, STDSTRING strPath)
{
	CNewButtonUI* cBtn = new CNewButtonUI;
	CLabelUI* Lab = new CLabelUI;
	cLyt->Add(cBtn);
	cLyt->Add(Lab);
	cBtn->SetContextMenuUsed(true);
	cBtn->SetFixedHeight(BUTTON_HEIGHT);
	cBtn->SetFixedWidth(BUTTON_WIDTH);
//	STDSTRING ImagePath = WriteablePath + pFileName;
	
	STDSTRING FileName = GetFileSavePath() + pFileName;
	cBtn->SetBkImage(FileName.c_str());
	cBtn->SetToolTip(strPath.c_str());

	RECT r;
	r.left = 25;	r.right = 22;
	r.bottom = 0;	r.top = 10;
	cBtn->SetPadding(r);

	STDSTRING path, displayName;
	path = strPath;
	displayName = path.substr(path.find_last_of(_T('\\')) + 1);

	Lab->SetText(displayName.c_str());
	Lab->SetFont(FONT_SIZE);
	Lab->SetMultiLine(true);
	RECT rect;
	rect.left = 25;
	rect.right = 22;
	rect.bottom = 40;
	rect.top = 5;
	Lab->SetPadding(rect);
	Lab->SetToolTip(strPath.c_str());
	Lab->SetMultiLine(true);
}

void CMyHandle::Push_LayOut(int xPos, int yPos, CNewVerticalLayoutUI* cLyt, STDSTRING strPath, vector<LayOut_Info>& AllLyt)
{
	
	LayOut_Info Lyt_info = { 0 };
	Lyt_info.Layout = cLyt;
	Lyt_info.FilePath = strPath;

	UINT Xnum = xPos / LYT_WIDTH + 1;
	UINT Ynum = (yPos - BORD_WIDTH) / LYT_HEIGHT;
	UINT n = Ynum * 4 + Xnum;
	if (n > AllLyt.size()){
		AllLyt.push_back(Lyt_info);
	}
	else{
		AllLyt.insert(AllLyt.begin() + n - 1, Lyt_info);
	}
}

void CMyHandle::ShowLayOut(CPaintManagerUI& CPaint, vector<LayOut_Info>& AllLyt)
{
	CTileLayoutUI* cListLyt = static_cast<CTileLayoutUI*>(CPaint.FindControl(_T("ListLayout")));
	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;

	for (auto& var : AllLyt)
	{
		cListLyt->Remove(var.Layout, true);

		cLyt = var.Layout;		cLyt->SetContextMenuUsed(true);
		cLyt->SetToolTip(var.FilePath.c_str());

		RECT rect;
		rect.left = rect.right = 10;
		rect.bottom = rect.top = 5;
		cLyt->SetPadding(rect);
		cListLyt->Add(cLyt);
	}
}

void CMyHandle::LoadIcon(STDSTRING strPath, vector<LayOut_Info>& AllLyt, CPaintManagerUI& CPaint)
{
	STDSTRING BmpFileName;
	m_BmpNameHead[0] += 1;
	BmpFileName = m_BmpNameHead + _T("tmp.bmp");

	HICON hIcon = QueryFileIcon(strPath);
	HBITMAP IconHbmp = IconToBitmap(hIcon);
	SaveBmp(IconHbmp, BmpFileName);


	CNewVerticalLayoutUI* cLyt = new CNewVerticalLayoutUI;
	InitLayOut(cLyt, BmpFileName, strPath);

	LayOut_Info Lyt_info = { 0 };
	Lyt_info.Layout = cLyt;
	Lyt_info.FilePath = strPath;
	AllLyt.push_back(Lyt_info);
	ShowLayOut(CPaint, AllLyt);
}

HICON CMyHandle::QueryFileIcon(STDSTRING lpszFilePath)
{
	HICON hIcon = NULL;
	SHFILEINFO FileInfo;
	DWORD_PTR dwRet = ::SHGetFileInfo(lpszFilePath.c_str(), 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
	if (dwRet)
	{
		hIcon = FileInfo.hIcon;
	}
	return hIcon;
}

HBITMAP CMyHandle::IconToBitmap(HICON hIcon, SIZE* pTargetSize)
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

BOOL CMyHandle::SaveBmp(HBITMAP hBitmap, STDSTRING FileName)
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
	STDSTRING strSavePath = GetFileSavePath() + FileName;
	fh = CreateFile(strSavePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
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
