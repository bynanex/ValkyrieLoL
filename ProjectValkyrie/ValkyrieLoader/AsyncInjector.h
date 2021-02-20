#pragma once
#include "AsyncTask.h"
#include <windows.h>

class AsyncInjector : public AsyncTask {


public:

	AsyncInjector(std::initializer_list<std::string> dlls)
		:dlls(dlls) {}

	virtual void Perform() { 

		while (true) {
			HWND hWindow = FindWindowA("RiotWindowClass", NULL);
			if (hWindow == NULL) {
				currentStep = "Waiting for league process";
			}
			else if(hWindow != handleInjected) {
				handleInjected = hWindow;
				currentStep = "Injecting cheat";

				DWORD processId = 0;
				GetWindowThreadProcessId(hWindow, &processId);

				for (auto& dllPath : dlls) {
					InjectIntoPID(dllPath, processId);
					Sleep(3000); 
				}
				currentStep = "Cheat is injected. Enjoy your game";
			}

			Sleep(100);
		}
	}

private:

	void InjectIntoPID(std::string& dllPath, DWORD pid){
		const char* dll = dllPath.c_str();

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (OpenProcess == NULL) {
			SetError("Could not open process");
			return;
		}

		HMODULE hModule = GetModuleHandle("kernel32.dll");
		LPVOID lpBaseAddress = (LPVOID)GetProcAddress(hModule, "LoadLibraryA");
		if (lpBaseAddress == NULL) {
			SetError("Unable to locate LoadLibraryA");
			return;
		}

		LPVOID lpSpace = (LPVOID)VirtualAllocEx(hProcess, NULL, strlen(dll), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (lpSpace == NULL) {
			SetError("Could not allocate memory in process");
			return;
		}

		int n = WriteProcessMemory(hProcess, lpSpace, dll, strlen(dll), NULL);
		if (n == 0) {
			SetError("Could not write to process's address space");
			return;
		}

		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpBaseAddress, lpSpace, NULL, NULL);
		if (hThread == NULL) {
			SetError("Couldn't create remote thread");
			return;
		}
		CloseHandle(hProcess);
	}

private:
	bool                     injected  = false;
	HWND                     handleInjected = NULL;
	std::vector<std::string> dlls;
};