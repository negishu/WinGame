#include "ManageObjects.h"
#include "Objects.h"
#include "Collision.h"
#include "Character.h"

MANAGE_OBJ::MANAGE_OBJ() :MapX(0), MapY(0), W(ToL(1600, D)), H(ToL(600, D)), nCnt(0)
{
    CollisionManager::GetCollisionManager().Init(W, H, 4);
    Initialize(*this);
}

bool MANAGE_OBJ::Add(OBJ* obj)
{
    for (int n = 0; n < SIZE; n++) {
        if (OBJs[n] == obj) return false;
    }
    OBJs[nCnt] = obj; nCnt++;
    Sort(0, nCnt - 1);
    return true;
}

bool MANAGE_OBJ::TryMove(int sx, int sy, int ex, int ey, const unsigned char * pKeys)
{
    return false;
}

bool MANAGE_OBJ::Move(int sx, int sy, int ex, int ey, const unsigned char* pKeys)
{
    if (nCnt > 0) {
        Sort(0, nCnt - 1);
    }

    for (int n = 0; n < nCnt; n++) {
        if (sx - 1000000 <= GetCharX(n) && GetCharX(n) <= ex + 100000) {
            TryMove(n, pKeys);
        }
    }

    if (GetMainCharacter().x < (100000 + MapX)) {
        MapX -= ((100000 + MapX) - GetMainCharacter().x) / 2;
    }

    if (GetMainCharacter().x > (400000 + MapX)) {
        MapX += (GetMainCharacter().x - (400000 + MapX)) / 2;
    }

    OBJ::CNT++; OBJ::CNT %= 10000;

    return true;
}

bool MANAGE_OBJ::TryMove(int n, const unsigned char* pKeys)
{
    OBJs[n]->TryMove(*this, pKeys);
    return OBJs[n]->Move();
}

void MANAGE_OBJ::Swap(int i, int j)
{
    OBJ* temp;
    temp = OBJs[i];
    OBJs[i] = OBJs[j];
    OBJs[j] = temp;
}

void MANAGE_OBJ::Sort(int left, int right)
{
    int i = left, j = right;
    int mid = OBJs[(i + j) / 2]->x;
    while (1) {
        while (OBJs[i]->x < mid) i++;
        while (mid < OBJs[j]->x) j--;
        if (i >= j) break;
        Swap(i, j);
        i++; j--;
    }
    if (left < i - 1) Sort(left, i - 1);
    if (j + 1 < right) Sort(j + 1, right);
}

const OBJ& MANAGE_OBJ::GetObj(const OBJ* obj, int nCheckType, int& index)
{
    int x = obj->cx, y = obj->cy, w = obj->w, h = obj->h;
    for (int n = index; n < nCnt; n++) {
        if (OBJs[n]->nTYPE == nCheckType) {
            if (OBJs[n] == obj) continue;
            if (MANAGE_OBJ::GetCharX(n) > x + w) break;
            if (MANAGE_OBJ::GetCharX(n) + MANAGE_OBJ::GetCharW(n) < x) continue;
            if (MANAGE_OBJ::GetCharY(n) + MANAGE_OBJ::GetCharH(n) < y) continue;
            if (MANAGE_OBJ::GetCharY(n) > y + h) continue;
            unsigned long target = (unsigned long)((x - (MANAGE_OBJ::GetCharX(n) + MANAGE_OBJ::GetCharW(n))) &
                (MANAGE_OBJ::GetCharX(n) - (x + w)) &
                (y - (MANAGE_OBJ::GetCharY(n) + MANAGE_OBJ::GetCharH(n))) &
                (MANAGE_OBJ::GetCharY(n) - (y + h)));
            if (target >> 31) {
                index = n;
                return *(OBJs[n]);
            }
        }
    }
    return OBJ::NULLOBJ;
}

const int MANAGE_OBJ::GetCharTYPE(int n) { return OBJs[n] ? OBJs[n]->nTYPE : 0; };
const int MANAGE_OBJ::GetCharID(int n) { return OBJs[n] ? OBJs[n]->nCharID : 0; };
const int MANAGE_OBJ::GetCharAlpha(int n) { return OBJs[n] ? OBJs[n]->nAlpha : 0; };
const int MANAGE_OBJ::GetCharX(int n) { return OBJs[n] ? OBJs[n]->x : 0; };
const int MANAGE_OBJ::GetCharY(int n) { return OBJs[n] ? OBJs[n]->y : 0; };
const int MANAGE_OBJ::GetCharCX(int n) { return OBJs[n] ? OBJs[n]->cx : 0; };
const int MANAGE_OBJ::GetCharCY(int n) { return OBJs[n] ? OBJs[n]->cy : 0; };
const int MANAGE_OBJ::GetCharW(int n) { return OBJs[n] ? OBJs[n]->w : 0; };
const int MANAGE_OBJ::GetCharH(int n) { return OBJs[n] ? OBJs[n]->h : 0; };

void MANAGE_OBJ::Initialize(MANAGE_OBJ& m)
{
    OBJ::Set(OBJ::NULLOBJ, 0, 0, 0x7FFFFFFF, 0x7FFFFFFF, 0, 0, 0);
    for (int n = 0; n < SIZE; n++) {
        OBJs[n] = &OBJ::NULLOBJ;
    }
    InitOBJs(m);
}

bool MANAGE_OBJ::Process(int sx, int sy, int ex, int ey, const unsigned char* pKeys)
{
    MANAGE_OBJ::Move(sx, sy, ex, ey, pKeys);
    return true;
}
