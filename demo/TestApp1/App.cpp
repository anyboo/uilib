// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "MainWnd.h"
#include "resource.h"
#include <Poco/NamedMutex.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/Message.h>

using Poco::Logger;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::FileChannel;
using Poco::Message;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	Poco::NamedMutex unique("SRTool");
	if (!unique.tryLock())
		return 0;

	AutoPtr<PatternFormatter> pPatternFormatter2(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"));
	AutoPtr<FormattingChannel> pFCFile(new FormattingChannel(pPatternFormatter2));
	AutoPtr<FileChannel> pFileChannel(new FileChannel("SRTool.log"));
	pFCFile->setChannel(pFileChannel);
	pFCFile->open();

	Logger& fileLogger = Logger::create("FileLogger", pFCFile, Message::PRIO_INFORMATION);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CMainWnd Frame;
	Frame.Create(NULL, NULL, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	Frame.SetIcon(IDI_ICON1);
	Frame.CenterWindow();
	Frame.ShowWindow(true);
	
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	Logger::get("FileLogger").information("end");
	Logger::shutdown();

	return 0;
}
