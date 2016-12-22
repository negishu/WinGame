#pragma once

#include "World.h"

class Win32World : public World
{
public:
	Win32World()
	{
		theThings.Init();
	};
	virtual void Draw(void*);
	virtual void DebugDraw(void*);
private:
	GRAPHIC::Things theThings;
};
