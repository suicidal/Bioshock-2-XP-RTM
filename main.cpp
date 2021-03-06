#include "ccapi.h"
#include <cstdio>
#include <iostream>

static CCAPI ccapi;

using namespace std;

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;


	if (!ccapi.GetLibraryState())
	{
		cout << "CCAPI Library couldn't be loaded." << endl
			<< "Check that you are using the correct CCAPI.dll." << endl;
	}
	else
	{
		vector<CCAPI::ConsoleInfo> cInfo = ccapi.GetConsoleList();

		for (u32 c = 0; c < cInfo.size(); ++c)
		{
			cout << "Console: " << cInfo.at(c).name << " Ip:" << cInfo.at(c).ip << endl;
		}

		if (ccapi.Connect("192.168.1.182") != CCAPI_OK) //_PS3_IP_HERE_
		{
			cout << "Couldn't establish a connection with your PS3." << endl;
			cout << "Verify your ps3 ip." << endl;
		}
		else
		{
			cout << "Connected to your PS3." << endl;
			ccapi.VshNotify(CCAPI::NotifyTrophy2, "Connected to CCAPI");
			ccapi.RingBuzzer(CCAPI::BuzzerTriple);

			u32 firmware, version;
			CCAPI::ConsoleType cType;
			int cell, rsx;
			int ret = ccapi.GetFirmware(&firmware) |
				ccapi.GetConsoleType(&cType) |
				ccapi.GetVersion(&version) |
				ccapi.GetTemperature(&cell, &rsx);

			if (ret != CCAPI_OK)
			{
				cout << "An error occured while retrieving console details." << endl;
			}
			else
			{
				cout << "CCAPI Version: " << version << endl;
				cout << "CCAPI Library version: " << ccapi.GetDllVersion() << endl;
				cout << "Firmware: " << CCAPI::FirmwareToString(firmware) << endl;
				cout << "Console Type: " << CCAPI::ConsoleTypeToString(cType) << endl;
				cout << "CELL Temp: " << cell << endl;
				cout << "RSX Temp: " << rsx << endl;

				bool found(false);
				u32 pid(~0);

				if (ccapi.AttachGameProcess(&found, &pid) != CCAPI_OK)
				{
					cout << "An error occured while attaching game process." << endl;
				}
				else if (!found)
				{
					cout << "Couldn't find a game process." << endl;
				}
				else
				{
					cout << "Process " << hex << pid << " attached with success." << endl;

					u8 i = ccapi.ReadMemory<u8>(0x43E86850);
					cout << "Read at 0x43E86850: " << hex << (u32)i << endl;

					ccapi.WriteMemory(0x43E86850, 39332109);
					cout << "set level to 40, will be 50 if DLC is owned" << endl;


					/*
					ccapi.WriteMemory<std::string>(0x,"hello");
					ccapi.SetConsoleIds(CCAPI::Idps,"0123456789ABCDEF0123456789ABCDEF");
					*/

				}
			}
		}
	}
	ccapi.Disconnect();

	cout << "Press any key to exit" << endl;
	getchar();

	return 0;
}
