#include "Registry.h"

Registry::Registry()
{
}

bool Registry::EditRunOnce(DRIVER d)
{
	HKEY hKEY = NULL;
	wchar_t szValue[256] = {};
	wchar_t szOption[3] = {};
	LONG result;
	switch (d) {
	case HARDDRIVE:
		wcscpy_s(szOption, L"-m");
		break;
	case MOTHERBOARD:
		wcscpy_s(szOption, L"-n");
		break;
	case NETWORK:
		wcscpy_s(szOption, L"");
		break;
	}
	if (wcscmp(szOption, L"") != 0) {
		wcscpy_s(szValue, 256, this->pathExe);
		wcscat_s(szValue, 256, L"install_drivers.exe"); // Name of the .exe file
		wcscat_s(szValue, 256, L" ");
		wcscat_s(szValue, 256, szOption);	// Argument of the command
		result = RegCreateKeyExW(HKEY_LOCAL_MACHINE, this->runOnceKey, 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKEY, NULL);
		OutputDebugStringA("Result of creating key: "  + result);
		if (result == 0) {
			DWORD len_word = (wcslen(szValue) + 1) * 2;
			result = RegSetValueExW(hKEY, L"Install", 0, REG_SZ, (BYTE*)szValue, len_word);
			OutputDebugStringA("Result of modifying key: " + result );
		}
		if (hKEY != NULL) {
			OutputDebugStringA("Cleaning up used Registry Key");
			RegCloseKey(hKEY);
			hKEY = NULL;
		}
	}
	else {
		OutputDebugStringA("Execution flow has ended. There is nothing to write on Windows registry");
	}
	return true;
}

Registry::~Registry()
{
}
