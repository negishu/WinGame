#pragma once
#include "ManageObjects.h"
class World : public MANAGE_OBJ
{
public:
    World(void);
    virtual ~World(void);
    void PrepareDraw();
    virtual void Process(const unsigned char*);
    virtual void Draw(void*);
    virtual void DebugDraw(void*) {};
    class IMAGE {
    public:
        int px, py, ox, oy, dw, dh, id, alpha;
        static void Set(IMAGE& img, int px, int py, int ox, int oy, int dw, int dh, int id, int alpha)
        {
            img.px = px; img.py = py; img.ox = ox; img.oy = oy; img.dw = dw; img.dh = dh; img.id = id; img.alpha = alpha;
        }
    };
    static const int IMAGEMAX = 512;
    IMAGE mIMAGE[IMAGEMAX];
    int nImageCnt;
};