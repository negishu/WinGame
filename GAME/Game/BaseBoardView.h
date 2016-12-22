#pragma once

#include "World.h"

class BaseBoardView : public WINDOW::BaseWindow, public WINDOW::IAnimation
{
public:

    BaseBoardView();

    virtual ~BaseBoardView(void);

protected:

	virtual void OnInitialize();

    virtual void OnTime();

    virtual	LRESULT OnDraw  (HDC hDC);

    virtual LRESULT OnSize	(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	GRAPHIC::DIBPlane mDIBPlane;

	World* pWorld;
};
