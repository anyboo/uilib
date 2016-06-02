#include "stdafx.h"
#include "Util.h"
#include "CanvasContainer.h"
#include <Poco/SingletonHolder.h>
#include "CaptureWnd.h"

using namespace Poco;

CCaptureWnd::CCaptureWnd()
:bClipChoiced(false), ClipDrawStringBuf(_T(""))
{
	ZeroMemory(&Desktop, sizeof(Desktop));
	ZeroMemory(&ClipPadding, sizeof(ClipPadding));
	ZeroMemory(&ClipBasePoint, sizeof(ClipBasePoint));
}

CCaptureWnd::~CCaptureWnd()
{
}

DUI_BEGIN_MESSAGE_MAP(CCaptureWnd, WindowImplBase)
//DUI_ON_MSGTYPE()
DUI_END_MESSAGE_MAP()

CDuiString CCaptureWnd::GetSkinFolder()
{
	return _T("SettingWnd");
}

CDuiString CCaptureWnd::GetSkinFile()
{
	return _T("Capture.xml");
}

LPCTSTR CCaptureWnd::GetWindowClassName(void) const
{
	return _T("CaptureWnd");
}

CControlUI* CCaptureWnd::CreateControl(LPCTSTR pstrClass)
{
	DUITRACE(pstrClass);

	if (_tcscmp(pstrClass, DUI_CTR_CANVASCONTAINER) == 0)
		return new CCanvasContainerUI(this);
	return NULL;
}

void CCaptureWnd::InitWindow()
{
	CPaintManagerUI& pm = m_PaintManager;
	Image = dynamic_cast<CControlUI*>(pm.FindControl(_T("desktopimage")));
	Mask = dynamic_cast<CControlUI*>(pm.FindControl(_T("desktopmask")));
	MaskBorder = dynamic_cast<CControlUI*>(pm.FindControl(_T("desktopmaskborder")));
	CanvasContainer = dynamic_cast<CContainerUI*>(pm.FindControl(_T("desktopcanvascontainer")));

	assert(Image || Mask || MaskBorder || CanvasContainer);

	::GetWindowRect(::GetDesktopWindow(), &Desktop);

	POINT p = { Desktop.left, Desktop.top };
	size_t width = Desktop.right - Desktop.left;
	size_t height = Desktop.bottom - Desktop.top;
	
	::SetWindowPos(GetHWND(), NULL, p.x, p.y, width, height, SWP_NOZORDER);

	HBITMAP hDesktopBmp = CreateDesktopBitmap(GetHWND());
	pm.AddImage(_T("desktopimage"), hDesktopBmp,width, height, false);
	HBITMAP hDesktopBmpMask = CreateDesktopMaskBitmap(GetHWND());
	pm.AddImage(_T("desktopimagemask"), hDesktopBmpMask, width, height, true);
	Image->SetAttribute(_T("pos"), _T("0, 0, 0, 0"));
	
	Image->SetBkImage(_T("desktopimage"));
	Mask->SetBkImage(_T("desktopimagemask"));
}

void CCaptureWnd::OnFinalMessage(HWND hWnd)
{
	CPaintManagerUI& pm = m_PaintManager;
	pm.RemoveAllImages(true);
	WindowImplBase::OnFinalMessage(hWnd);

	//delete this;
}

void CCaptureWnd::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}

LRESULT CCaptureWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_ESCAPE) {
			bClipChoiced = false;
			Close();
			return true;
		}

	}
	return false;
}

LRESULT CCaptureWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN: 
		OnLBUTTONDOWN(wParam, lParam);
		return 0;
		break;
	case WM_LBUTTONUP:
		OnLBUTTONUP(wParam, lParam);
		return 0;
		break;
	case WM_LBUTTONDBLCLK:
		OnLBUTTONDBLCLK(wParam, lParam);
		return 0;
		break;
	case WM_RBUTTONUP:
		OnRBUTTONUP(wParam, lParam);
		return 0;
		break;
	case WM_MOUSEMOVE:
		OnMOUSEMOVE(wParam, lParam);
		break;
	default:
		break;
	}
	LRESULT lRes = 0;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CCaptureWnd::OnLBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
	if (!bClipChoiced) {
		CanvasContainer->SetEnabled(false);
		ClipBasePoint.x = GET_X_LPARAM(lParam);
		ClipBasePoint.y = GET_Y_LPARAM(lParam);
	}
}

void CCaptureWnd::OnLBUTTONUP(WPARAM wParam, LPARAM lParam)
{
	if (!bClipChoiced) {
		bClipChoiced = true;
		MaskBorder->SetVisible(false);
		CanvasContainer->SetVisible(true);
		CanvasContainer->SetEnabled(true);
		SetClipPadding(ClipPadding);
	}
}

