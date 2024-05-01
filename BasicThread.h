#ifndef _BASIC_THREAD_H_
#define _BASIC_THREAD_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*=======================================================================
** File          : BasicThread.h
** Desc          : �⺻���� Thread Class
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
    BasicThread(const BasicThread& bt) {}                 // �ش� Thread�� ������ ���´�
    BasicThread& operator = (const BasicThread& bt) {}    // �ش� Thread�� ������ ���´�

    static DWORD WINAPI StaticThreadStart(LPVOID lpParam) // ���� Thread�� ���ۺκ�
    {
        BasicThread* pThread = (BasicThread*)lpParam;
        return pThread->Run();
    }

protected:
    virtual DWORD Run(void) = 0;    // Protected�� ����ڰ� ���� ȣ���ϴ� ���� ����

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
    bool Start()    // Thread ����
    {
        if (m_hThread) // �̹� Thread�� �����Ǿ� �ִ� ���
        {
            if (WaitForSingleObject(m_hThread, 0) == WAIT_TIMEOUT) { // Thread Still Running
                return false;    // Start �ź�
            }
            CloseHandle(m_hThread);
        }

        // Thread ����
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

    void Stop()    // Thread ���� ���� - ����!!
    {
        if (this->IsRunning()) {
            // ���� Terminate�̹Ƿ� �������� �ڿ� ������ ����� �� ����
            // �����ڿ� ���� ���� ���� ��� deadlock ���� ������ ���� �� ����
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