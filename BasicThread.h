#ifndef _BASIC_THREAD_H_
#define _BASIC_THREAD_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*=======================================================================
** File          : BasicThread.h
** Desc          : 기본적인 Thread Class
** Author        : Jin-Gyu Choi
** Create        : 2013.03.28
** Last Modify   : 2013.03.28
========================================================================*/

class BasicThread
{
protected:
    HANDLE m_hThread;        // Window Thread Handle
    DWORD  m_nThreadID;      // Window Thread ID

private:
    BasicThread(const BasicThread& bt) {}                 // 해당 Thread의 복제를 막는다
    BasicThread& operator = (const BasicThread& bt) {}    // 해당 Thread의 복제를 막는다

    static DWORD WINAPI StaticThreadStart(LPVOID lpParam) // 실제 Thread의 시작부분
    {
        BasicThread* pThread = (BasicThread*)lpParam;
        return pThread->Run();
    }

protected:
    virtual DWORD Run(void) = 0;    // Protected로 사용자가 직접 호출하는 것을 막음

public:
    BasicThread() : m_hThread(NULL), m_nThreadID(0)
    {
    }

    virtual ~BasicThread()
    {
        if (m_hThread) CloseHandle(m_hThread);
    }

public:
    //----------------------------------------------------------------
    // public functions
    //----------------------------------------------------------------
    bool Start()    // Thread 시작
    {
        if (m_hThread) // 이미 Thread가 생성되어 있는 경우
        {
            if (WaitForSingleObject(m_hThread, 0) == WAIT_TIMEOUT) { // Thread Still Running
                return false;    // Start 거부
            }
            CloseHandle(m_hThread);
        }

        // Thread 생성
        m_hThread = CreateThread(
            NULL,            // default security attributes
            0,               // default stack size
            (LPTHREAD_START_ROUTINE)BasicThread::StaticThreadStart,
            this,            // thread function argument = this class
            0,               // default creation flags
            &m_nThreadID     // receive thread identifier
        );

        if (m_hThread != NULL) return true;

        return false;
    }

    void Stop()    // Thread 강제 종료 - 비추!!
    {
        if (this->IsRunning()) {
            // 강제 Terminate이므로 정상적인 자원 해제를 기대할 수 없음
            // 공유자원 등을 물고 있을 경우 deadlock 등의 위험이 있을 수 있음
            ::TerminateThread(m_hThread, -1);
        }

        if (m_hThread) {
            CloseHandle(m_hThread);
            m_hThread = NULL;
        }
    }

public:
    //----------------------------------------------------------------
    // inline functions
    //----------------------------------------------------------------

    // Getter
    inline HANDLE GetThreadHandle() { return m_hThread; }
    inline DWORD GetThreadID() { return m_nThreadID; }

    // Status
    inline bool IsRunning()
    {
        if (m_hThread) {
            DWORD dwExitCode = 0;
            ::GetExitCodeThread(m_hThread, &dwExitCode);
            if (dwExitCode == STILL_ACTIVE) return true;
        }
        return false;
    }

    // Join - Wait For Thread Done
    inline void Join()
    {
        ::WaitForSingleObject(m_hThread, INFINITE);
    }

    // Yeild - Yeild Execution to Another Thread
    inline BOOL Yeild()
    {
        return ::SwitchToThread();
    }

    // Sleep - Suspends the execution of the current thread until the time-out interval elapses
    inline void Sleep(DWORD dsMilliiseconds)
    {
        ::Sleep(dsMilliiseconds);
    }

    // Suspend - Suspend Thread
    inline DWORD Suspend()
    {
        return ::SuspendThread(m_hThread);
    }

    // Resume - Resume Thread
    inline DWORD Resume()
    {
        return ::ResumeThread(m_hThread);
    }

};

#endif