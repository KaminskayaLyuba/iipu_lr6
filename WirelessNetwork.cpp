#include "WirelessNetwork.h"


WirelessNetwork::WirelessNetwork(string name, string bssid, string quality, string type, WLAN_AVAILABLE_NETWORK net)
{
	this->name = name;
	this->bssId = bssid;
	this->quality = quality;
	this->authType = type;
	this->net = net;
}


WirelessNetwork::~WirelessNetwork()
{
}

string WirelessNetwork::getAuthType()
{
	return this->authType;
}

string WirelessNetwork::getMAC()
{
	return this->bssId;
}

string WirelessNetwork::getName()
{
	return this->name;
}

string WirelessNetwork::getQuality()
{
	return this->quality;
}

string WirelessNetwork::getAuthType(DWORD dot11_code)
{
	string result;
	switch (dot11_code)
	{
	case DOT11_AUTH_ALGO_80211_OPEN:
	{
		result = "802.11 Open System authentication";
		break;
	}

	case DOT11_AUTH_ALGO_80211_SHARED_KEY:
	{
		result = "802.11 Shared Key authentication";
		break;
	}

	case DOT11_AUTH_ALGO_WPA:
	{
		result = "WPA";
		break;
	}

	case DOT11_AUTH_ALGO_WPA_PSK:
	{
		result = "WPA-PSK";
		break;
	}

	case DOT11_AUTH_ALGO_RSNA:
	{
		result = "WPA2";
		break;
	}

	case DOT11_AUTH_ALGO_RSNA_PSK:
	{
		result = "WPA2-PSK";
		break;
	}

	}
	return result;
}

string WirelessNetwork::calculateQuality(ULONG quality)
{
	string result = to_string(quality);
	int db = -100 + 0.5 * quality;
	result += "% (" + to_string(db) + " dbm)";
	return result;
}

bool WirelessNetwork::connect(GUID guid, string password)
{
	HANDLE hClient;
	DWORD currentVersion;
	WlanOpenHandle(WLAN_CLIENT_VERSION, NULL, &currentVersion, &hClient);		//получить дескриптор для раоты с сетью
	WLAN_CONNECTION_PARAMETERS params;											//структура с параметрами для подключения
	string profileOpen("<?xml version=\"1.0\"?><WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"><name>" + name + "</name><SSIDConfig><SSID><name>" + name + "</name></SSID></SSIDConfig><connectionType>ESS</connectionType><connectionMode>auto</connectionMode><autoSwitch>false</autoSwitch><MSM><security><authEncryption><authentication>open</authentication><encryption>none</encryption><useOneX>false</useOneX></authEncryption></security></MSM></WLANProfile>");
	string profile("<?xml version=\"1.0\"?><WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"><name>" + name + "</name><SSIDConfig><SSID><name>" + name + "</name></SSID></SSIDConfig><connectionType>ESS</connectionType><connectionMode>auto</connectionMode><autoSwitch>false</autoSwitch><MSM><security><authEncryption><authentication>WPA2PSK</authentication><encryption>AES</encryption><useOneX>false</useOneX></authEncryption><sharedKey><keyType>passPhrase</keyType><protected>false</protected><keyMaterial>" + password + "</keyMaterial></sharedKey></security></MSM></WLANProfile>");
	wstring profileOpenWide(profileOpen.begin(), profileOpen.end());
	params.wlanConnectionMode = wlan_connection_mode_temporary_profile;				//использовать временный профиль для подключения
	params.pDot11Ssid = &net.dot11Ssid;												//имя точки доступа к которой нужно подключиться
	params.pDesiredBssidList = NULL;
	params.strProfile = profileOpenWide.c_str();									//профиль который будет использоваться для подключения
	params.dot11BssType = net.dot11BssType;											//тип точки досупа
	params.dwFlags = WLAN_CONNECTION_HIDDEN_NETWORK;								//поключение к любой (видимой\невидимой) сети
	int res = -1;
	if (this->authType == TYPE_OPEN)												//если тип сети без пароля
		res = WlanConnect(hClient, &guid, &params, NULL);
	else
	{
		wstring wide(profile.begin(), profile.end());
		params.strProfile = wide.c_str();
		res = WlanConnect(hClient, &guid, &params, NULL);
	}
	Sleep(300);
	WlanCloseHandle(hClient, NULL);
	return res == 0;
}

void WirelessNetwork::disconnect(GUID guid)
{
	HANDLE hClient;
	DWORD currentVersion;
	WlanOpenHandle(WLAN_CLIENT_VERSION, NULL, &currentVersion, &hClient);
	WlanDisconnect(hClient, &guid, NULL);
	WlanCloseHandle(hClient, NULL);
	return;
}

WLAN_AVAILABLE_NETWORK WirelessNetwork::getAvNetStr()
{
	return this->net;
}