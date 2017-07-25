#pragma once
#include <wlanapi.h>
#pragma comment(lib, "Wlanapi.lib")

/*错误信息的原因
typedef enum _WLAN_HOSTED_NETWORK_REASON {
	wlan_hosted_network_reason_success = 0,
	wlan_hosted_network_reason_unspecified,
	wlan_hosted_network_reason_bad_parameters,
	wlan_hosted_network_reason_service_shutting_down,
	wlan_hosted_network_reason_insufficient_resources,
	wlan_hosted_network_reason_elevation_required,
	wlan_hosted_network_reason_read_only,
	wlan_hosted_network_reason_persistence_failed,
	wlan_hosted_network_reason_crypt_error,
	wlan_hosted_network_reason_impersonation,
	wlan_hosted_network_reason_stop_before_start,
	wlan_hosted_network_reason_interface_available,
	wlan_hosted_network_reason_interface_unavailable,
	wlan_hosted_network_reason_miniport_stopped,
	wlan_hosted_network_reason_miniport_started,
	wlan_hosted_network_reason_incompatible_connection_started,
	wlan_hosted_network_reason_incompatible_connection_stopped,
	wlan_hosted_network_reason_user_action,
	wlan_hosted_network_reason_client_abort,
	wlan_hosted_network_reason_ap_start_failed,
	wlan_hosted_network_reason_peer_arrived,
	wlan_hosted_network_reason_peer_departed,
	wlan_hosted_network_reason_peer_timeout,
	wlan_hosted_network_reason_gp_denied,
	wlan_hosted_network_reason_service_unavailable,
	wlan_hosted_network_reason_device_change,
	wlan_hosted_network_reason_properties_change,
	wlan_hosted_network_reason_virtual_station_blocking_use,
	wlan_hosted_network_reason_service_available_on_virtual_station
} WLAN_HOSTED_NETWORK_REASON, *PWLAN_HOSTED_NETWORK_REASON;
*/


// WIFIMAIN

class WIFIMAIN : public CWnd
{
	DECLARE_DYNAMIC(WIFIMAIN)

public:
	WIFIMAIN();
	virtual ~WIFIMAIN();

protected:
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bEnable;
	BOOL m_bStart;
	HANDLE m_hClient;
public:
	BOOL Init();
	BOOL EnableHostedNetWork();
	BOOL DisableHostedNetWork();
	BOOL StartHostedNetWork();
	BOOL StopHostedNetWork();
	BOOL GetIsEnable();
	BOOL GetIsStart();
	void SetStart(BOOL bStart = TRUE);

	int Resume();
	int GetPeerNumber();
	void SetWifiKey(CString key);
	void SetWifiName(CString Name);
};