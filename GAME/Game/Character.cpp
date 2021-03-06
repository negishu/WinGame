#include "Objects.h"
#include "ManageObjects.h"
#include "Character.h"

static unsigned long next = 1;
static inline unsigned long GetRand()
{
    next = next * 1103515245L + 12345L;
    return ((unsigned long)next);
}

class Character : public OBJ {
public:
    Character() :nCurCharID(0), nJump(0) {};
    int nCurCharID, nCharIDs[8][8], nCharIDCnts[8], nJump;
    void SetCharSeq(int id, int s0, int s1, int s2, int s3, int s4, int s5, int s6, int s7)
    {
        nCharIDs[id][0] = s0;nCharIDs[id][1] = s1;nCharIDs[id][2] = s2;nCharIDs[id][3] = s3;nCharIDs[id][4] = s4;nCharIDs[id][5] = s5;nCharIDs[id][6] = s6;nCharIDs[id][7] = s7;nCharIDCnts[id] = 8;
    }
    void SetCharSeq(int id, int s0, int s1, int s2, int s3, int s4, int s5, int s6)
    {
        nCharIDs[id][0] = s0;nCharIDs[id][1] = s1;nCharIDs[id][2] = s2;nCharIDs[id][3] = s3;nCharIDs[id][4] = s4;nCharIDs[id][5] = s5;nCharIDs[id][6] = s6;nCharIDCnts[id] = 7;
    }
    void SetCharSeq(int id, int s0, int s1, int s2, int s3, int s4, int s5)
    {
        nCharIDs[id][0] = s0;nCharIDs[id][1] = s1;nCharIDs[id][2] = s2;nCharIDs[id][3] = s3;nCharIDs[id][4] = s4;nCharIDs[id][5] = s5;nCharIDCnts[id] = 6;
    }
    void SetCharSeq(int id, int s0, int s1, int s2, int s3, int s4)
    {
        nCharIDs[id][0] = s0;nCharIDs[id][1] = s1;nCharIDs[id][2] = s2;nCharIDs[id][3] = s3;nCharIDs[id][4] = s4;nCharIDCnts[id] = 5;
    }
    void SetCharSeq(int id, int s0, int s1, int s2, int s3)
    {
        nCharIDs[id][0] = s0;nCharIDs[id][1] = s1;nCharIDs[id][2] = s2;nCharIDs[id][3] = s3;nCharIDCnts[id] = 4;
    }
    void SetCharSeq(int id, int s0, int s1, int s2)
    {
        nCharIDs[id][0] = s0;nCharIDs[id][1] = s1;nCharIDs[id][2] = s2;nCharIDCnts[id] = 3;
    }
    void SetCharSeq(int id, int s0, int s1)
    {
        nCharIDs[id][0] = s0;nCharIDs[id][1] = s1;nCharIDCnts[id] = 2;
    }
    void SetCharSeq(int id, int s0)
    {
        nCharIDs[id][0] = s0;nCharIDCnts[id] = 1;
    }
    virtual bool TryMove(MANAGE_OBJ& m, const unsigned char* pKeys = 0) { return false; };
};

class Block : public Character {
public:
    virtual bool TryMove(MANAGE_OBJ& m, const unsigned char* pKeys = 0)
    {
        dx += ax;
        dy += ay;
        nx = x + dx;
        ny = y + dy;
        return true;
    };
    static const class BrokenBlock mBrokenBlock[64];
};

class BrokenBlock : public Block {
};

