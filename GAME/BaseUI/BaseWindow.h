#pragma once

namespace GRAPHIC {
    class DIBPlane;
}
namespace WINDOW {
    class WindowDelegate {
    public:
        WindowDelegate() {};
        virtual ~WindowDelegate() {};
        virtual WCHAR* GetWindowTitle() const { return L"...."; }
        virtual WCHAR* GetWindowClass() const { return L"...."; }
        virtual bool CanResize()              const { return false; }
        virtual bool CanMaximize()            const { return false; }
        virtual bool IsAlwaysOnTop()          const { return false; }
        virtual bool HasAlwaysOnTopMenu()     const { return false; }
        virtual bool ShouldShowWindowTitle()  const { return true; }
        virtual bool ShouldShowWindowIcon()   const { return false; }
        virtual void WindowClosing() {}
    };
    class WindowOption {
    public:
        WindowOption()
        {
        }
        virtual ~WindowOption()
        {
        }
        virtual DWORD GetWndStyle() { return WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX; };
        virtual DWORD GetWndExStyle() { return 0; };
        virtual DWORD GetClassStyle() { return CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW; };
    };
    class IWindow {
    public:
        IWindow() : m_pParentWindow(0), m_hWnd(0) { __WindowChainList.push_back(this); };
        virtual ~IWindow()
        {
            __WindowChainList.remove(this);
        }
        LPVOID GetThis() { return this; };
        HWND m_hWnd;
    protected:
        virtual LRESULT	Create(const IWindow* pWindow = NULL) = 0;
        virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
        typedef std::list<IWindow*> WindowChainList;
        static WindowChainList __WindowChainList;
        const IWindow* m_pParentWindow;
    };
    class BaseWindow : public IWindow, public WindowDelegate, public WindowOption
    {
    public:
        virtual LRESULT Create(const IWindow* pWindow = NULL);
        virtual ~BaseWindow();
    protected:
        static LRESULT CALLBACK gWndProc(HWND, UINT, WPARAM, LPARAM);
        BaseWindow();
        virtual BOOL CreateWndClass()
        {
            HINSTANCE hInst = GetModuleHandle(0);
            WNDCLASSEXW wndclass;
            if (::GetClassInfoExW(hInst, GetWindowClass(), &wndclass) == 0) {
                wndclass.cbSize = sizeof(WNDCLASSEX);
                wndclass.style = GetClassStyle();
                wndclass.lpfnWndProc = gWndProc;
                wndclass.cbClsExtra = 0;
                wndclass.cbWndExtra = 0;
                wndclass.hInstance = hInst;
                wndclass.hIcon = (HICON)LoadImage(hInst, L"NORMAL_ICON", IMAGE_ICON, 32, 32, 0);
                wndclass.hIconSm = (HICON)LoadImage(hInst, L"SMALL_ICON", IMAGE_ICON, 16, 16, 0);
                wndclass.hCursor = NULL;
                wndclass.hbrBackground = 0;
                wndclass.lpszMenuName = NULL;
                wndclass.lpszClassName = GetWindowClass();
                if (RegisterClassExW(&wndclass) == 0) {
                    return FALSE;
                }
            }
            return TRUE;
        }
        virtual void MakeWindow(const IWindow* p);
        virtual void OnInitialize() { };
        virtual void DrawOnScreen() { };
        virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual LRESULT OnClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnDrawItem(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSystemCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSetWindowText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCCalcSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnGetMinMaxInfo(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCHitTest(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCLBtnDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCLBtnUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCDCLBtn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCRBtnDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCRBtnUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCDCRBtn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCActive(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnNCMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnPosChanging(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSizing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnMoving(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnErase(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSetFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnKillFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnUniChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnIMEChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnDeadChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSysKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSysKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSysChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnSysDeadChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnVScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnHScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnMouseHover(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnMouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnDCLButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnRButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnDCRButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnDropFiles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnIMEStartComp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnIMEComp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnIMEEndComp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnEnterSizeMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
        virtual LRESULT OnExitSizeMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, uMsg, wParam, lParam); }
    };

    class IAnimation {
    public:
        IAnimation() : nTimerID(0) { __AnimationChainList.push_back(this); }
        virtual ~IAnimation() { __AnimationChainList.remove(this); }
        static HANDLE GetEvent() { return ghEvent; };
        static void OnTimer();
    protected:
        void ManageTimer(BOOL bStop = FALSE);
        int nTimerID;
    private:
        virtual void DoProcess() {}
        virtual void DoDraw() {}
        typedef std::list<IAnimation*> AnimationChainList;
        static AnimationChainList __AnimationChainList;
        static UINT   gnThreadCount;
        static HANDLE ghTimer;
        static HANDLE ghEvent;
    };

    class BaseBoardView : public WINDOW::BaseWindow, public WINDOW::IAnimation
    {
    public:
        BaseBoardView();
        virtual ~BaseBoardView(void);
    protected:
        virtual	void Draw(const GRAPHIC::DIBPlane& aDIBPlane) {};
        virtual	void Process(const unsigned char keys[]) {};
        virtual void OnInitialize();
        virtual void DoProcess();
        virtual void DoDraw();
        virtual LRESULT OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        GRAPHIC::DIBPlane& mDIBPlane;
        CRITICAL_SECTION mCS;
    };

}
