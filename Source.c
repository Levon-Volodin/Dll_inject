#include <Windows.h>
#include <stdio.h>
#include <io.h>

NTSTATUS(NTAPI* RtlAdjustPrivilege)(
	_In_ ULONG Privilege,
	_In_ BOOLEAN Enable,
	_In_ BOOLEAN Client,
	_Out_ PBOOLEAN WasEnabled
);

LPCSTR path = ""; //path to your DLL (i was going to make this shell-code only but decided to use the DLL route instead because it is more memory-friendly)
BOOL isFilePresent;

INT isExist() {
	if (_access(path, 0) == 0) {
		printf("%s\n", "DLL exists... continuing...");
		isFilePresent = 0x01;
		getchar();
	}
	else {
		printf("%s\n", "process injection initialization has failed: missing DLL or access to DLL has been restricted (press any key)\n(ERR 0x03)");
		isFilePresent = 0x00;
	}
}

INT main() {
	if ((CreateMutexA(NULL, TRUE, L"processInjection.exe")) == NULL) {
		printf("%s\n", "creating mutex has not succeeded... aborting...");
		printf("%i\n", GetLastError());
		getchar();
		return FALSE;
	}
	else {
		if (GetLastError() == 0xB7) {
			printf("%s\n", "It appears another instance of this process is already running... aborting... (INFORMATION 0x04)");
			getchar();
			return 0x01;
		}
	}
	//loads in all needed windows NT API endpoints
	isExist();
	if (isFilePresent == 0x00 || FALSE) {
		printf("%i\n", GetLastError());
		MessageBoxA(NULL, "DLL does not exist\nExiting...", "ATTENTION", MB_OK | MB_ICONERROR);
		for (;;) {
			return 0x00;
		}
	}

	HMODULE hNTDLL_LIB = LoadLibraryW(L"NTDLL.DLL");
	RtlAdjustPrivilege = GetProcAddress(hNTDLL_LIB, "RtlAdjustPrivilege");
	BOOLEAN bNTReceiver;
	if (RtlAdjustPrivilege) {
		NTSTATUS ntN = RtlAdjustPrivilege(19, TRUE, FALSE, &bNTReceiver); //privilege 19 is SeDebugPrivilege which is enabled and checked at runtime
		if (!ntN) {
			MessageBoxA(NULL, "All is operational and is ready to continue\n(INFORMATION 0x00)", "ATTENTION", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBoxA(NULL, "It appears RtlAdjustPrivilege is refusing syscall (ERR 0x01)\nI cant believe youve done this.", "Houston, we have a problem", MB_OK | MB_ICONERROR);
			printf("%i\n", GetLastError);
			return FALSE;
		}
	}
	else {
		printf("%i\n", GetLastError);
		MessageBoxA(NULL, "It appears RtlAdjustPrivilege has not been found (ERR 0x02)\nI cant believe youve done this.", "Houston, we have a problem", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//locating process window name (in this case it is DLL_TEST because of extensive testing)
	HWND hWnd = FindWindowA(NULL, "DLL_TEST");
	INT processID;
	GetWindowThreadProcessId(hWnd, &processID);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

	//this would be considered the ".text" section of an assembly program (where all the good magic happens)
	if (hWnd) {
		printf("process has been located as PID: %i\n", processID);
		Sleep(1000);
		LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(path), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!pDllPath) {
			printf("%s\n", "Virtual memory allocation function has not succeeded... aborting...");
			printf("%i\n", GetLastError());
			getchar();
			for (;;) {
				return 0x00;
			}
		}
		LONG sReceiver;
		if ((WriteProcessMemory(hProcess, pDllPath, path, strlen(path), &sReceiver))) {
			LPVOID libraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "LoadLibraryA");
			HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)libraryAddress, pDllPath, 0, 0);
			if (!hThread) {
				printf("%s\n", "remote thread creation denied/failed... aborting...");
				printf("%i\n", GetLastError());
				getchar();
				for (;;) {
					return 0x00;
				}
			}
			else {
				printf("%s\n", "remote thread creation and written process memory successful... awaiting response from remote thread... please wait...");
				WaitForSingleObject(hThread, INFINITE);
				printf("DLL is located at address 0x%x\n", pDllPath);
				VirtualFreeEx(hProcess, pDllPath, strlen(path) + 1, MEM_RELEASE);
			}
		}
		else {
			printf("%s\n", "Write access to desired memory address/location has been denied... aborting...");
			printf("%i\n", GetLastError);
			getchar();
			for (;;) {
				return 0x00;
			}
		}
		if ((INT)sReceiver == 0) {
			printf("%s\n", "nothing has been written to memory, (INFORMATION 0x00)");
		}
		else {
			printf("%i bytes written\n", sReceiver);
			//printf("DLL is located at address %x", );
		}
		getchar();
		for (;;) {
			return 0x00;
		}
	}
	else {
		printf("%s\n", "process window has not been located... aborting...");
		printf("%s\n", "nothing has been written, (INFORMATION 0x00)");
		getchar();
		for (;;) {
			return 0x00;
		}
	}

}