#include "stdafx.h"
#include "BaseBoardView.h"
#include "Win32World.h"

void BaseBoardView::Draw(const GRAPHIC::DIBPlane& aDIBPlane)
{
    GetWorld().PrepareDraw();
    GetWorld().Draw(&mDIBPlane);
    GetWorld().DebugDraw(&mDIBPlane);
}
void BaseBoardView::Process(const unsigned char keys[])
{
    GetWorld().Process(keys);
}
