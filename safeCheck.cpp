#include <driver.h++>
#include "vector"
// #include <stdio.h>
#include <iostream>
#include <psapi.h>
#include <synchapi.h>
#include <tchar.h>

#include "windows.h"
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1
#ifdef libtest
#define targetName "noSocketMain.exe"
#elif defined visualtest
#define targetName "VISUAL.exe"
#else
#define targetName "LabUSE.exe"
#endif
using namespace std;

bool check_name(DWORD processID);

bool check_name(DWORD processID, const std::string& target_name);

void PrintProcessNameAndID(DWORD processID);

void safety_behaviour();

int main(int argc, char* argv[])
{
	// Get the list of process identifiers.
	int check_counts{};
	while (true)
	{
		DWORD aProcesses[1024], cbNeeded, cProcesses;
		unsigned int i;

		if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) { return 1; }
		// Calculate how many process identifiers were returned.

		cProcesses = cbNeeded / sizeof(DWORD);

		// Print the name and process identifier for each process.

		for (i = 0; i < cProcesses; i++)
		{
			if (aProcesses[i] != 0)
			{
				if (argc == 2) { check_counts += check_name(aProcesses[i], argv[1]); }
				else { check_counts += check_name(aProcesses[i]); }

				//                PrintProcessNameAndID(aProcesses[i]);
			}
		}
		if (check_counts == 0)
		{
			std::cout << "Warning! LOST RUNNING PROCESS!" << std::endl;
			safety_behaviour();
			//            system("pause");
			return 0;
		}
		else
		{
			//            std::cout << "safe" << std::endl;
			check_counts = 0;
		}
		Sleep(10);
	}
	return 0;
}

void safety_behaviour()
{
	Sleep(50);
	Sleep(50);
	Sleep(50);
	Sleep(50);
	Sleep(50);
    auto arm_ads = new ADS::axis_ads();
    arm_ads->setPort(PLC);
	Sleep(50);

    auto m = DRIVE::axis_drive(arm_ads);
	// force the enable flag equals 1 to force disable!
    m.DISABLE();
	// auto I = new IO_ADS;
	// auto i = IO_DRIVE(I);
	// i.GRAB_HOLD();
	// i.stopSignal(false);
	cout << "This is Safety Check program!" << endl;
	cout << "EMERGENCY QUIT!" << endl;
}

void PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	// Print the process name and identifier.

	// _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

	// Release the handle to the process.

	CloseHandle(hProcess);
}

bool check_name(DWORD processID, const std::string& target_name)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}
	if ((std::string)szProcessName == target_name) { return 1; }
	return 0;
}

bool check_name(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}
	if ((std::string)szProcessName == targetName) { return 1; }
	return 0;
}
