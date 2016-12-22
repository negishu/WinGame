#include "stdafx.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    Application::CreateApplication(hInstance, hPrevInstance, lpCmdLine, nCmdShow, 25);
    BaseBoardView::DO();
    Application::MainThread();
    return 0;
}

void main()
{
    World theWorld;
    while (1) {
        unsigned char keys[0x100] = { 0, };
        for (int i = 0; i < 0x100; i++) {
            keys[i] = (GetAsyncKeyState(i) < 0 ? 1 : 0);
        }
        theWorld.Process(keys);
        theWorld.PrepareDraw();
        theWorld.Draw(0);
        if (keys[0x0D]) break;
    }
}