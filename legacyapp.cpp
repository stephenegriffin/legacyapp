#include "pch.h"
#include <Windows.h>
#include <iostream>

#define EC_W32(fnx) \
	[&]() -> LSTATUS { \
		const auto _lresult = (fnx); \
		if (_lresult != ERROR_SUCCESS) wprintf(L"lResult = 0x%08X from %hs\r\n", _lresult, #fnx); \
		return _lresult; \
	}()

int main()
{
	HKEY hKey{};

	wprintf(L"Testing RegOpenKeyExW\r\n");
	auto lResult =
		EC_W32(RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft", 0, KEY_READ | KEY_WOW64_64KEY, &hKey));
	if (lResult == ERROR_SUCCESS)
	{
		wprintf(L"hKey = 0x%08p\r\n", hKey);
		EC_W32(RegCloseKey(hKey));
	}

	wprintf(L"Testing RegCreateKeyExW \r\n");
	auto dwDisposition = DWORD{};
	lResult = EC_W32(RegCreateKeyExW(
		HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE | KEY_WOW64_64KEY,
		NULL,
		&hKey,
		&dwDisposition));
	if (lResult == ERROR_SUCCESS)
	{
		wprintf(L"hKey = 0x%08p, dwDisposition = 0x%08X\r\n", hKey, dwDisposition);

		auto dwValue = DWORD(42);
		EC_W32(RegSetValueExW(hKey, L"testkey", NULL, REG_DWORD, reinterpret_cast<LPBYTE>(&dwValue), sizeof(DWORD)));

		EC_W32(RegCloseKey(hKey));
	}
}