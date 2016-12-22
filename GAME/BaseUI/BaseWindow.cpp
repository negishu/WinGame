#include "stdafx.h"
#include "BaseWindow.h"
#include "GraphicBase.h"

enum APP_STATE {
    APP_OFF,
    APP_ON,
    APP_PAUSE,
    APP_EXIT,
    APP_NOCHANGE,
};

LONG gAppState = (LONG)APP_ON;

namespace WINDOW {

    using namespace std;

    list<IWindow*> IWindow::__WindowChainList;

#define HANDLE_MY_MSG(Ret, message, fn) case (message): Ret = fn((hWnd), (message), (wParam), (lParam)); break;

    static const DWORD cgWindowDefaultStyle = WS_OVERLAPPEDWINDOW;
    static const DWORD cgWindowDefaultExStyle = 0;

    BaseWindow::BaseWindow()
    {
    }

    BaseWindow::~BaseWindow()
    {
    }

    LRESULT BaseWindow::Create(const IWindow* pParentWindow)
    {
        CreateWndClass();
        MakeWindow(pParentWindow);
        ::SetWindowLongW(m_hWnd, GWL_USERDATA, reinterpret_cast<LONG>(GetThis()));
        OnInitialize();
        return 0;
    }

    void BaseWindow::MakeWindow(const IWindow* p)
    {
        m_hWnd = ::CreateWindowExW(
            GetWndExStyle(),
            GetWindowClass(),
            GetWindowTitle(),
            GetWndStyle(),
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            p ? p->m_hWnd : 0, NULL,
            GetModuleHandle(0), 0);

        m_pParentWindow = p;
    }

