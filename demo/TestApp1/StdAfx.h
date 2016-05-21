
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
#define CTR_ENCODE			(_T("btencode"))
#define CTR_VOICE			(_T("btvoice"))
#define CTR_ABOUT			(_T("btabout"))
#define CTR_SETTING			(_T("savebtn"))
#define CTR_MIN				(_T("minbtn"))
#define CTR_CLOSE			(_T("closebtn"))

void trace(TNotifyUI& msg);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