class MAP : public Character {
public:
    MAP() {};
    virtual bool Init(MANAGE_OBJ& m)
    {
        int n = 0;
        {
            const int w = 16000, h = 18000;
            for (int x = 0; x < 120; x++, n++) {
                OBJ::Set(_OBJ[n], 182, OBJ::TYPE_FLOOR, (x * 48 * w), (32 * h), (48 * w), h, 255);
                m.Add(&_OBJ[n]);
            }
        }
        {
            const int w = 16000, h = 16000;
            for (int x = 0; x < 100; x++) {
                int y = ((18 * h) + (GetRand() % 15) * h) + (GetRand() % h);
                for (int a = 0; a < 10; a++, n++) {
                    OBJ::Set(_OBJ[n], 158, OBJ::TYPE_BLOCK, ((x * 10 + a) * w), y, w, h, 255);
                    m.Add(&_OBJ[n]);
                }
            }
        }
        {
            const int w = 16000, h = 16000;
            for (int x = 0; x < 500; x++, n++) {
                int y = 4 * h;
                OBJ::Set(_OBJ[n], x % 261, OBJ::TYPE_BLOCK, (x * w), y, w, h, 255);
                //m.Add(&_OBJ[n]);
            }
        }
        return true;
    }
private:
    static const int SIZE = 4096;
    Block _OBJ[SIZE];
};

