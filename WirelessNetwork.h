#pragma once
#include <Windows.h>
#include <string>
#include <wchar.h>
#include <Wlanapi.h>
#define WLAN_CLIENT_VERSION 2
#define TYPE_OPEN "802.11 Open System authentication"
#pragma comment(lib, "Wlanapi.lib")

using namespace std;

class WirelessNetwork
{
private:
	WLAN_AVAILABLE_NETWORK net;							//структура с инф о сети
	string name;										//имя точки
	string bssId;										
	string quality;										//качество
	string authType;									//тип авторизации
public:
	static string calculateQuality(ULONG quality);		//посчитать качество
	static string getAuthType(DWORD dot11_code);		//получить тип автоизации
	bool connect(GUID guid, string password);			//подключиься к сети
	static void disconnect(GUID guid);					//отключиться от сети
	string getName();									//получить имя сети
	string getMAC();									//получить MAC
	string getQuality();								//получить качество
	string getAuthType();								//поучить тип авторизции
	WLAN_AVAILABLE_NETWORK getAvNetStr();				//получить структуру с информацией о сети
	WirelessNetwork(string name, string bssid, string quality, string type, WLAN_AVAILABLE_NETWORK net);
	~WirelessNetwork();
};

