#include "MainForm.h"
using namespace IapdWireless;

MainForm::MainForm()
{
	InitializeComponent();
}

MainForm::~MainForm()
{
	if (components)
	{
		delete components;
	}
}

void MainForm::InitializeComponent()
{
	this->components = gcnew System::ComponentModel::Container();
	this->Size = System::Drawing::Size(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->Text = WINDOW_TITLE;
	this->Padding = System::Windows::Forms::Padding(0);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
	list = gcnew ListView();
	list->Width = LISTVIEW_WIDTH;
	list->Height = LISTVIEW_HEIGHT;
	list->Location = Point(LISTVIEW_X, LISTVIEW_Y);
	list->View = View::Details;
	list->Columns->Add(LISTVIEW_COL1, LISTVIEW_COL1_WIDTH, HorizontalAlignment::Center);
	list->Columns->Add(LISTVIEW_COL2, LISTVIEW_COL2_WIDTH, HorizontalAlignment::Center);
	list->Columns->Add(LISTVIEW_COL3, LISTVIEW_COL3_WIDTH, HorizontalAlignment::Center);
	list->Columns->Add(LISTVIEW_COL4, LISTVIEW_COL4_WIDTH, HorizontalAlignment::Center);
	refreshTimer = gcnew Timer;
	refreshTimer->Interval = TIMER_TICK_TIME;
	refreshTimer->Enabled = true;
	refreshTimer->Tick += gcnew System::EventHandler(this, &MainForm::Timer_Tick);
	Controls->Add(list);
	connectButton = gcnew Button();
	connectButton->Text = "Connect";
	connectButton->Location = Point(10, 425);
	disconnectButton = gcnew Button();
	disconnectButton->Text = "Disconnect";
	disconnectButton->Location = Point(95, 425);
	passwordBox = gcnew TextBox();
	passwordBox->Location = Point(190, 427);
	Controls->Add(connectButton);
	Controls->Add(disconnectButton);
	Controls->Add(passwordBox);
	WirelessNetworkInterface wireless;
	if (wireless.isConnected())
	{
		disconnectButton->Enabled = true;
		connectButton->Enabled = false;
		
	}
	else
	{
		vector<WirelessNetwork> wirelessVector = wireless.getAvailableNetworksVector();
		if (wirelessVector.size() == 0)
		{
			disconnectButton->Enabled = false;
			connectButton->Enabled = false;
		}
		else
		{
			disconnectButton->Enabled = false;
			connectButton->Enabled = true;
		}
		
	}
	connectButton->Click += gcnew System::EventHandler(this, &MainForm::Connect_Click);
	disconnectButton->Click += gcnew System::EventHandler(this, &MainForm::Disconnect_Click);
	InitList();
}

void MainForm::InitList()
{
	WirelessNetworkInterface wireless;
	vector<WirelessNetwork> wirelessVector = wireless.getAvailableNetworksVector();
	if (wireless.isConnected())
	{
		disconnectButton->Enabled = true;
		connectButton->Enabled = false;
	}
	else
	{
		if (wirelessVector.size() == 0)
		{
			disconnectButton->Enabled = false;
			connectButton->Enabled = false;
		}
		else
		{
			disconnectButton->Enabled = false;
			connectButton->Enabled = true;
		}
		
	}
	int selectedIndex = this->GetSelectedIndex();
	list->Items->Clear();
	for (int i = 0; i < wirelessVector.size(); i++)
	{
		WirelessNetwork net = wirelessVector.at(i);
		String ^name = gcnew String(net.getName().c_str());
		String ^mac = gcnew String(net.getMAC().c_str());
		String ^quality = gcnew String(net.getQuality().c_str());
		String ^authType = gcnew String(net.getAuthType().c_str());
		ListViewItem ^item = gcnew ListViewItem(name);
		item->SubItems->Add(mac);
		item->SubItems->Add(quality);
		item->SubItems->Add(authType);
		list->Items->Add(item);
	}
	try {
		list->Items[selectedIndex]->Selected = true;
	}
	catch (...)
	{
		selectedIndex = 0;
	}
}

void MainForm::Timer_Tick(System::Object^ Sender, EventArgs ^e)
{
	InitList();
}

void MainForm::Connect_Click(System::Object^ Sender, EventArgs ^e)
{
	ListViewItem ^item = list->SelectedItems[0];
	String ^t = item->Text;

	WirelessNetworkInterface wireless;
	vector<WirelessNetwork> wirelessVector = wireless.getAvailableNetworksVector();
	for (vector<WirelessNetwork>::iterator it = wirelessVector.begin(); it != wirelessVector.end(); it++)
	{
		WirelessNetwork net = *it;
		String ^netName = gcnew String(net.getName().c_str());
		if (netName == t)
		{
			GUID g = wireless.getInterfaceGUID();
			string password = marshal_as<std::string>(passwordBox->Text);
			int result = net.connect(g, password);
			if (!wireless.isConnected())
			{
				MessageBox::Show("Can't connect to this AP.", "Error", MessageBoxButtons::OK);
			}
			else
			{
				disconnectButton->Enabled = true;
				connectButton->Enabled = false;
			}
			return;

		}
	}
}

void MainForm::Disconnect_Click(System::Object^ Sender, EventArgs ^e)
{
	WirelessNetworkInterface net;
	GUID guid = net.getInterfaceGUID();
	WirelessNetwork::disconnect(guid);
	disconnectButton->Enabled = false;
	connectButton->Enabled = true;
}



int MainForm::GetSelectedIndex()
{
	int selectedIndex = -1;
	try
	{
		if (list->SelectedItems->Count > 0)
		{
			selectedIndex = list->Items->IndexOf(list->SelectedItems[0]);
		}
	}
	catch (...)
	{
		selectedIndex = 0;
	}
	return selectedIndex;
}