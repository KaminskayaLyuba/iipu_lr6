#include "WirelessNetworkInterface.h"

WirelessNetworkInterface::WirelessNetworkInterface()
{
}


WirelessNetworkInterface::~WirelessNetworkInterface()
{
}


vector<WLAN_INTERFACE_INFO> WirelessNetworkInterface::getWlanInterfaces()			//Получить список беспроводных интерфейсов
{
	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	vector<WLAN_INTERFACE_INFO> result;
	if (WlanEnumInterfaces(hClient, NULL, &pIfList) != ERROR_SUCCESS)
		return result;
	for (int i = 0; i < pIfList->dwNumberOfItems; i++)
	{
		result.push_back(pIfList->InterfaceInfo[i]);
	}
	return result;
}

vector<WLAN_AVAILABLE_NETWORK> WirelessNetworkInterface::getAvailableNetworks(GUID interfaceGuid)
{
	PWLAN_AVAILABLE_NETWORK_LIST pAvList = NULL;
	vector<WLAN_AVAILABLE_NETWORK> result;
	WlanGetAvailableNetworkList(hClient,				//получить список доступных сетей
		&interfaceGuid,									//GUID устройства которое щет сети
		0,
		NULL,
		&pAvList);
	if (!pAvList)
		return result;
	for (int i = 0; i < pAvList->dwNumberOfItems; i++)
	{
		result.push_back(pAvList->Network[i]);
	}
	return result;
}

string WirelessNetworkInterface::getBssId(GUID interfaceGuid, PDOT11_SSID ssid, bool isSecured)
{
	WLAN_BSS_ENTRY info = getBssInfo(interfaceGuid, ssid, isSecured);
	char macStr[18];
	macStr[0] = info.dot11Bssid[0] / 16 + (info.dot11Bssid[0] / 16 < 10 ? '0' : 'A'-10);
	macStr[1] = info.dot11Bssid[0] % 16 + (info.dot11Bssid[0] % 16 < 10 ? '0' : 'A'-10);
	macStr[2] = ':';
	macStr[3] = info.dot11Bssid[1]/ 16 + (info.dot11Bssid[1] / 16 < 10 ? '0' : 'A'-10);
	macStr[4] = info.dot11Bssid[1] % 16 + (info.dot11Bssid[1] % 16 < 10 ? '0' : 'A'-10);
	macStr[5] = ':';
	macStr[6] = info.dot11Bssid[2]/ 16 + (info.dot11Bssid[2] / 16 < 10 ? '0' : 'A'-10);
	macStr[7] = info.dot11Bssid[2]% 16 + (info.dot11Bssid[2] % 16 < 10 ? '0' : 'A'-10);
	macStr[8] = ':';
	macStr[9] = info.dot11Bssid[3]/ 16 + (info.dot11Bssid[3] / 16 < 10 ? '0' : 'A'-10);
	macStr[10] = info.dot11Bssid[3] % 16+ (info.dot11Bssid[3] % 16 < 10 ? '0' : 'A'-10);
	macStr[11] = ':';
	macStr[12] = info.dot11Bssid[4]/ 16 + (info.dot11Bssid[4] / 16 < 10 ? '0' : 'A'-10);
	macStr[13] = info.dot11Bssid[4]% 16 + (info.dot11Bssid[4] % 16 < 10 ? '0' : 'A'-10);
	macStr[14] = ':';
	macStr[15] = info.dot11Bssid[5]/ 16 + (info.dot11Bssid[5] / 16 < 10 ? '0' : 'A'-10);
	macStr[16] = info.dot11Bssid[5]% 16 + (info.dot11Bssid[5] % 16 < 10 ? '0' : 'A'-10);
	macStr[17] = '\n';
	return string(macStr);
}

WLAN_BSS_ENTRY WirelessNetworkInterface::getBssInfo(GUID interfaceGuid, PDOT11_SSID ssid, bool isSecured)
{
	PWLAN_BSS_LIST list;
	WlanGetNetworkBssList(hClient, &interfaceGuid, ssid, dot11_BSS_type_infrastructure, isSecured, NULL, &list);
	return list->wlanBssEntries[0];
}

vector<WirelessNetwork> WirelessNetworkInterface::getAvailableNetworksVector()
{
	WlanOpenHandle(WLAN_CLIENT_VERSION, NULL, &currentVersion, &hClient);
	vector<WirelessNetwork> result;
	vector<WLAN_INTERFACE_INFO> interfaces = getWlanInterfaces();
	if (interfaces.size() == 0)
		return result;
	vector<WLAN_AVAILABLE_NETWORK> availableNet = getAvailableNetworks(interfaces[0].InterfaceGuid);
	for (int i = 0; i < availableNet.size(); i++)
	{
		string name((char *) availableNet[i].dot11Ssid.ucSSID);
		ULONG quality = availableNet[i].wlanSignalQuality;
		string qualityStr = WirelessNetwork::calculateQuality(quality);
		string authType = WirelessNetwork::getAuthType(availableNet[i].dot11DefaultAuthAlgorithm);
		string bssid = getBssId(interfaces[0].InterfaceGuid, &availableNet[i].dot11Ssid, authType != "802.11 Open System authentication");
		WirelessNetwork net(name, bssid, qualityStr, authType, availableNet[i]);
		result.push_back(net);
	}
	WlanCloseHandle(hClient, NULL);
	return result;
}

bool WirelessNetworkInterface::isConnected()
{
	WlanOpenHandle(WLAN_CLIENT_VERSION, NULL, &currentVersion, &hClient);
	vector<WLAN_INTERFACE_INFO> interfaces = getWlanInterfaces();
	if (interfaces.size() == 0 || interfaces[0].isState == wlan_interface_state_disconnected)
		return false;
	WlanCloseHandle(hClient, NULL);
	return true;
}

GUID WirelessNetworkInterface::getInterfaceGUID()
{
	WlanOpenHandle(WLAN_CLIENT_VERSION, NULL, &currentVersion, &hClient);
	GUID res;
	vector<WLAN_INTERFACE_INFO> interfaces = getWlanInterfaces();
	if (interfaces.size() > 0)
	{
		res = interfaces[0].InterfaceGuid;
	}
	WlanCloseHandle(hClient, NULL);
	return res;
}