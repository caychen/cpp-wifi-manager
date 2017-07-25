// WIFIMAIN.cpp : implementation file
//

#include "stdafx.h"
#include "WifiShared.h"
#include "WIFIMAIN.h"


// WIFIMAIN

IMPLEMENT_DYNAMIC(WIFIMAIN, CWnd)

WIFIMAIN::WIFIMAIN()
: m_bEnable(FALSE)
, m_bStart(FALSE)
{

}

WIFIMAIN::~WIFIMAIN()
{
	if (m_bStart)
		StopHostedNetWork();
		//DisableHostedNetWork();

	//关闭Wlan句柄
	if (m_hClient)
		WlanCloseHandle(m_hClient, NULL);
}

BEGIN_MESSAGE_MAP(WIFIMAIN, CWnd)
END_MESSAGE_MAP()

// WIFIMAIN message handlers


/*The WlanOpenHandle function opens a connection to the server.

	DWORD WINAPI WlanOpenHandle(
		_In_        DWORD dwClientVersion,
		_Reserved_  PVOID pReserved,
		_Out_       PDWORD pdwNegotiatedVersion,
		_Out_       PHANDLE phClientHandle
	);
*/


/*The WlanHostedNetworkSetProperty function sets static properties of the wireless Hosted Network. 

	DWORD WINAPI WlanHostedNetworkSetProperty(
		_In_        HANDLE hClientHandle,
		_In_        WLAN_HOSTED_NETWORK_OPCODE OpCode,
		_In_        DWORD dwDataSize,
		_In_        PVOID pvData,
		_Out_opt_   PWLAN_HOSTED_NETWORK_REASON pFailReason,
		_Reserved_  PVOID pvReserved
	);
*/
BOOL WIFIMAIN::Init()
{
	//调用WlanOpenHandled得到一个连接hClient句柄
	DWORD dwError = 0;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	dwResult = WlanOpenHandle(WLAN_API_VERSION, NULL, &dwCurVersion, &m_hClient);
	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return FALSE;
	}

	//设置承载网络的一些属性
	BOOL bIsEnable = TRUE;
	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	dwResult = WlanHostedNetworkSetProperty(
		m_hClient,
		wlan_hosted_network_opcode_enable,
		sizeof(BOOL),
		&bIsEnable,
		&dwFailReason,
		NULL);

	WLAN_HOSTED_NETWORK_REASON reason = 
		(WLAN_HOSTED_NETWORK_REASON)dwFailReason;
	if (reason == wlan_hosted_network_reason_elevation_required)
	{
		AfxMessageBox(_T("请使用管理员身份运行！"));
		return FALSE;
	}

	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return FALSE;
	}
	return TRUE;
}

/*The WlanHostedNetworkForceStart function transitions the wireless Hosted Network to the wlan_hosted_network_active state 
	without associating the request with the application's calling handle. 

	DWORD WINAPI WlanHostedNetworkForceStart(
		_In_        HANDLE hClientHandle,
		_Out_opt_   PWLAN_HOSTED_NETWORK_REASON pFailReason,
		_Reserved_  PVOID pvReserved
	);
*/
BOOL WIFIMAIN::EnableHostedNetWork()//启用无线承载网络
{
	DWORD dwResult = 0;
	DWORD dwError = 0;
	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	dwResult = WlanHostedNetworkForceStart(m_hClient, &dwFailReason, NULL);
	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return FALSE;
	}
	m_bEnable = TRUE;
	m_bStart = TRUE;
	return TRUE;
}

/*The WlanHostedNetworkForceStop function transitions the wireless Hosted Network to the wlan_hosted_network_idle 
	without associating the request with the application's calling handle. 

	DWORD WINAPI WlanHostedNetworkForceStop(
		_In_        HANDLE hClientHandle,
		_Out_opt_   PWLAN_HOSTED_NETWORK_REASON pFailReason,
		_Reserved_  PVOID pvReserved
	);
*/
BOOL WIFIMAIN::DisableHostedNetWork()//禁用无线承载网络,类似启动
{
	DWORD dwResult = 0;
	DWORD dwError = 0;
	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	dwResult = WlanHostedNetworkForceStop(m_hClient, &dwFailReason, NULL);
	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return FALSE;
	}
	m_bEnable = FALSE;
	m_bStart = FALSE;
	return TRUE;
}

