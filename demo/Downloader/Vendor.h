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

	STDSTRING intToString(int num);
	void AddVendorList();
	void AddChannelsList(int CurSel);
	CListContainerElementUI* AddChannels();

	DUI_DECLARE_MESSAGE_MAP();
	
	

private:
	CPaintManagerUI* ppm;
	int				m_ContListSel;
};
