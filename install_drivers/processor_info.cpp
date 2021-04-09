#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "Registry.h"
#include "Harddrive.h"
#include "NetworkAdapters.h"
#include "Logger.h"

//Query for network adapters : wmic path win32_pnpentity where "deviceid like '%PCI%' and pnpclass like '%NET%'" get name or wmic path win32_pnpentity where "pnpclass like '%NET%'" > C:\Users\xPAD_Gollum\Desktop\info.txt
//Query for wmic path win32_pnpentity where "pnpclass like '%NET%' and ( deviceid like '%PCI%' or deviceid like '%USB%' ) " get hardwareid > C:\Users\xpad\Desktop\info.txt


int main(int argc, char * argv[]) {
	OutputDebugStringA("Number of arguments " + argc);
	Registry r;
	if (argc == 2) {
		if (strcmp(argv[1], "-d") == 0) {
			Logger::getInstance("main")->logging_info() << "Option -d is selected. Setting up hard disk.";
			Harddrive d;
			//d.SetUpPartitions();
			d.CopyToDPartition();
			r.EditRunOnce(HARDDRIVE);
		}
		else if (strcmp(argv[1], "-m") == 0) {
			Logger::getInstance("main")->logging_info() << "Option -m is selected. Installing motherboard drivers.";
			r.EditRunOnce(MOTHERBOARD);
		}
		else if (strcmp(argv[1], "-n") == 0) {
			Logger::getInstance("main")->logging_info() << "Option -m is selected. Installing network drivers.";
			r.EditRunOnce(NETWORK);
		}
	}
	for (int i = 0; i < argc; i++) {
		Logger::getInstance("main")->logging_info() << argv[i];
	}
}
