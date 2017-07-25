
// WifiSharedDlg.h : header file
//

#pragma once
#include "WIFIMAIN.h"
#include "resource.h"
#define  WM_NOTIFYICON	(WM_USER + 1)

#define TimerClock  1
// CWifiSharedDlg dialog
class CWifiSharedDlg : public CDialogEx
{
// Construction
public:
	CWifiSharedDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WIFISHARED_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBtnStartOrClose();
	afx_msg void OnEnChangeWifiName();
	afx_msg void OnEnChangeWifiPassWord();
	afx_msg void OnBtnQuit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnMinimize();
	BOOL ShowTask(BOOL bAdd = TRUE);

public:
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);

private:
	WIFIMAIN Wifi;
	BOOL m_bIsOK;
public:
	afx_msg void OnMenuShow();
	afx_msg void OnMenuExit();
};
