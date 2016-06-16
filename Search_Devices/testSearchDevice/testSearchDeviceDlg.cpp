
// testSearchDeviceDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CtestSearchDeviceDlg �Ի���



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


// CtestSearchDeviceDlg ��Ϣ�������

BOOL CtestSearchDeviceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T(""));
	m_list.InsertColumn(1, _T("SDK"));
	m_list.InsertColumn(2, _T("IP"));
	m_list.InsertColumn(3, _T("�˿�"));
	m_list.InsertColumn(4, _T("MAC"));	
	m_list.InsertColumn(4, _T("��������"));
	m_list.SetColumnWidth(0, 30);
	m_list.SetColumnWidth(1, 150);
	m_list.SetColumnWidth(2, 150);
	m_list.SetColumnWidth(3, 100);
	m_list.SetColumnWidth(4, 150);
	m_list.SetColumnWidth(5, 150);
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestSearchDeviceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestSearchDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestSearchDeviceDlg::OnBnClickedButton1()
{

	
	m_list.DeleteAllItems();

	this->m_list.SetRedraw(FALSE);

	SearchDeviceInterface *pSearch = CreateSearchObj();
	//����DLLĿ¼·��
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
				m_list.SetItemText(nIndex, 1, "��");
				break;
			case DZP_SDK:
				m_list.SetItemText(nIndex, 1, "������");
				break;
			case JXJ_SDK:
				m_list.SetItemText(nIndex, 1, "���Ž�");
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
