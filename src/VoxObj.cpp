#include "VoxObj.hpp"
#include <stdio.h>
#include <assert.h>
#include <math.h>


static BOOL _stdcall d_load(VoxObj* pthis, std::string filename) {
    BOOL res = pthis->original->load(filename.c_str());
#if 0
    pthis->is_loaded = true;
    std::hash<std::string> hasher;
    pthis->st_hash = hasher(filename);
#endif
    return res;
};
static BOOL _stdcall d_restart(VoxObj* pthis) {
    return pthis->original->restart();
};
static BOOL _stdcall d_pause(VoxObj* pthis) {
    return pthis->original->pause();
};
static BOOL _stdcall d_free_buffer_and_close_file_probably(VoxObj* pthis) {
    BOOL res = pthis->original->free_buffer_and_close_file_probably();
#if 0
    pthis->is_loaded = false;
#endif
    return res;
};
static BOOL _stdcall d_unk_sub_10001BC0(VoxObj* pthis, int a2) {
    return pthis->original->unk_sub_10001BC0(a2);
};
static BOOL _stdcall d_set_volume(VoxObj* pthis, float volume) {
    return pthis->original->set_volume(volume);
};
static BOOL _stdcall d_Function13(VoxObj* pthis) {
    return pthis->original->Function13();
};
static BOOL _stdcall d_set_loop_range(VoxObj* pthis, int a3, float a4, float a5) {
    return pthis->original->set_loop_range(a3, a4, a5);
};

bool g_quit = false;

#if 0
DWORD WINAPI VoxThreadFunc(LPVOID param) {
    VoxObj* vox = static_cast<VoxObj*>(param);

    g_executor.WorkerLoop();
    return 0;
}
#endif

BOOL VoxObj::load_mem(unsigned char *a_mem, int a_length) {
#if 0
	if (!m_w) {
		m_w = new SoLoud::Wav;
	}
	int result = m_w->loadMem(a_mem, a_length, false, false);
	if (result) {
		MessageBox(NULL, "Soloud loadMem error!", "ERROR", MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
#endif
    return TRUE;
}


BOOL VoxObj::load(const char* filename)
{
#ifdef _DEBUG
	printf("[snd.drv] %s (pthis=0x%X, filename=%s);\n", __FUNCTION__, (size_t)this, filename);
#endif // _DEBUG
#if 0
    std::hash<std::string> hasher;
    size_t new_hash = hasher(filename);

    if (st_hash != new_hash) {
        free_buffer_and_close_file_probably();
        executor->defer(d_load, this, std::string{filename});

	    return TRUE;//original->load(filename);
    }

    if (is_loaded.load(std::memory_order_acquire)) {
        return original->load(filename);
    }
#endif
    executor->defer(d_load, this, std::string{filename});
	return TRUE;//original->load(filename);
}

BOOL VoxObj::restart()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->restart();
    }
#endif
    executor->defer(d_restart, this);
	return TRUE;//original->restart();
}

BOOL VoxObj::pause()
{

#ifdef _DEBUG
	printf("[snd.drv] %s(0x%X) \n", __FUNCTION__, (uintptr_t)this);
#endif // DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->pause();
    }
#endif
    executor->defer(d_pause, this);
	return TRUE;//original->pause();
}

BOOL VoxObj::free_buffer_and_close_file_probably()
{

#ifdef _DEBUG
	printf("[snd.drv] %s(0x%X) \n", __FUNCTION__, (uintptr_t)this);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        auto res = original->free_buffer_and_close_file_probably();
        is_loaded = false;
        return res;
    }
    is_loaded = false;
#endif
    executor->defer(d_free_buffer_and_close_file_probably, this);
	return TRUE;//original->free_buffer_and_close_file_probably();
}

BOOL VoxObj::unk_sub_10001BC0(int a2)
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->unk_sub_10001BC0(a2);
    }
#endif
    executor->defer(d_unk_sub_10001BC0, this, a2);
    return TRUE;//original->unk_sub_10001BC0(a2);
}

void VoxObj::Function5()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function5();
    }
#endif
    return original->Function5();

}
bool AlmostEqual2sComplement(float A, float B, int maxUlps)
{
	// Make sure maxUlps is non-negative and small enough that the
	// default NAN won't compare as equal to anything.
	assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
	int aInt = *(int*)&A;
	// Make aInt lexicographically ordered as a twos-complement int
	if (aInt < 0)
		aInt = 0x80000000 - aInt;
	// Make bInt lexicographically ordered as a twos-complement int
	int bInt = *(int*)&B;
	if (bInt < 0)
		bInt = 0x80000000 - bInt;
	int intDiff = abs(aInt - bInt);
	if (intDiff <= maxUlps)
		return true;
	return false;
}

BOOL VoxObj::set_volume(float volume)
{
#if 0
	unsigned int volume_bits = *(unsigned int*)&volume;
	if (m_previous_volume_bits == volume_bits) {
		return TRUE;
	}
	m_previous_volume_bits = *(unsigned int*)&volume;
#ifdef FUCKING_SEEK_HACK
	if (volume <= 0.1f) { 
        m_volume = 0.0f;
        g_soloud->setVolume(m_wave_handle, 0.0);
        g_soloud->seek(m_wave_handle, 0.0);
        return FALSE;
    }
    else {
        m_volume = volume;
    }
#endif

#endif

#ifdef _DEBUG
	printf("[snd.drv] set volume %f\n", volume);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->set_volume(volume);
    }
#endif
    executor->defer(d_set_volume, this, volume);
    //thread_action |= VOXPROC::SET_VOLUME;
	return TRUE;//original->set_volume(volume);
}

void VoxObj::Function7()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function7();
    }
#endif
    return original->Function7();
}

void VoxObj::Function8()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function8();
    }
#endif
    return original->Function8();
}

void VoxObj::Function9()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function9();
    }
#endif
    return original->Function9();
}

void VoxObj::Function10()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function10();
    }
#endif
    return original->Function10();
}

void VoxObj::Function11()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function11();
    }
#endif
    return original->Function11();
}

void VoxObj::Function12()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function12();
    }
#endif
    return original->Function12();
}

BOOL VoxObj::Function13()
{
#ifdef _DEBUG
    //printf("active voices: %d\n", g_soloud->getActiveVoiceCount());
	printf("[snd.drv] %s(0x%X) \n", __FUNCTION__, (uintptr_t)this);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function13();
    }
#endif
    executor->defer(d_Function13, this);
    return TRUE;//original->Function13();
}

// Function14
void* VoxObj::get_ogg_comment_ptr()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);

#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->get_ogg_comment_ptr();
    }
#endif
	return original->get_ogg_comment_ptr();
}

void VoxObj::Function15()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function15();
    }
#endif
    return original->Function15();
}

void VoxObj::Function16()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
#if 0
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->Function16();
    }
#endif
    return original->Function16();

}


BOOL VoxObj::set_loop_range(int a3, float loop_start, float loop_end)
{
    #ifdef _DEBUG
	printf("[snd.drv] %s (%d, %f, %f)\n", __FUNCTION__, a3, loop_start, loop_end);
#endif // _DEBUG
#if 0
    st_loop_start  = loop_start;
    st_loop_end = loop_end;
    st_a3 = a3;
    if (is_loaded.load(std::memory_order_acquire)) {
        return original->set_loop_range(a3, loop_start, loop_end);
    }
#endif

    //thread_action |= VOXPROC::SET_LOOP_RANGE;
    executor->defer(d_set_loop_range, this, a3, loop_start, loop_end);
	return TRUE;//original->set_loop_range(a3, loop_start, loop_end);

	//return TRUE;
}
