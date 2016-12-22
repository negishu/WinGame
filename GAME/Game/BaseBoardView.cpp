#include "stdafx.h"
#include "BaseBoardView.h"

#include "Win32World.h"

BaseBoardView::BaseBoardView()
{
	wndstyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

	pWorld = new Win32World();
}

BaseBoardView::~BaseBoardView(void)
{
	ManageTimer(TRUE);

	mDIBPlane.Destroy();

	delete pWorld;
}

BOOL ReadMP3File(LPTSTR lpszFileName, LPMPEGLAYER3WAVEFORMAT lpmf, LPBYTE *lplpData, LPDWORD lpdwSize);
BOOL DecodeToWave(LPWAVEFORMATEX lpwfSrc, LPBYTE lpSrcData, DWORD dwSrcSize, LPWAVEFORMATEX lpwfDest, LPBYTE *lplpDestData, LPDWORD lpdwDestSize);
BOOL ReadWaveFile(LPTSTR lpszFileName, LPWAVEFORMATEX lpwf, LPBYTE *lplpData, LPDWORD lpdwDataSize);

void BaseBoardView::OnInitialize()
{
    ::ShowWindow(m_hWnd, SW_SHOWNORMAL);

    ::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	BaseWindow::OnInitialize();

	::SetFocus(m_hWnd);

	ManageTimer(FALSE);

	static LPBYTE   lpWaveData = NULL;
	static HWAVEOUT hwo = NULL;
	static WAVEHDR  wh = {0};

	DWORD        dwDataSize;
	WAVEFORMATEX wf;

	if (!ReadWaveFile(TEXT("Overworld.wav"), &wf, &lpWaveData, &dwDataSize))
		return;
	
	if (waveOutOpen(&hwo, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
		return;
	}

	wh.lpData         = (LPSTR)lpWaveData;
	wh.dwBufferLength = dwDataSize;
	wh.dwFlags        = WHDR_BEGINLOOP | WHDR_ENDLOOP;
	wh.dwLoops        = 18;

	waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));
	waveOutWrite(hwo, &wh, sizeof(WAVEHDR));
	
}

LRESULT BaseBoardView::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT clientrect; ::GetClientRect(m_hWnd, &clientrect);

    mDIBPlane.Destroy();

    mDIBPlane.Create(clientrect.right - clientrect.left, clientrect.bottom - clientrect.left);

    return 0;
}

LRESULT BaseBoardView::OnDraw(HDC hDC)
{
	pWorld->Draw(&mDIBPlane);

	mDIBPlane.BitBlt(hDC, SRCCOPY);

    return 1;
}

void BaseBoardView::OnTime()
{	
	BYTE state[0x100]={0,};

	for (int i = 0; i < 0x100; i++) {

		state[i] = (GetAsyncKeyState(i) < 0 ? 1 : 0);
	}

	pWorld->Process(state);

	::InvalidateRect(m_hWnd, NULL, FALSE);
}