class Mario : public Character {
public:
    virtual bool Init(MANAGE_OBJ& m)
    {
        OBJ::Set(*this, 70, OBJ::TYPE_MARIO, ToL(300, D), ToL(300, D), ToL(16, D), ToL(16, D));
        Character::SetCharSeq(0, 73, 72, 73, 74);
        Character::SetCharSeq(1, 73 + 0x1000, 72 + 0x1000, 73 + 0x1000, 74 + 0x1000);
        Character::SetCharSeq(2, 70);
        Character::SetCharSeq(3, 70 + 0x1000);
        Character::SetCharSeq(4, 69);
        Character::SetCharSeq(5, 69 + 0x1000);
        Character::SetCharSeq(6, 71);
        Character::SetCharSeq(7, 71 + 0x1000);
        m.Add(this);
        nCurCharID = 0;
        return true;
    }
    virtual bool TryMove(MANAGE_OBJ& m, const unsigned char* pKeys = 0)
    {
        ax = 0; ay = 500;

        if (pKeys[0x27]) {
            if (dx < 0)
                ax = +500;
            else if (dx < +6000)
                ax = +500;
        }
        else
            if (pKeys[0x25]) {
                if (dx > 0)
                    ax = -500;
                else if (dx > -6000)
                    ax = -500;
            }
            else {
                if (nJump == 0) {
                    dx *= 235;
                    dx /= 255;
                }
            }

            if ((pKeys[0x20] || pKeys[0x26])) {
                if (nJump == 0) {
                    ay = -2000;
                    nJump = 32;
                }
                else {
                    if (dy < 0) {
                        if (nJump > 1) {
                            ay = -(2000 / (32 - nJump));
                        }
                    }
                }
            }

            if ((pKeys[0x28])) {
                if (dy <= 0) dy = 0;
                ay = +1000;
            }

            if (nJump > 1) nJump--;

            dx += ax;
            dy += ay;

            cx = x;
            cy = y + dy;

            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_FLOOR, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (dy > 0) nJump = 0;
                    if (obj.y >= y) { cy = obj.y - h; }
                    if (obj.y <= y) { cy = obj.y + obj.h; }
                    dy = 0;
                }
                else break;
            }

            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_KAME, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (dy > 0) nJump = 0;
                    if (obj.y > y) { cy = obj.y - h; dy = -dy; dy *= 1; dy /= 16; }
                    if (obj.y < y) { cy = obj.y + obj.h; }
                    dy = 0;
                }
                else break;
            }

            for (int n = 0;;n++) {
                OBJ& obj = (OBJ&)m.GetObj(this, OBJ::TYPE_BLOCK, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (dy > 0) nJump = 0;
                    if (dy < 0 || (dy > 16000 && pKeys[0x28])) {
                        static int c = 0; c++; c %= 16;
                        OBJ::Set(mBrokenBlock[c * 4 + 0], 158, OBJ::TYPE_BROKENBLOCK, obj.x, obj.y, obj.w / 2, obj.h / 2, 255);
                        mBrokenBlock[c * 4 + 0].SetTarget(+dx / 8 - 2000, dy, 0, +400);
                        m.Add(&mBrokenBlock[c * 4 + 0]);
                        OBJ::Set(mBrokenBlock[c * 4 + 1], 158, OBJ::TYPE_BROKENBLOCK, obj.x + obj.w / 2, obj.y, obj.w / 2, obj.h / 2, 255);
                        mBrokenBlock[c * 4 + 1].SetTarget(+dx / 8 + 2000, dy, 0, +400);
                        m.Add(&mBrokenBlock[c * 4 + 1]);
                        OBJ::Set(mBrokenBlock[c * 4 + 2], 158, OBJ::TYPE_BROKENBLOCK, obj.x, obj.y + obj.h / 2, obj.w / 2, obj.h / 2, 255);
                        mBrokenBlock[c * 4 + 2].SetTarget(+dx / 8 - 3000, dy, 0, +500);
                        m.Add(&mBrokenBlock[c * 4 + 2]);
                        OBJ::Set(mBrokenBlock[c * 4 + 3], 158, OBJ::TYPE_BROKENBLOCK, obj.x + obj.w / 2, obj.y + obj.h / 2, obj.w / 2, obj.h / 2, 255);
                        mBrokenBlock[c * 4 + 3].SetTarget(-dx / 8 + 3000, dy, 0, +500);
                        m.Add(&mBrokenBlock[c * 4 + 3]);
                        OBJ::Set(obj, obj.nCharID, obj.nTYPE, 0, 0, 0, 0);
                    }
                }
                else { if (n) dy = 0; break; }
            }

            for (int n = 0;;n++) {
                OBJ& obj = (OBJ&)m.GetObj(this, OBJ::TYPE_BLOCK, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.y > y) cy = obj.y - h;
                    if (obj.y < y) dy = -dy;//cy = obj.y + obj.h;
                    break;
                }
                else { break; }
            }

            ny = cy;

            cy = y;
            cx = x + dx;

            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_KAME, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.x > x) cx = obj.x - w;
                    if (obj.x < x) cx = obj.x + obj.w;
                }
                else break;
            }
            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_FLOOR, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.x > x) cx = obj.x - w;
                    if (obj.x < x) cx = obj.x + obj.w;
                }
                else break;
            }
            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_BLOCK, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.x > x) { cx = obj.x - w; dx = -(dx * 2); }
                    if (obj.x < x) { cx = obj.x + obj.w; dx = -(dx * 2); }
                }
                else { if (n) dx = 0; break; }
            }

            nx = cx;

            if (dy != 0) {
                if (nCurCharID == 0 || nCurCharID == 4) nCurCharID = 2;
                if (nCurCharID == 1 || nCurCharID == 5) nCurCharID = 3;
            }
            else {
                if (ax > 0 || nCurCharID == 2) {
                    nCurCharID = 7;
                    if (dx > 0) nCurCharID = 0;
                }
                if (ax < 0 || nCurCharID == 3) {
                    nCurCharID = 6;
                    if (dx < 0) nCurCharID = 1;
                }
                if (dx == 0) {
                    if (nCurCharID == 0 || nCurCharID == 2 || nCurCharID == 7) nCurCharID = 4;
                    if (nCurCharID == 1 || nCurCharID == 3 || nCurCharID == 6) nCurCharID = 5;
                }
            }

            nCharID = nCharIDs[nCurCharID][(OBJ::CNT) % nCharIDCnts[nCurCharID]];

            if (pKeys['Z']) {

                OBJ::Set(*this, 70, OBJ::TYPE_MARIO, ToL(300, D), ToL(300, D), ToL(32, D), ToL(32, D));
            }

            return true;
    }
    BrokenBlock mBrokenBlock[64];
};