    LRESULT BaseWindow::OnClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        ::DestroyWindow(hWnd);
        return 0;
    }

    LRESULT BaseWindow::OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        ::PostQuitMessage(0);
        return 0;
    }

    LRESULT CALLBACK BaseWindow::gWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        BaseWindow* win = reinterpret_cast<BaseWindow*>(::GetWindowLongW(hWnd, GWL_USERDATA));
        if (win != NULL)
        {
            return win->WndProc(hWnd, uMsg, wParam, lParam);
        }
        return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

    LRESULT	BaseWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT Ret = 1;
        switch (uMsg)
        {
            HANDLE_MY_MSG(Ret, WM_NCCALCSIZE, OnNCCalcSize);
            HANDLE_MY_MSG(Ret, WM_GETMINMAXINFO, OnGetMinMaxInfo);
            HANDLE_MY_MSG(Ret, WM_NCPAINT, OnNCPaint);
            HANDLE_MY_MSG(Ret, WM_NCHITTEST, OnNCHitTest);
            HANDLE_MY_MSG(Ret, WM_NCLBUTTONDOWN, OnNCLBtnDown);
            HANDLE_MY_MSG(Ret, WM_NCLBUTTONUP, OnNCLBtnUp);
            HANDLE_MY_MSG(Ret, WM_NCLBUTTONDBLCLK, OnNCDCLBtn);
            HANDLE_MY_MSG(Ret, WM_NCRBUTTONDOWN, OnNCRBtnDown);
            HANDLE_MY_MSG(Ret, WM_NCRBUTTONUP, OnNCRBtnUp);
            HANDLE_MY_MSG(Ret, WM_NCRBUTTONDBLCLK, OnNCDCRBtn);
            HANDLE_MY_MSG(Ret, WM_NCACTIVATE, OnNCActive);
            HANDLE_MY_MSG(Ret, WM_NCMOUSEMOVE, OnNCMouseMove);
            HANDLE_MY_MSG(Ret, WM_WINDOWPOSCHANGING, OnPosChanging);
            HANDLE_MY_MSG(Ret, WM_SIZING, OnSizing);
            HANDLE_MY_MSG(Ret, WM_MOVING, OnMoving);
            HANDLE_MY_MSG(Ret, WM_SYSCOMMAND, OnSystemCommand);
            HANDLE_MY_MSG(Ret, WM_PAINT, OnPaint);
            HANDLE_MY_MSG(Ret, WM_DRAWITEM, OnDrawItem);
            HANDLE_MY_MSG(Ret, WM_ERASEBKGND, OnErase);
            HANDLE_MY_MSG(Ret, WM_SIZE, OnSize);
            HANDLE_MY_MSG(Ret, WM_DESTROY, OnDestroy);
            HANDLE_MY_MSG(Ret, WM_NCDESTROY, OnNCDestroy);
            HANDLE_MY_MSG(Ret, WM_VSCROLL, OnVScroll);
            HANDLE_MY_MSG(Ret, WM_HSCROLL, OnHScroll);
            HANDLE_MY_MSG(Ret, WM_MOUSEWHEEL, OnMouseWheel);
            HANDLE_MY_MSG(Ret, WM_MOUSEMOVE, OnMouseMove);
            HANDLE_MY_MSG(Ret, WM_MOUSEHOVER, OnMouseHover)
                HANDLE_MY_MSG(Ret, WM_MOUSELEAVE, OnMouseLeave)
                HANDLE_MY_MSG(Ret, WM_SETFOCUS, OnSetFocus);
            HANDLE_MY_MSG(Ret, WM_KILLFOCUS, OnKillFocus);
            HANDLE_MY_MSG(Ret, WM_ACTIVATE, OnActivate);
            HANDLE_MY_MSG(Ret, WM_KEYUP, OnKeyUp);
            HANDLE_MY_MSG(Ret, WM_KEYDOWN, OnKeyDown);
            HANDLE_MY_MSG(Ret, WM_CHAR, OnChar);
            //HANDLE_MY_MSG(Ret, WM_UNICHAR,				OnUniChar);
            HANDLE_MY_MSG(Ret, WM_IME_CHAR, OnIMEChar);
            HANDLE_MY_MSG(Ret, WM_DEADCHAR, OnDeadChar);
            HANDLE_MY_MSG(Ret, WM_SYSKEYUP, OnSysKeyUp);
            HANDLE_MY_MSG(Ret, WM_SYSKEYDOWN, OnSysKeyDown);
            HANDLE_MY_MSG(Ret, WM_SYSCHAR, OnSysChar);
            HANDLE_MY_MSG(Ret, WM_SYSDEADCHAR, OnSysDeadChar);
            HANDLE_MY_MSG(Ret, WM_LBUTTONDOWN, OnLButtonDown);
            HANDLE_MY_MSG(Ret, WM_LBUTTONUP, OnLButtonUp);
            HANDLE_MY_MSG(Ret, WM_LBUTTONDBLCLK, OnDCLButton);
            HANDLE_MY_MSG(Ret, WM_RBUTTONDOWN, OnRButtonDown);
            HANDLE_MY_MSG(Ret, WM_RBUTTONUP, OnRButtonUp);
            HANDLE_MY_MSG(Ret, WM_RBUTTONDBLCLK, OnDCRButton);
            HANDLE_MY_MSG(Ret, WM_COMMAND, OnCommand);
            HANDLE_MY_MSG(Ret, WM_DROPFILES, OnDropFiles);
            HANDLE_MY_MSG(Ret, WM_SETTEXT, OnSetWindowText);
            HANDLE_MY_MSG(Ret, WM_IME_STARTCOMPOSITION, OnIMEStartComp);
            HANDLE_MY_MSG(Ret, WM_IME_COMPOSITION, OnIMEComp);
            HANDLE_MY_MSG(Ret, WM_IME_ENDCOMPOSITION, OnIMEEndComp);
            HANDLE_MY_MSG(Ret, WM_ENTERSIZEMOVE, OnEnterSizeMove);
            HANDLE_MY_MSG(Ret, WM_EXITSIZEMOVE, OnExitSizeMove);
        default:
            return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
        }
        return Ret;
    }

    list<IAnimation*> IAnimation::__AnimationChainList;

    HANDLE IAnimation::ghTimer = 0;
    HANDLE IAnimation::ghEvent = 0;
    UINT IAnimation::gnThreadCount = 0;

    DWORD TimerThread(LPDWORD lpdwParam)
    {
        for (;;) {

            switch (::InterlockedExchange(&gAppState, APP_NOCHANGE)) {
            case APP_OFF:
                break;
            case APP_ON:
                break;
            case APP_PAUSE:
                break;
            case APP_EXIT:
                ExitThread(0);
                break;
            }

            IAnimation::OnTimer();
            ::WaitForSingleObject((HANDLE)lpdwParam, INFINITE);
        }
        return 0;
    }

    void CALLBACK TimeToUpdate(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
    {
        SetEvent(IAnimation::GetEvent());
    }

    void IAnimation::ManageTimer(BOOL bStop)
    {
        if (bStop) {
            if (nTimerID) {
                if (::timeKillEvent(nTimerID) == TIMERR_NOERROR) nTimerID = 0;
            }
            gnThreadCount--;
            if (gnThreadCount == 0) {
                ::CloseHandle(ghEvent);
                ::TerminateThread(ghTimer, 0);
            }
        }
        else {
            if (gnThreadCount == 0) {
                ghEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
                ghTimer = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, ghEvent, 0, NULL);
                ::SetPriorityClass(ghTimer, IDLE_PRIORITY_CLASS);
                ::SetThreadPriority(ghTimer, THREAD_PRIORITY_BELOW_NORMAL);
                nTimerID = ::timeSetEvent(Application::GetWaitTime(), 0, TimeToUpdate, 0, TIME_PERIODIC);
            }
            gnThreadCount++;
        }
    }

    void IAnimation::OnTimer()
    {
        for (list<IAnimation*>::iterator it = __AnimationChainList.begin();it != __AnimationChainList.end();it++) {
            (*it)->DoProcess();
            (*it)->DoDraw();
        }
    }

    static GRAPHIC::DIBPlane& GetDIBPlane() {
        static GRAPHIC::DIBPlane g_dib;
        return g_dib;
    }

    BaseBoardView::BaseBoardView() :mDIBPlane(GetDIBPlane())
    {
        ::InitializeCriticalSection(&mCS);
    }

    BaseBoardView::~BaseBoardView(void)
    {
        IAnimation::ManageTimer(TRUE);
        mDIBPlane.Destroy();
        ::DeleteCriticalSection(&mCS);
    }

    void BaseBoardView::OnInitialize()
    {
        CRITICAL cs(&mCS);
        ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
        ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
        BaseWindow::OnInitialize();
        ::SetFocus(m_hWnd);
        IAnimation::ManageTimer(FALSE);
    }

    LRESULT BaseBoardView::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        CRITICAL cs(&mCS);
        RECT clientrect; ::GetClientRect(m_hWnd, &clientrect);
        mDIBPlane.Destroy();
        mDIBPlane.Create(clientrect.right - clientrect.left, clientrect.bottom - clientrect.left);
        return 0;
    }

    void BaseBoardView::DoProcess()
    {
        CRITICAL cs(&mCS);
        unsigned char keys[0x100] = { 0, };
        for (int i = 0; i < 0x100; i++) {
            keys[i] = (GetAsyncKeyState(i) < 0 ? 1 : 0);
        }
        Process(keys);
    }

    void BaseBoardView::DoDraw()
    {
        CRITICAL cs(&mCS);
        HDC hDC = GetDC(m_hWnd);
        if (hDC != NULL) {
            Draw(mDIBPlane);
            mDIBPlane.BitBlt(hDC, SRCCOPY);
            ReleaseDC(m_hWnd, hDC);
        }
    }

}