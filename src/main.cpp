#include "VoxObj.hpp"
#include <assert.h>
#include "soloud_speech.h"

SoLoud::Soloud* g_soloud;

extern "C" __declspec(dllexport) void* CreateVox() {
	if (!g_soloud) {
		printf("CreateVox(): initializing SoLoud;\n");
		g_soloud = new SoLoud::Soloud();
		printf("new SoLoud::Soloud;\n");
		if (g_soloud) {
			printf("CreateVox(): SoLoud core started;\n");
			g_soloud->init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::SDL2);
		}
		else {
			MessageBox(NULL, "Failed to initialize SoLoud engine.", "SND.DRV ERROR", MB_ICONERROR);
			abort();
		}
	}
	return (void*)new VoxObj;
}

extern "C" __declspec(dllexport) BOOL VoxCheckDevice(int a1) {
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL VoxDelete(int a1) {
	return TRUE;
}

// todo not a bool prob
extern "C" __declspec(dllexport) BOOL VoxFade(int a1, signed int a2, signed int a3, int a4) {
	return FALSE;
}

extern "C" __declspec(dllexport) int VoxGetComment(int a1) {
	return NULL;
}

extern "C" __declspec(dllexport) int VoxGetCurrentTime(int a1) {
	return NULL;
}

extern "C" __declspec(dllexport) signed int VoxGetStatus(LPVOID a1) {
	return MAXINT32;
}

extern "C" __declspec(dllexport) int VoxGetTotalTime(LPVOID a1) {
	return MAXINT32;
}

extern "C" __declspec(dllexport) int VoxGetVolume(LPVOID a1) {
	return MAXINT32;
}

extern "C" __declspec(dllexport) unsigned int VoxLoad(LPVOID a1) {
	return MAXUINT32;
}

extern "C" __declspec(dllexport) int VoxParseComment(int a1, int a2, int a3) {
	return NULL;
}

extern "C" __declspec(dllexport) BOOL VoxPause(int a1) {
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL VoxPlay(int a1) {
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL VoxRelease(int a1) {
	return FALSE;
}

extern "C" __declspec(dllexport) BOOL VoxSeek(int a1, signed int a2) {
	return FALSE;
}

extern "C" __declspec(dllexport) BOOL VoxSetLoop(int a1, int a2) {
	return FALSE;
}

extern "C" __declspec(dllexport) BOOL VoxSetVolume(int a1, signed int a2) {
	return FALSE;
}

extern "C" __declspec(dllexport) BOOL IsSndDrvSexy() {
	return TRUE;
}

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved )  // reserved
{
	// Perform actions based on the reason for calling.
	switch( fdwReason ) 
	{ 
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
