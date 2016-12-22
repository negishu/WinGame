#pragma once
#include "Collision.h"
static const int D = 1000;
static inline int ToL(int n, int D) { return n * D; };
static inline int ToP(int n, int D) { return n / D; };
class MANAGE_OBJ;
class OBJ {
public:
    OBJ() :nCharID(0), nTYPE(TYPE_NONE), x(0x7FFFFFFF), y(0x7FFFFFFF), cx(0x7FFFFFFF), cy(0x7FFFFFFF), nx(0x7FFFFFFF), ny(0x7FFFFFFF), dx(0), dy(0), ax(0), ay(0), w(0), h(0) {};
    virtual bool Init(MANAGE_OBJ& m) { return true; }
    virtual bool TryMove(MANAGE_OBJ& m, const unsigned char* pKeys = 0) { return true; }
    virtual bool Move() { x = nx; y = ny; return true; }
    virtual bool SetTarget(int _dx, int _dy, int _ax, int _ay) { dx = _dx; dy = _dy; ax = _ax; ay = _ay;return true; }
    static void Set(OBJ& obj, int nCharID, int nTYPE, int x, int y, int w, int h, int a = 255)
    {
        obj.nCharID = nCharID;
        obj.nTYPE = nTYPE;
        obj.x = x;
        obj.y = y;
        obj.w = w;
        obj.h = h;
        obj.cx = x;
        obj.cy = y;
        obj.nx = x;
        obj.ny = y;
        obj.tx = x;
        obj.ty = y;
        obj.ox = x;
        obj.oy = y;
        obj.dx = 0;
        obj.dy = 0;
        obj.ax = 0;
        obj.ay = 0;
        obj.nAlpha = a;
    }
    int nCharID, nTYPE, nAlpha;
    int x, y, w, h;
    int cx, cy, nx, ny, dx, dy, ax, ay, tx, ty, ox, oy;
    static OBJ NULLOBJ;
    static int CNT;
    static const int TYPE_NONE = 0;
    static const int TYPE_BLOCK = 1;
    static const int TYPE_FLOOR = 2;
    static const int TYPE_KAME = 3;
    static const int TYPE_MARIO = 4;
    static const int TYPE_BROKENBLOCK = 5;
};