class Kame : public Character {
public:
    virtual bool Init(MANAGE_OBJ& m)
    {
        OBJ::Set(*this, 1, OBJ::TYPE_KAME, GetRand() % (ToL(16, D) * 1024), GetRand() % (ToL(16, D) * 32), ToL(16, D), ToL(24, D));
        Character::SetCharSeq(0, 62 + 0x1000, 63 + 0x1000);
        Character::SetCharSeq(1, 62, 63);
        nCurCharID = 0;
        nAlpha = 255;
        m.Add(this);
        return true;
    }

    virtual bool TryMove(MANAGE_OBJ& m, const unsigned char* pKeys = 0)
    {
        dx *= 64;
        dx /= 256;
        dy *= 251;
        dy /= 256;

        ay = 300;

        if (GetRand() % 1000 < 10) {
            if (dx < 0)
                ax = +2000;
            else if (dx < +3000)
                ax = +1000;
        }
        else
            if (GetRand() % 1000 < 10) {
                if (dx > 0)
                    ax = -2000;
                else if (dx > -3000)
                    ax = -1000;
            }
            else {
                dx *= 215;
                dx /= 255;
            }

            dx += ax;
            dy += ay;

            cx = x;
            cy = y + dy;

            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_FLOOR, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (dy > 0) nJump = false;
                    if (obj.y > y) { cy = obj.y - h; }
                    if (obj.y < y) { cy = obj.y + obj.h; }
                    dy = 0;
                }
                else break;
            }
            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_MARIO, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.y > y) cy = obj.y - h;
                    if (obj.y < y) cy = obj.y + obj.h;
                    dy = 0;
                }
                else break;
            }
            for (int n = 0;;n++) {
                OBJ& obj = (OBJ&)m.GetObj(this, OBJ::TYPE_BLOCK, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.y > y) cy = obj.y - h;
                    if (obj.y < y) cy = obj.y + obj.h;
                    dy = 0;
                }
                else break;
            }

            cx = x + dx;

            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_KAME, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.x > x) cx = obj.x - w;
                    if (obj.x < x) cx = obj.x + obj.w;
                }
                else break;
            }
            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_FLOOR, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.x > x) cx = obj.x - w;
                    if (obj.x < x) cx = obj.x + obj.w;
                }
                else break;
            }
            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_BLOCK, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.x > x) cx = obj.x - w;
                    if (obj.x < x) cx = obj.x + obj.w;
                }
                else break;
            }
            for (int n = 0;;n++) {
                const OBJ& obj = m.GetObj(this, OBJ::TYPE_MARIO, n);
                if (&obj != &OBJ::NULLOBJ) {
                    if (obj.x > x) cx = obj.x - w;
                    if (obj.x < x) cx = obj.x + obj.w;
                }
                else break;
            }

            nx = cx;
            ny = cy;

            if (dy != 0) {
            }
            else {
                if (dx > 0) nCurCharID = 0;
                if (dx < 0) nCurCharID = 1;
            }

            nCharID = nCharIDs[nCurCharID][(OBJ::CNT / 10) % nCharIDCnts[nCurCharID]];

            return true;
    }
};

static OBJ& GetMAP()
{
    static MAP theMap;
    return theMap;
}

static OBJ& GetKame(int n)
{
    static const int KAMEMAX = 512;
    static Kame theKames[KAMEMAX];
    if (0 <= n && n < KAMEMAX) {
        return theKames[n];
    }
    return theKames[0];
}

static OBJ& GetMario()
{
    static Mario theMario;
    return theMario;
}

void InitOBJs(MANAGE_OBJ& m)
{
    for (int n = 0; n < 4096; n++) {
        switch (n) {
        case 0:
            GetMario().Init(m);
            break;
        case 1:
            GetMAP().Init(m);
            break;
        default:
            GetKame(n - 2).Init(m);
            break;
        }
    }
}

const OBJ& GetMainCharacter()
{
    return GetMario();
}
