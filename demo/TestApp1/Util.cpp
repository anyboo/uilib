#include "StdAfx.h"
#include "Util.h"
#include <gdiplus.h>
#include <memory>
#include <sstream>
#include <iostream>

HBITMAP CreateDesktopBitmap(HWND hWnd)
{
	HWND hWndDesktop = ::GetDesktopWindow();
	RECT rect;
	GetWindowRect(hWndDesktop, &rect);
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = rect.top = 0;

	HDC hDcDesktop = GetDC(hWndDesktop);
	HDC hDcMem = CreateCompatibleDC(hDcDesktop);
	HBITMAP hBmp = CreateCompatibleBitmap(hDcDesktop, rect.right, rect.bottom);
	HGDIOBJ hOld = SelectObject(hDcMem, hBmp);
	::BitBlt(hDcMem, 0, 0, rect.right, rect.bottom, hDcDesktop, 0, 0, SRCCOPY|CAPTUREBLT);
	::ReleaseDC(hWndDesktop, hDcDesktop);
	::DeleteDC(hDcMem);
	return hBmp;
}

HBITMAP CreateDesktopMaskBitmap(HWND hWnd)
{
	HDC hDCMem = CreateCompatibleDC(NULL);
	RECT rect;
	GetWindowRect(::GetDesktopWindow(), &rect);
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = rect.top = 0;

	HBITMAP hBmp = NULL;

	HDC hDC = GetDC(hWnd);
	hBmp = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
	ReleaseDC(hWnd, hDC);

	HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
	CRenderEngine::DrawColor(hDCMem, rect, 0x4F000000);
	SelectObject(hDCMem, hOld);

	DeleteObject(hDCMem);
	return hBmp;
}

HWND SmallestWindowFromCursor(RECT& rcWindow)
{	
	HWND hWnd, hTemp;
	POINT pt;
	::GetCursorPos(&pt);
	hWnd = ::ChildWindowFromPointEx(::GetDesktopWindow(), pt, CWP_SKIPDISABLED|CWP_SKIPINVISIBLE);
	if( hWnd != NULL )
	{
		hTemp = hWnd;  
		while (true) {
			::GetCursorPos(&pt);
			::ScreenToClient(hTemp, &pt);  
			hTemp = ::ChildWindowFromPointEx(hTemp, pt, CWP_SKIPINVISIBLE);  
			if (hTemp == NULL || hTemp == hWnd)  
				break;
			hWnd = hTemp;
		}  
		::GetWindowRect(hWnd, &rcWindow);
	}
	return hWnd;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;          // number of image encoders
	UINT size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1;  // Failure
	}

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1;  // Failure
	}

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

#include <ctime>
#include <iomanip>

void BitmapToJpg(HBITMAP hbmpImage, int width, int height, const std::wstring& path)
{
	Bitmap *p_bmp = Bitmap::FromHBITMAP(hbmpImage, NULL);

	CLSID pngClsid;
	int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
	if (result != -1)
		std::cout << "Encoder succeeded" << std::endl;
	else
		std::cout << "Encoder failed" << std::endl;

	time_t current;
	time(&current);
	auto tm = *std::localtime(&current);

	std::wstringstream ss;
	ss << path << L"screen" << std::put_time(&tm, L"%Y%m%d_%H%M%S") << L".jpg";
	p_bmp->Save(ss.str().c_str(), &pngClsid, NULL);

	delete p_bmp;
}