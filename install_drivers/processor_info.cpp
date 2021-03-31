

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "Registry.h"
#include "Harddrive.h"
#include "NetworkAdapters.h"

//Query for network adapters : wmic path win32_pnpentity where "deviceid like '%PCI%' and pnpclass like '%NET%'" get name or wmic path win32_pnpentity where "pnpclass like '%NET%'" > C:\Users\xPAD_Gollum\Desktop\info.txt
//Query for wmic path win32_pnpentity where "pnpclass like '%NET%' and ( deviceid like '%PCI%' or deviceid like '%USB%' ) " get hardwareid > C:\Users\xpad\Desktop\info.txt


int main(int argc, char * argv[]) {
	std::cout << "Number of arguments " << argc << std::endl;
	Registry r;
	if (argc == 2) {
		if (strcmp(argv[1], "-d") == 0) {
			Harddrive d;
			//d.SetUpPartitions();
			d.GetHardDrives();
			r.EditRunOnce(HARDDRIVE);
		}
		else if (strcmp(argv[1], "-m") == 0) {
			r.EditRunOnce(MOTHERBOARD);
		}
		else if (strcmp(argv[1], "-n") == 0) {
			r.EditRunOnce(NETWORK);
		}
	}
	for (int i = 0; i < argc; i++) {
		std::cout << argv[i] << std::endl;
	}
	//GetDriverMotherboard();
}
