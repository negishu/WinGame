#include "stdafx.h"
#include ".\\..\\Game\includes.h"

class Win32World : public World
{
public:
    static inline World& GetWorld()
    {
        static Win32World theWin32World;
        return theWin32World;
    }
    virtual void Draw(void* p)
    {
        PrivateDraw((GRAPHIC::DIBPlane* const)p);
    }
    virtual void DebugDraw(void* p)
    {
        PrivateDebugDraw((GRAPHIC::DIBPlane* const)p);
    }
private:
    Win32World() {};
    void PrivateDraw(GRAPHIC::DIBPlane * const pDIBPlane);
    void PrivateDebugDraw(GRAPHIC::DIBPlane * const pDIBPlane);
    GRAPHIC::Things THINGS;
};

void Win32World::PrivateDraw(GRAPHIC::DIBPlane * const pDIBPlane)
{
    for (int n = 0; n < nImageCnt; n++) {
        if (mIMAGE[n].id < 0x1000) {
            THINGS.DrawR(pDIBPlane, mIMAGE[n].px, mIMAGE[n].py, mIMAGE[n].ox, mIMAGE[n].oy, mIMAGE[n].dw, mIMAGE[n].dh, mIMAGE[n].id, mIMAGE[n].alpha);
        }
        else {
            THINGS.DrawL(pDIBPlane, mIMAGE[n].px, mIMAGE[n].py, mIMAGE[n].ox, mIMAGE[n].oy, mIMAGE[n].dw, mIMAGE[n].dh, mIMAGE[n].id - 0x1000, mIMAGE[n].alpha);
        }
    }
}

void Win32World::PrivateDebugDraw(GRAPHIC::DIBPlane * const pDIBPlane)
{
    for (int n = 0; n < nImageCnt; n++) {
        if (mIMAGE[n].id < 0x1000) {
            THINGS.DrawBox(pDIBPlane, mIMAGE[n].px, mIMAGE[n].py, mIMAGE[n].ox, mIMAGE[n].oy, mIMAGE[n].dw, mIMAGE[n].dh, mIMAGE[n].id, mIMAGE[n].alpha);
        }
        else {
            THINGS.DrawBox(pDIBPlane, mIMAGE[n].px, mIMAGE[n].py, mIMAGE[n].ox, mIMAGE[n].oy, mIMAGE[n].dw, mIMAGE[n].dh, mIMAGE[n].id - 0x1000, mIMAGE[n].alpha);
        }
    }
}

World& GetWorld() { return Win32World::GetWorld(); }
