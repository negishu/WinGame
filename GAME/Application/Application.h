#pragma once

class Application
{
public:
    static Application& CreateApplication(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow, DWORD dwWaitTimer);
    inline static void MainThread()
    {
        static MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            DispatchMessage(&msg);
        }
    }
    static inline DWORD	GetWaitTime() { return m_theObj->m_dwWaitTime; }
private:
    Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow, DWORD dwWaitTimer);
    virtual ~Application();
    static inline HINSTANCE GetInstance() { return m_theObj->m_hInstance; }
    static inline HINSTANCE GetPrevInstance() { return m_theObj->m_hPrevInstance; }
    static inline LPTSTR    GetCmdLine() { return m_theObj->m_lpCmdLine; }
    static inline int		GetCmdShow() { return m_theObj->m_nCmdShow; }
    inline static bool PeekMessage()
    {
        static MSG msg;
        if (::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else {
            msg.message = 0;
        }
        return msg.message == WM_QUIT ? 1 : 0;
    }
    static Application* m_theObj;
    HINSTANCE	m_hInstance;
    HINSTANCE	m_hPrevInstance;
    LPTSTR  	m_lpCmdLine;
    INT			m_nCmdShow;
    DWORD		m_dwWaitTime;
};
