#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <cstdint>
#include <atomic>

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class DeferredExecutor {
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread worker;
    bool stop = false;

public:
    DeferredExecutor() {
        worker = std::thread([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    // Wait until there's work or we're shutting down
                    cv.wait(lock, [this] { return stop || !tasks.empty(); });
                    
                    if (stop && tasks.empty()) break;
                    
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                // Execute outside the lock
                task();
            }
        });
    }

    // Template to accept any callable + any arguments
    template <typename Func, typename... Args>
    void defer(Func&& func, Args&&... args) {
        std::lock_guard<std::mutex> lock(mtx);
        
        // C++17: Capture everything by value. This copies/moves args into the
        // lambda's closure object, safely extending their lifetime.
        tasks.emplace([func = std::forward<Func>(func), 
                       ...args = std::forward<Args>(args)]() mutable {
            std::invoke(func, args...);
        });
        
        cv.notify_one();
    }

    ~DeferredExecutor() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        worker.join(); // Wait for pending tasks to finish
    }
};

enum VOXPROC : uint32_t {
    NONE           = 0,
    LOAD           = 1 << 0,
    RESTART        = 1 << 1,
    PAUSE          = 1 << 2,
    FREE           = 1 << 3,
    UNK_SUB        = 1 << 4,
    SET_VOLUME     = 1 << 5,
    SET_LOOP_RANGE = 1 << 6,
    FUNCTION13     = 1 << 7,
};

class VoxObj
{
public:
    VoxObj* original;
    DeferredExecutor* executor;
#if 0
    char pad[56];

    std::atomic<bool> is_loaded{0};

    char pad0[60];

    size_t st_hash;
    char st_filename[2048];

    int st_a2;
    float st_volume;
    int st_a3;
    float st_loop_start;
    float st_loop_end;
    int st_af13;
#endif
	virtual BOOL _stdcall load(const char* filename);
	virtual BOOL _stdcall restart();
	virtual BOOL _stdcall pause();
	virtual BOOL _stdcall free_buffer_and_close_file_probably();
	virtual BOOL _stdcall unk_sub_10001BC0(int a2);
	virtual void _stdcall Function5();
	virtual BOOL _stdcall set_volume(float volume);
	virtual void _stdcall Function7();
	virtual void _stdcall Function8();
	virtual void _stdcall Function9();
	virtual void _stdcall Function10();
	virtual void _stdcall Function11();
	virtual void _stdcall Function12();
	virtual BOOL _stdcall Function13();
	virtual void* _stdcall get_ogg_comment_ptr();
	virtual void _stdcall Function15();
	virtual void _stdcall Function16();
	virtual BOOL _stdcall set_loop_range(int a3, float a4, float a5);
	virtual BOOL _stdcall load_mem(unsigned char *a_mem, int a_length); // NOTE(): custom own stuff dont call through original
	//virtual void _stdcall set_source3d_pos(float x, float y, float z);
}; //Size: 0x204A

DWORD WINAPI VoxThreadFunc(LPVOID param);

extern bool g_quit;
extern class DeferredExecutor* g_executor;