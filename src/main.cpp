#include "VoxObj.hpp"
#include <assert.h>
#include <array>
#include <vector>


static HMODULE g_snd_drv_handle = NULL;
static std::vector<VoxObj*> g_vox_objs;

#define CREATE_VOX_ORIGINAL(name) \
  void * name()
typedef CREATE_VOX_ORIGINAL(create_vox_original);

CREATE_VOX_ORIGINAL(CreateVoxOriginalStub) { return 0; }
static create_vox_original *CreateVoxOriginal_ = CreateVoxOriginalStub;

static std::array<LPVOID, 2> dmc3se_sleep_offsets = {
    (LPVOID)0x00404987,
    (LPVOID)0x00404998
};

extern "C" __declspec(dllexport) void* CreateVox() {
    static bool once = false;
    if (!once) {
        DWORD oldProtect;
        for(auto address : dmc3se_sleep_offsets) {
            DWORD* code_ptr = (DWORD*)address;
            if (*code_ptr != 0xD7FF646A) {
                continue;
            }

            BOOL res = VirtualProtect(address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &oldProtect);
            if (!res) {
                char buffer[256];
                sprintf(buffer, "Failed to patch sleep call at %lu", (unsigned long)address);
                MessageBoxA(NULL, buffer, "dmc3se.exe", MB_ICONERROR); 
            }
            *code_ptr = 0x90909090;

            DWORD oldOldProtect;
            res =VirtualProtect(address, sizeof(DWORD), oldProtect, &oldOldProtect);
            if (!res) {
                char buffer[256];
                sprintf(buffer, "Failed to restore oldProtect at %lu", (unsigned long)address);
                MessageBoxA(NULL, buffer, "dmc3se.exe", MB_ICONERROR); 
            }
        }
        once = true;
    }
    VoxObj* proxy = g_vox_objs.emplace_back(new VoxObj);
    void* original = CreateVoxOriginal_();
    proxy->original = (VoxObj*)original;
    proxy->executor = new DeferredExecutor();
    //proxy->hthread = CreateThread(NULL, 0, VoxThreadFunc, proxy, 0, NULL);
	return proxy;
}

extern "C" __declspec(dllexport) BOOL VoxCheckDevice(int a1) {
    assert("VoxCheckDevice");
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL VoxDelete(int a1) {
    VoxObj* proxy = (VoxObj*)a1;
    delete proxy->executor;
    proxy->original->free_buffer_and_close_file_probably();
    assert("VoxDelete");
	return TRUE;
}

// todo not a bool prob
extern "C" __declspec(dllexport) BOOL VoxFade(int a1, signed int a2, signed int a3, int a4) {
    assert("VoxFade");
	return FALSE;
}

extern "C" __declspec(dllexport) int VoxGetComment(int a1) {
    assert("VoxGetComment");
	return NULL;
}

extern "C" __declspec(dllexport) int VoxGetCurrentTime(int a1) {
    assert("VoxGetCurrentTime");
	return NULL;
}

extern "C" __declspec(dllexport) signed int VoxGetStatus(LPVOID a1) {
    assert("VoxGetStatus");
	return MAXINT32;
}

extern "C" __declspec(dllexport) int VoxGetTotalTime(LPVOID a1) {
    assert("VoxGetTotalTime");
	return MAXINT32;
}

extern "C" __declspec(dllexport) int VoxGetVolume(LPVOID a1) {
    assert("VoxGetVolume");
	return MAXINT32;
}

extern "C" __declspec(dllexport) unsigned int VoxLoad(LPVOID a1) {
    assert("VoxLoad");
	return MAXUINT32;
}

extern "C" __declspec(dllexport) int VoxParseComment(int a1, int a2, int a3) {
    assert("VoxParseComment");
	return NULL;
}

extern "C" __declspec(dllexport) BOOL VoxPause(int a1) {
    assert("VoxPause");
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL VoxPlay(int a1) {
    assert("VoxPlay");
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL VoxRelease(int a1) {
    assert("VoxRelease");
	return FALSE;
}

extern "C" __declspec(dllexport) BOOL VoxSeek(int a1, signed int a2) {
    assert("VoxSeek");
	return FALSE;
}

extern "C" __declspec(dllexport) BOOL VoxSetLoop(int a1, int a2) {
    assert("VoxSetLoop");
	return FALSE;
}

extern "C" __declspec(dllexport) BOOL VoxSetVolume(int a1, signed int a2) {
    assert("VoxSetVolume");
	return FALSE;
}
#if 0
extern "C" __declspec(dllexport) BOOL IsSndDrvSexy() {
    assert("IsSndDrvSexy");
	return TRUE;
}
#endif
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
        g_snd_drv_handle = LoadLibrary("snd.drv.original");
        if (g_snd_drv_handle) {
            CreateVoxOriginal_ = (create_vox_original*)GetProcAddress(g_snd_drv_handle, "CreateVox");
        }
        else {
            MessageBoxA(NULL, "Could not load snd.drv.orginal. Rename original capcoms snd.drv to snd.drv.original and restart the game", "SND.DRV", MB_ICONERROR);
        }
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
        g_quit = true;
        for (VoxObj* obj : g_vox_objs) {
            //CloseHandle(obj->hthread);
            delete obj->executor;
            delete obj; // idk call delete on original?
        }
        FreeLibrary(g_snd_drv_handle);
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
