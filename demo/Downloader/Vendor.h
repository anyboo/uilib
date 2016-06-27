#pragma once
#include <DuiLib/UIlib.h>

class CVendor
	:public CNotifyPump
{
public:
	CVendor();
	~CVendor();

	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	CListContainerElementUI* ShowVendor(bool IsOnLine);
	void AddVendorList();

	DUI_DECLARE_MESSAGE_MAP();
	
	

private:
	CPaintManagerUI* ppm;

};
