#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "Registry.h"
#include "Harddrive.h"
#include "Motherboard.h"
#include "NetworkAdapters.h"
#include "Logger.h"

//Query for network adapters : wmic path win32_pnpentity where "deviceid like '%PCI%' and pnpclass like '%NET%'" get name or wmic path win32_pnpentity where "pnpclass like '%NET%'" > C:\Users\xPAD_Gollum\Desktop\info.txt
//Query for wmic path win32_pnpentity where "pnpclass like '%NET%' and ( deviceid like '%PCI%' or deviceid like '%USB%' ) " get hardwareid > C:\Users\xpad\Desktop\info.txt
const char* classname = "processor_info";

int main(int argc, char * argv[]) {
	Logger::getInstance(classname, "C:\\Users\\Isma_VM\\Documents\\Technica_Equipment\\log_.txt")->logging_debug() << "Number of arguments " << argc;
	Registry r = Registry(argv[0]);
	if (argc == 2) {
		if (strcmp(argv[1], "-d") == 0) {
			Logger::getInstance(classname)->logging_info() << "Option -d is selected. Setting up hard disk.";
			Harddrive d;
			//d.SetUpPartitions();
			d.CopyToDPartition();
			r.EditRunOnce(HARDDRIVE);
		}
		else if (strcmp(argv[1], "-m") == 0) {
			Logger::getInstance(classname)->logging_info() << "Option -m is selected. Installing motherboard drivers.";
			Motherboard m;
			m.GetMotherboardInformation();
			m.ShowMotherboardInformation();
			m.SetUpModel("PRIME X570-PRO");
			m.GetDriverMotherboard("drivers.json");
			m.SetUpDriverPath("C:\\Users\\Isma_VM\\Documents\\Drivers\\Motherboard\\PRIME-X570-PRO");
			m.InstallDriver();
			r.EditRunOnce(MOTHERBOARD);
		}
		else if (strcmp(argv[1], "-n") == 0) {
			Logger::getInstance(classname)->logging_info() << "Option -m is selected. Installing network drivers.";
			r.EditRunOnce(NETWORK);
		}
	}
	for (int i = 0; i < argc; i++) {
		Logger::getInstance(classname)->logging_info() << argv[i];
	}
}
