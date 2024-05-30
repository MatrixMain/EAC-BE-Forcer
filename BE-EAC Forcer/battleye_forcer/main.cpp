#include <iostream>
#include <chrono>
#include <Windows.h>
#include <psapi.h> 
#include <tlhelp32.h>


#include "Forcer.hpp"
#include "Log.h"



#define MAX_PROCESSES 1024 
DWORD ProcessIDF;
DWORD abcd;
DWORD FindProcess(__in_z LPCTSTR lpcszFileName)
{
	LPDWORD lpdwProcessIds;
	LPTSTR  lpszBaseName;
	HANDLE  hProcess;
	DWORD   i, cdwProcesses, dwProcessId = 0;

	lpdwProcessIds = (LPDWORD)HeapAlloc(GetProcessHeap(), 0, MAX_PROCESSES * sizeof(DWORD));
	if (lpdwProcessIds != NULL)
	{
		if (EnumProcesses(lpdwProcessIds, MAX_PROCESSES * sizeof(DWORD), &cdwProcesses))
		{
			lpszBaseName = (LPTSTR)HeapAlloc(GetProcessHeap(), 0, MAX_PATH * sizeof(TCHAR));
			if (lpszBaseName != NULL)
			{
				cdwProcesses /= sizeof(DWORD);
				for (i = 0; i < cdwProcesses; i++)
				{
					hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lpdwProcessIds[i]);
					if (hProcess != NULL)
					{
						if (GetModuleBaseName(hProcess, NULL, lpszBaseName, MAX_PATH) > 0)
						{
							if (!lstrcmpi(lpszBaseName, lpcszFileName))
							{
								dwProcessId = lpdwProcessIds[i];
								CloseHandle(hProcess);
								break;
							}
						}
						CloseHandle(hProcess);
					}
				}
				HeapFree(GetProcessHeap(), 0, (LPVOID)lpszBaseName);
			}
		}
		HeapFree(GetProcessHeap(), 0, (LPVOID)lpdwProcessIds);
	}
	return dwProcessId;
}
void AddOneDay()
{
	system("powershell -Command \"Set-Date -Date(Get-Date).AddDays(1) | Out-Null\"");
}
void RemoveEye()
{
	system("taskkill /F /IM FortniteClient-Win64-Shipping_BE.exe");
	system("sc stop BEService");
	system("taskkill /F /IM FortniteClient-Win64-Shipping.exe");
}

auto main() -> void
{
	printf("You want to Force EAC or BE ?\n");
	printf("[1] - EasyAntiCheat\n");
	printf("[2] - Battleye\n");
	std::string get;
	std::cin >> get;
	if (get == "1")
	{
		printf("EAC FORCER ENABLED!\n");
		bool e = true;

		while (e)
		{
			ProcessIDF = FindProcess(L"FortniteClient-Win64-Shipping_BE.exe");

			if (ProcessIDF != 0)
			{
				RemoveEye();
				AddOneDay();

			}

			ProcessIDF = 0;
			Sleep(500);
		}
	}
	else if (get == "2")
	{
		system("cls");
		while (utils::get_process_id(L"EasyAntiCheat.exe") || utils::get_process_id(L"FortniteClient-Win64-Shipping_EAC.exe"))
		{
			forcer::kill_eac();
		}
		forcer::force_battleye();

		std::cin.get();
	}
	else
	{
		printf("Error");
		Sleep(5000);
		exit(0);
	}


}
