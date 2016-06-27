#include "stdafx.h"
#include "Vendor.h"


CVendor::CVendor()
:ppm(nullptr)
{	
}

CVendor::~CVendor()
{
}

DUI_BEGIN_MESSAGE_MAP(CVendor, CNotifyPump)

DUI_END_MESSAGE_MAP()

void CVendor::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	assert(pPaintMgr);
	ppm = pPaintMgr;
}
