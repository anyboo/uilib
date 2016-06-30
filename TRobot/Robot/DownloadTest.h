#pragma once

#include "JxjVendor.h"

class CDownloadTest
{
public:
	CDownloadTest();
	~CDownloadTest();

	void Test();
	static void CallBack();

	CJxjVendor jxjVendor;
	int m_id;
};

