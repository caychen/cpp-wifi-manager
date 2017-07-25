
// WifiSharedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WifiShared.h"
#include "WifiSharedDlg.h"
#include "afxdialogex.h"
#include <VersionHelpers.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CWifiSharedDlg dialog


CWifiSharedDlg::CWifiSharedDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWifiSharedDlg::IDD, pParent)
	, m_bIsOK(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CWifiSharedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWifiSharedDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_STARTORCLOSE, &CWifiSharedDlg::OnBtnStartOrClose)
	ON_EN_CHANGE(IDC_WIFI_NAME, &CWifiSharedDlg::OnEnChangeWifiName)
	ON_EN_CHANGE(IDC_WIFI_PASSWORD, &CWifiSharedDlg::OnEnChangeWifiPassWord)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CWifiSharedDlg::OnBtnQuit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MINIMIZE, &CWifiSharedDlg::OnBnClickedBtnMinimize)
	ON_MESSAGE(WM_NOTIFYICON, &CWifiSharedDlg::OnNotifyIcon)
	ON_COMMAND(ID_MENU_SHOW, &CWifiSharedDlg::OnMenuShow)
	ON_COMMAND(ID_MENU_EXIT, &CWifiSharedDlg::OnMenuExit)
END_MESSAGE_MAP()


// CWifiSharedDlg message handlers

BOOL CWifiSharedDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//检查操作系统是否在Win7以上
	/*if (!IsWindows7OrGreater())
	{
		MessageBox(_T("请在Windows7版本以上的操作系统上运行！"), _T("错误信息"), MB_ICONWARNING);
		OnCancel();
		return FALSE;
	}*/

	//功能同上
	if (!IsWindowsVersionOrGreater(6, 0, 0))//Windows7主版本号为6，次版本号为0，sp为0
	{
		MessageBox(_T("请在Windows7版本以上的操作系统上运行！"), _T("错误信息"), MB_ICONWARNING);
		OnCancel();
		return FALSE;
	}

	if (!Wifi.Init())
	{
		//OnCancel();
		//return FALSE;
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWifiSharedDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWifiSharedDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWifiSharedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWifiSharedDlg::OnBtnStartOrClose()
{
	// TODO: Add your control notification handler code here

	CString WifiName;
	CString WifiPassWord;

	UpdateData();

	if (Wifi.GetIsStart())
	{
		if (Wifi.StopHostedNetWork())
		{
			SetDlgItemText(IDC_BTN_STARTORCLOSE, _T("开启"));
			GetDlgItem(IDC_WIFI_NAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_WIFI_PASSWORD)->EnableWindow(TRUE);
		}
		else
			MessageBox(_T("命令执行失败"));
	}
	else
	{
		GetDlgItemText(IDC_WIFI_NAME, WifiName);
		GetDlgItemText(IDC_WIFI_PASSWORD, WifiPassWord);

		Wifi.SetWifiName(WifiName);
		Wifi.SetWifiKey(WifiPassWord);

		if (m_bIsOK && Wifi.StartHostedNetWork())
		{
			SetDlgItemText(IDC_BTN_STARTORCLOSE, _T("关闭"));
			GetDlgItem(IDC_WIFI_NAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_WIFI_PASSWORD)->EnableWindow(FALSE);
			SetTimer(TimerClock, 5000, NULL);
		}
		else
		{
		}
	}
}

void CWifiSharedDlg::OnEnChangeWifiName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	CString WifiName;
	GetDlgItemText(IDC_WIFI_NAME, WifiName);

	int len = WifiName.GetLength();
	if (len <= 0)
	{
		m_bIsOK = FALSE;
		SetDlgItemText(IDC_MSGTEXT, _T("请设置网络名称"));
	}
	else
	{
		m_bIsOK = TRUE;
		SetDlgItemText(IDC_MSGTEXT, _T(""));
	}
}

void CWifiSharedDlg::OnEnChangeWifiPassWord()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	CString WifiPassWord;
	GetDlgItemText(IDC_WIFI_PASSWORD, WifiPassWord);

	int len = WifiPassWord.GetLength();
	if (len > 63 || len < 8)
	{
		m_bIsOK = false;
		SetDlgItemText(IDC_MSGTEXT, _T("密码位数应该大于8位小于63位"));

	}
	else
	{
		m_bIsOK = true;
		SetDlgItemText(IDC_MSGTEXT, _T(""));					//清空内容
	}
}

void CWifiSharedDlg::OnBtnQuit()
{
	// TODO: Add your control notification handler code here
	if (Wifi.GetIsStart())
	{
		Wifi.StopHostedNetWork();
	}

	CDialog::OnCancel();
}

void CWifiSharedDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	SetDlgItemInt(IDC_ONLINE_NUMBERS, Wifi.GetPeerNumber());

	CDialogEx::OnTimer(nIDEvent);
}

void CWifiSharedDlg::OnBnClickedBtnMinimize()
{
	// TODO: Add your control notification handler code here
	ShowTask();
}

LRESULT CWifiSharedDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	CMenu menu, *pMenu = NULL;
	CPoint pt;

	switch (lParam)
	{
	case WM_RBUTTONUP:
		menu.LoadMenu(IDR_MENU1);
		pMenu = menu.GetSubMenu(0);
		GetCursorPos(&pt);
		SetForegroundWindow();
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
		break;

	case  WM_LBUTTONDBLCLK:
		ShowWindow(SW_RESTORE);
		SetForegroundWindow();
		ShowTask(FALSE);
		break;
	}
	return NULL;
}

BOOL CWifiSharedDlg::ShowTask(BOOL bAdd)
{
	BOOL bRet = FALSE;
	NOTIFYICONDATA NotifyIcon;
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	NotifyIcon.hWnd = m_hWnd;//GetSafeHwnd();
	NotifyIcon.uID = IDI_ICON1;

	if (bAdd)
	{
		NotifyIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		NotifyIcon.uCallbackMessage = WM_NOTIFYICON;			//NIF_MESSAGE
		NotifyIcon.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);//NIF_ICON
		_tcscpy_s(NotifyIcon.szTip, _T("Wifi Share"));			//NIF_TIP

		ShowWindow(SW_HIDE);
		bRet = Shell_NotifyIcon(NIM_ADD, &NotifyIcon);
	}
	else
	{
		ShowWindow(SW_SHOW);
		SetForegroundWindow();//设置前景窗口
		bRet = Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
	}
	return bRet;
}

void CWifiSharedDlg::OnMenuShow()
{
	// TODO: Add your command handler code here
	ShowWindow(SW_RESTORE);
	ShowTask(FALSE);
}


void CWifiSharedDlg::OnMenuExit()
{
	// TODO: Add your command handler code here
	ShowTask(FALSE);
	OnCancel();
}
