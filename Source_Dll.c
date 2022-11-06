#include <Windows.h>
#include <stdio.h>

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "Process has been attached to target process", "ATTENTION", MB_OK | MB_ICONINFORMATION);
		break;
	case DLL_PROCESS_DETACH:
		MessageBoxA(NULL, "Process has been detached from target process", "ATTENTION", MB_OK | MB_ICONINFORMATION);
		break;
	case DLL_THREAD_ATTACH:
		MessageBoxA(NULL, "Process has been attached to targeted process thread: CreateRemoteThread();", "ATTENTION", MB_ICONINFORMATION | MB_OK);
		break;
	case DLL_THREAD_DETACH:
		MessageBoxA(NULL, "Process has been detached from targeted process thread", "ATTENTION", MB_OK | MB_ICONINFORMATION);
		break;
	default:
		MessageBoxA(NULL, "An error has occured\nPlease re-run the program\nthe issue has been spotted: between the chair and monitor\nerror code: ID-10-T", "ATTENTION", MB_OK | MB_ICONERROR);
		break;
	}
	return TRUE;
}