void CCaptureWnd::OnLBUTTONDBLCLK(WPARAM wParam, LPARAM lParam)
{
	// todo:完成截图，保存到剪贴板
	bClipChoiced = true;
	RECT rc = GetClipRect();
	DUITRACE(_T("ClipPadding -> left : %d, top : %d, right : %d, bottom : %d, width : %d, height : %d"), \
		rc.left, rc.top, rc.right, rc.bottom,
		rc.right - rc.left, rc.bottom - rc.top);
	Close();
}

void CCaptureWnd::OnRBUTTONUP(WPARAM wParam, LPARAM lParam)
{
	if (bClipChoiced) {
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		RECT rcClipRect = GetClipRect();
		if (::PtInRect(&rcClipRect, pt)) {
			/*::ClientToScreen(m_hWnd, &pt);
			::TrackPopupMenu(m_hEditMenu, TPM_TOPALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
			::PostMessage(m_hWnd, WM_NULL, 0, 0);*/
		}
		else {
			bClipChoiced = false;
			CanvasContainer->SetEnabled(false);
			::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
		}
	}
	else{
		Close();
	}
}

void CCaptureWnd::OnMOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
	if (!bClipChoiced) {
		if (::GetKeyState(VK_LBUTTON) < 0) { // LBUTTON IS PUSHED
			if (ClipBasePoint.x != GET_X_LPARAM(lParam) || ClipBasePoint.y != GET_Y_LPARAM(lParam)) {
				ClipPadding.left = min(GET_X_LPARAM(lParam), ClipBasePoint.x) - Desktop.left; if (ClipPadding.left < 0) ClipPadding.left = 0;
				ClipPadding.top = min(GET_Y_LPARAM(lParam), ClipBasePoint.y) - Desktop.top; if (ClipPadding.top < 0) ClipPadding.top = 0;
				ClipPadding.right = Desktop.right - max(GET_X_LPARAM(lParam), ClipBasePoint.x); if (ClipPadding.right < 0) ClipPadding.right = 0;
				ClipPadding.bottom = Desktop.bottom - max(GET_Y_LPARAM(lParam), ClipBasePoint.y); if (ClipPadding.bottom < 0) ClipPadding.bottom = 0;
				MaskBorder->SetVisible(false);
				CanvasContainer->SetVisible(true);
				SetClipPadding(ClipPadding);
			}
		}
		else {
			RECT rc;
			HWND hWnd;
			::EnableWindow(m_hWnd, FALSE);
			hWnd = SmallestWindowFromCursor(rc);
			::EnableWindow(m_hWnd, TRUE);
			::SetFocus(m_hWnd);
			rc.left = rc.left - Desktop.left; if (rc.left < 0) rc.left = 0;
			rc.top = rc.top - Desktop.top; if (rc.top < 0) rc.top = 0;
			rc.right = Desktop.right - rc.right; if (rc.right < 0) rc.right = 0;
			rc.bottom = Desktop.bottom - rc.bottom; if (rc.bottom < 0) rc.bottom = 0;
			if (rc.left != ClipPadding.left || rc.right != ClipPadding.right || rc.top != ClipPadding.top || rc.bottom != ClipPadding.bottom) {
				SetClipPadding(rc);
				MaskBorder->SetVisible(true);
				CanvasContainer->SetVisible(false);
			}
		}
	}
}

void CCaptureWnd::SetClipPadding(RECT rc) 
{
	ClipPadding = rc;
	ClipDrawStringBuf.Format(_T("file='desktopimagemask' scale9='%d,%d,%d,%d' hole='true'"), \
		ClipPadding.left, ClipPadding.top, ClipPadding.right, ClipPadding.bottom);

	Mask->SetBkImage(ClipDrawStringBuf);
	MaskBorder->SetPos(GetClipRect());
	CanvasContainer->SetPos(GetCanvasContainerRect());
}

RECT CCaptureWnd::GetWindowRect() const
{
	return Desktop;
}

RECT CCaptureWnd::GetClipPadding() const
{
	return ClipPadding;
}

bool CCaptureWnd::IsClipChoiced() const
{
	return bClipChoiced;
}

RECT CCaptureWnd::GetClipRect() const
{
	RECT rc = ClipPadding;
	rc.left = rc.left + Desktop.left;
	rc.top = rc.top + Desktop.top;
	rc.right = Desktop.right - rc.right;
	rc.bottom = Desktop.bottom - rc.bottom;
	return rc;
}

RECT CCaptureWnd::GetCanvasContainerRect() const
{
	RECT rcInset = CanvasContainer->GetInset();
	RECT rc = ClipPadding;
	rc.left = rc.left + Desktop.left - rcInset.left;
	rc.top = rc.top + Desktop.top - rcInset.left;
	rc.right = Desktop.right - rc.right + rcInset.left;
	rc.bottom = Desktop.bottom - rc.bottom + rcInset.left;
	return rc;
}