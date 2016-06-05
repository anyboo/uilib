
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>

#include "DuiLib\UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

#define CTR_RECORD			(_T("btlz"))
#define CTR_SCREEN_CAPTURE	(_T("btpmbh"))
#define CTR_AREA_RECORD		(_T("btlzqy"))
#define CTR_LOCATION		(_T("btopen"))
#define CTR_ENCODE_MP4		(_T("btencodemp4"))
#define CTR_ENCODE_FLV		(_T("btencodeflv"))
#define CTR_VOICE			(_T("btvoice"))
#define CTR_MUTE			(_T("btmute"))
#define CTR_ABOUT			(_T("btabout"))
#define CTR_SETTING			(_T("savebtn"))
#define CTR_MIN				(_T("minbtn"))
#define CTR_CLOSE			(_T("closebtn"))
#define CTR_FOLDER			(_T("folder"))
#define CTR_ENCODING		(_T("encoding")) 

#define  DUI_CTR_CANVASCONTAINER                 (_T("CanvasContainer"))

void trace(TNotifyUI& msg);

#define TRACE (DUITRACE(_T("<%s>  %s:%d"), __FUNCTION__, __FILE__, __LINE__));

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
