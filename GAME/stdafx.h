#pragma once

#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _WIN32
#pragma warning(disable : 4127)
#pragma warning(disable : 4995)
#endif

// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <strsafe.h>
#include <wchar.h>
#include <atlbase.h>
#include <xmllite.h>
#include <strsafe.h>

#pragma warning(disable:4201)
#include <mmsystem.h>
#pragma warning(default:4201)

#include <mmreg.h>
#include <msacm.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <map>
#include <iterator>
#include <list>
#include <assert.h>


#include <iostream>


#define ASSERT(b) assert(b);

#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

class CRITICAL
{
public:
    CRITICAL(LPCRITICAL_SECTION pcs)
    {
        EnterCriticalSection(m_pcs = pcs);
    }
    ~CRITICAL(void)
    {
        LeaveCriticalSection(m_pcs);
    }
private:
    LPCRITICAL_SECTION  m_pcs;
};

static inline BOOL IsKeyDown(int nKey)
{
    return (::GetAsyncKeyState(nKey) < 0);
}

static inline void ScreenToWindow(HWND hWnd, POINT& pt)
{
    RECT windowrect; ::GetWindowRect(hWnd, &windowrect);

    pt.x -= windowrect.left;
    pt.y -= windowrect.top;
}

static WCHAR* GetExePath()
{
    static WCHAR szOutput[_MAX_PATH * 6] = { 0 };

    WCHAR szPath[_MAX_PATH] = { 0 };
    WCHAR szDrive[_MAX_DRIVE] = { 0 };
    WCHAR szDir[_MAX_DIR] = { 0 };
    WCHAR szFileName[_MAX_FNAME] = { 0 };
    WCHAR szExt[_MAX_EXT] = { 0 };

    DWORD dwRet = GetModuleFileName(NULL, szPath, sizeof(szPath));

    if (dwRet) {

        _wsplitpath_s(szPath, szDrive, szDir, szFileName, szExt);

        ::StringCchPrintfExW(szOutput, sizeof(szOutput), NULL, NULL, STRSAFE_NO_TRUNCATION, L"%s%s", szDrive, szDir);
    }

    return szOutput;
}

static inline int GetScreenWidth()
{
    return ::GetSystemMetrics(SM_CXSCREEN);
}

static inline int GetScreenHeight()
{
    return ::GetSystemMetrics(SM_CYSCREEN);
}

#include ".\\Application\includes.h"
#include ".\\BaseUI\includes.h"
#include ".\\Game\includes.h"
#include ".\\WinGame\includes.h"
