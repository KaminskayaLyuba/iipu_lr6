#pragma once
#include <Windows.h>
#include <stdio.h>
#include <Wlanapi.h>
#include <Windot11.h>
#include <vector>
#include "WirelessNetwork.h"
#define WLAN_CLIENT_VERSION 2
#pragma comment(lib, "Wlanapi.lib")
using namespace std;

class WirelessNetworkInterface
{
private:
	HANDLE hClient;																		//хендл
	DWORD currentVersion;																//текущая версия
	WLAN_BSS_ENTRY getBssInfo(GUID interfaceGuid, PDOT11_SSID ssid, bool isSecured);	//получить инф о точке доступа
	vector<WLAN_AVAILABLE_NETWORK> getAvailableNetworks(GUID interfaceGuid);			//получить доступные сети
	vector<WLAN_INTERFACE_INFO> getWlanInterfaces();									//получиь доступные бепроводные интерфейсы
	string getBssId(GUID interfaceGuid, PDOT11_SSID ssid, bool isSecured);				//получить ID точки доступа
	
public:
	WirelessNetworkInterface();
	bool isConnected();																	//подключен ли 
	GUID getInterfaceGUID();															//получает GUID интерфейса
	vector<WirelessNetwork> getAvailableNetworksVector();								
	~WirelessNetworkInterface();
};

