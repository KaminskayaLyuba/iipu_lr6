#pragma once
#include "WirelessNetworkInterface.h"
#include <msclr\marshal_cppstd.h>
#include <stdlib.h>
using namespace msclr::interop;
#define WINDOW_TITLE "Wireless Info"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 500
#define LISTVIEW_WIDTH 580
#define LISTVIEW_HEIGHT 420
#define LISTVIEW_X 2
#define LISTVIEW_Y 2
#define LISTVIEW_COL1 "Имя"
#define LISTVIEW_COL2 "MAC (BSS ID)"
#define LISTVIEW_COL3 "Качество"
#define LISTVIEW_COL4 "Тип аутентификации"
#define LISTVIEW_COL1_WIDTH 100
#define LISTVIEW_COL2_WIDTH 150
#define LISTVIEW_COL3_WIDTH 90
#define LISTVIEW_COL4_WIDTH 215
#define TIMER_TICK_TIME 3000



namespace IapdWireless {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	public ref class MainForm : public System::Windows::Forms::Form
	{
	private:
		Button ^connectButton;
		Button ^disconnectButton;
		ListView ^list;
		TextBox ^passwordBox;
		System::ComponentModel::Container ^components;
		Timer ^refreshTimer;
		void InitList();
		void InitializeComponent();
		void Timer_Tick(System::Object^ Sender, EventArgs ^e);
		void Connect_Click(System::Object^ Sender, EventArgs ^e);
		void Disconnect_Click(System::Object^ Sender, EventArgs ^e);
		int GetSelectedIndex();
	protected:
		~MainForm();
	
	public:
		MainForm();
	};
}