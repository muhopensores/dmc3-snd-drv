#include "VoxObj.hpp"
#include <stdio.h>
#include <assert.h>

#define FUCKING_SEEK_HACK 1

BOOL VoxObj::load_mem(unsigned char *a_mem, int a_length) {
	if (!m_w) {
		m_w = new SoLoud::Wav;
	}
	int result = m_w->loadMem(a_mem, a_length, false, false);
	if (result) {
		MessageBox(NULL, "Soloud loadMem error!", "ERROR", MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

void VoxObj::play3d(float a_pos_x, float a_pos_y, float a_pos_z, float volume) {
	g_soloud->update3dAudio();
	m_wave_handle = g_soloud->play3d(*m_w, a_pos_x, a_pos_y, a_pos_z, 0.0f,0.0f,0.0f, volume);
}

void VoxObj::set_listener3d(float a_lpos_x, float a_lpos_y, float a_lpos_z,
	                        float a_at_x,   float a_at_y,   float a_at_z,
	                        float a_up_x,   float a_up_y,   float a_up_z ) {
	
	g_soloud->set3dListenerParameters(a_lpos_x, a_lpos_y, a_lpos_z,
		a_at_x, a_at_y, a_at_z,
		a_up_x, a_up_y, a_up_z);
}


BOOL VoxObj::load(const char* filename)
{
	if (!m_wave) {
		m_wave = new SoLoud::WavStream;
	}
	int result = m_wave->loadToMem(filename);

	m_wave_handle = g_soloud->playBackground(*m_wave);
	m_wave->setInaudibleBehavior(false, true);

#ifdef _DEBUG
	
	printf("[snd.drv] %s (pthis=0x%X, filename=%s);\n", __FUNCTION__, (size_t)this, filename);
#endif // _DEBUG
	return TRUE;
}

BOOL VoxObj::restart()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG
	return TRUE;
}

BOOL VoxObj::pause()
{
	if (m_wave_handle != -1 && m_wave) {
		g_soloud->setPause(m_wave_handle, true);
	}
#ifdef DEBUG
	printf("[snd.drv] %s(0x%X) \n", __FUNCTION__, (uintptr_t)this);
#endif // DEBUG

	return TRUE;
}

BOOL VoxObj::free_buffer_and_close_file_probably()
{
	if (m_wave) {
		g_soloud->stop(m_wave_handle);
		m_wave_handle = -1;
		delete m_wave;
		m_wave = nullptr;
	}

#ifdef _DEBUG
	printf("[snd.drv] %s(0x%X) \n", __FUNCTION__, (uintptr_t)this);
#endif // _DEBUG

	return TRUE;
}

BOOL VoxObj::unk_sub_10001BC0(int a2)
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG

	return TRUE;
}

void VoxObj::Function5()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG

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

#ifdef _DEBUG
	printf("[snd.drv] set volume %f\n", volume);
#endif // _DEBUG
	g_soloud->setVolume(m_wave_handle, m_volume);

	return TRUE;
}

void VoxObj::Function7()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG

}

void VoxObj::Function8()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG

}

void VoxObj::Function9()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG

}

void VoxObj::Function10()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif
}

void VoxObj::Function11()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif
}

void VoxObj::Function12()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif
}

BOOL VoxObj::Function13()
{
#ifdef _DEBUG
    printf("active voices: %d\n", g_soloud->getActiveVoiceCount());
	printf("[snd.drv] %s(0x%X) \n", __FUNCTION__, (uintptr_t)this);
	return FALSE;
#endif // _DEBUG
    return FALSE;
}

// Function14
void* VoxObj::get_ogg_comment_ptr()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);

#endif // _DEBUG

	return (void*)0xDEADBEEF;
}

void VoxObj::Function15()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG

}

void VoxObj::Function16()
{
#ifdef _DEBUG
	printf("[snd.drv] %s not implemented\n", __FUNCTION__);
#endif // _DEBUG

}


BOOL VoxObj::Function17(int a3, float loop_start, float loop_end)
{
	if (!m_wave) { return TRUE; }

	m_loop = true;
	if (AlmostEqual2sComplement(loop_start, 0.0f, 5) && AlmostEqual2sComplement(loop_end, 0.0f, 5)) {
		m_loop = false;
#ifdef _DEBUG
		printf("[snd.drv] %s loop_start && loop_end == 0", __FUNCTION__);
#endif // _DEBUG
		return TRUE;
	}
    m_wave->setLooping(m_loop);

    m_loop_start = (double)(loop_start / 1000.0f);
    m_loop_end = (double)(loop_end / 1000.0f);
    
    m_wave->setLoopRange(m_loop_start, m_loop_end);
	
#ifdef _DEBUG
	printf("[snd.drv] %s (%d, %f, %f)\n", __FUNCTION__, a3, loop_start, loop_end);
#endif // _DEBUG

	return TRUE;
}
