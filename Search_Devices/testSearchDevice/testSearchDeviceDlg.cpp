
// testSearchDeviceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testSearchDevice.h"
#include "testSearchDeviceDlg.h"
#include "afxdialogex.h"

#include "SearchDevicesImpl.h"

#pragma comment(lib, "SearchDevices.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestSearchDeviceDlg 对话框



CtestSearchDeviceDlg::CtestSearchDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestSearchDeviceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestSearchDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CtestSearchDeviceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestSearchDeviceDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CtestSearchDeviceDlg 消息处理程序

BOOL CtestSearchDeviceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T(""));
	m_list.InsertColumn(1, _T("SDK"));
	m_list.InsertColumn(2, _T("IP"));
	m_list.InsertColumn(3, _T("端口"));
	m_list.InsertColumn(4, _T("MAC"));	
	m_list.InsertColumn(4, _T("子网掩码"));
	m_list.SetColumnWidth(0, 30);
	m_list.SetColumnWidth(1, 150);
	m_list.SetColumnWidth(2, 150);
	m_list.SetColumnWidth(3, 100);
	m_list.SetColumnWidth(4, 150);
	m_list.SetColumnWidth(5, 150);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestSearchDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestSearchDeviceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestSearchDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestSearchDeviceDlg::OnBnClickedButton1()
{

	
	m_list.DeleteAllItems();

	this->m_list.SetRedraw(FALSE);

	SearchDeviceInterface *pSearch = CreateSearchObj();
	//设置DLL目录路径
	pSearch->SetDllPath("factorys\\");

	int nDeviceLen = sizeof(NET_DEVICE_INFO);
	char *pDevice = new char[nDeviceLen * 512];
	int nRet = 0;
	memset(pDevice, 0, nDeviceLen * 512);
	bool bRet = pSearch->GetDevice(pDevice, nDeviceLen * 512, &nRet);

	

	if (bRet)
	{
		int i;
		for (i = 0; i < (nRet / nDeviceLen); i++)
		{
			char sTmp[20] = { 0 };
			sprintf_s(sTmp, "%d", i + 1);
			int nIndex = m_list.InsertItem(i, sTmp);			
			
			NET_DEVICE_INFO *ndiTmp = (NET_DEVICE_INFO *)(pDevice + i*nDeviceLen);
			switch (ndiTmp->nSDKType)
			{
			case DH_SDK:
				m_list.SetItemText(nIndex, 1, "大华");
				break;
			case DZP_SDK:
				m_list.SetItemText(nIndex, 1, "迪智普");
				break;
			case JXJ_SDK:
				m_list.SetItemText(nIndex, 1, "佳信捷");
				break;
			default:
				break;
			}

			m_list.SetItemText(nIndex, 2, ndiTmp->szIp);

			memset(sTmp, 0, 20);
			sprintf_s(sTmp, "%d", ndiTmp->nPort);
			m_list.SetItemText(nIndex, 3, sTmp);

			m_list.SetItemText(nIndex, 4, ndiTmp->szSubmask);

			m_list.SetItemText(nIndex, 5, ndiTmp->szMac);
		}
	}
	DestroySearchObj(pSearch);
	delete pDevice;
	this->m_list.SetRedraw(TRUE);
}