/*The WlanHostedNetworkStartUsing function starts the wireless Hosted Network.

	DWORD WINAPI WlanHostedNetworkStartUsing(
		_In_        HANDLE hClientHandle,
		_Out_opt_   PWLAN_HOSTED_NETWORK_REASON pFailReason,
		_Reserved_  PVOID pvReserved
	);
*/
BOOL WIFIMAIN::StartHostedNetWork()//连接无线网络
{
	DWORD dwResult = 0;
	DWORD dwError = 0;
	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	dwResult = WlanHostedNetworkStartUsing(m_hClient, &dwFailReason, NULL);
	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return FALSE;
	}
	m_bStart = TRUE;
	return TRUE;
}

/*The WlanHostedNetworkStopUsing function stops the wireless Hosted Network.

	DWORD WINAPI WlanHostedNetworkStopUsing(
		_In_        HANDLE hClientHandle,
		_Out_opt_   PWLAN_HOSTED_NETWORK_REASON pFailReason,
		_Reserved_  PVOID pvReserved
	);
*/
BOOL WIFIMAIN::StopHostedNetWork()//断开无线网络
{
	DWORD dwResult = 0;
	DWORD dwError = 0;
	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	dwResult = WlanHostedNetworkStopUsing(m_hClient, &dwFailReason, NULL);
	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return FALSE;
	}
	m_bStart = FALSE;
	//DisableHostedNetWork();
	return TRUE;
}

BOOL WIFIMAIN::GetIsEnable()//是否使能
{
	return m_bEnable;
}

BOOL WIFIMAIN::GetIsStart()//是否开启
{
	return m_bStart;
}

int WIFIMAIN::Resume()
{
	DWORD dwError = 0;
	DWORD dwResult = 0;
	BOOL bIsEnable = FALSE;

	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	dwResult = WlanHostedNetworkSetProperty(
		m_hClient,
		wlan_hosted_network_opcode_enable,
		sizeof(BOOL),
		&bIsEnable,
		&dwFailReason,
		NULL);

	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return -1;
	}
	return 0;
}

int WIFIMAIN::GetPeerNumber()
{
	DWORD dwResult = 0;
	DWORD dwError = 0;
	PWLAN_HOSTED_NETWORK_STATUS pWlanHostedNetworkStatus = NULL;//必须是null

	dwResult = WlanHostedNetworkQueryStatus(
		m_hClient,
		&pWlanHostedNetworkStatus,
		NULL);
	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return -1;
	}
	return pWlanHostedNetworkStatus->dwNumberOfPeers;
}

void WIFIMAIN::SetWifiKey(CString key)
{
	UCHAR keyBuf[100] = { 0 };
#ifdef _UNICODE
    WideCharToMultiByte(CP_ACP,
		0,
		key.GetBuffer(0),
		key.GetLength(),
		(LPSTR)keyBuf,
		100,
		NULL,
		NULL
	);
#else
	memcpy(keyBuf, key.GetBuffer(0), strlen(key.GetBuffer(0)));
#endif

	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	DWORD dwResult = 0;
	DWORD dwError = 0;

	dwResult = WlanHostedNetworkSetSecondaryKey(m_hClient,
		strlen((const char*)keyBuf) + 1,
		keyBuf,
		TRUE,
		FALSE,
		&dwFailReason,
		NULL);

	WLAN_HOSTED_NETWORK_REASON Reason =
		(WLAN_HOSTED_NETWORK_REASON)dwFailReason;


	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return ;
	}
}

void WIFIMAIN::SetWifiName(CString Name)
{
	UCHAR szName[64] = { 0 };

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,
		0,
		Name.GetBuffer(0),
		Name.GetLength(),
		(LPSTR)szName,
		100,
		NULL,
		NULL
		);
#else
	memcpy(szName, Name.GetBuffer(0), strlen(Name.GetBuffer(0)));
#endif

	WLAN_HOSTED_NETWORK_REASON dwFailReason;
	WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS settings;
	DWORD dwResult = 0;
	DWORD dwError = 0;
	
	memset(&settings, 0, sizeof(WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS));

	memcpy(settings.hostedNetworkSSID.ucSSID, szName, strlen((LPSTR)szName));
	settings.hostedNetworkSSID.uSSIDLength = strlen((LPSTR)szName);
	settings.dwMaxNumberOfPeers = 20;

	dwResult = WlanHostedNetworkSetProperty(
		m_hClient,
		wlan_hosted_network_opcode_connection_settings,
		sizeof(WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS),
		&settings,
		&dwFailReason,
		NULL);

	if (ERROR_SUCCESS != dwResult)
	{
		dwError = GetLastError();
		AfxMessageBox(dwError);
		return ;
	}
}

void WIFIMAIN::SetStart(BOOL bStart)
{
	m_bStart = bStart;
}