#include "stdafx.h"
#include "World.h"

World::World(void) :nImageCnt(0) {}
World::~World(void) {}
void World::PrepareDraw()
{
    nImageCnt = 0;
    const int MapXE = MapX + W;
    const int MapYE = MapY + H;
    for (int n = 0; n < MANAGE_OBJ::GetObjCnt(); n++) {
        if (MANAGE_OBJ::GetCharTYPE(n) == 0) break;
        int lmxs = MANAGE_OBJ::GetCharX(n);
        if (lmxs <= (MapX + W)) {
            int lmxe = MANAGE_OBJ::GetCharX(n) + MANAGE_OBJ::GetCharW(n);
            if (MapX <= lmxe) {
                int lmys = MANAGE_OBJ::GetCharY(n);
                if (lmys <= (MapY + H)) {
                    int lmye = MANAGE_OBJ::GetCharY(n) + MANAGE_OBJ::GetCharH(n);
                    if (MapY <= lmye) {
                        if ((lmxs <= MapX && MapXE <= lmxe) ||
                            (MapX <= lmxs && lmxs <= MapXE) ||
                            (MapX <= lmxe && lmxe <= MapXE)) {
                            if ((lmys <= MapY && MapYE <= lmye) ||
                                (MapY <= lmys && lmys <= MapYE) ||
                                (MapY <= lmye && lmye <= MapYE)) {
                                int px = ToP((lmxs - MapX), D);
                                int py = ToP((lmys - MapY), D);
                                int dx = ToP(MANAGE_OBJ::GetCharW(n), D);
                                int dw = min(dx, (ToP(W, D) - px));
                                int dy = ToP(MANAGE_OBJ::GetCharH(n), D);
                                int dh = min(dy, (ToP(H, D) - py));
                                int ox = 0;
                                int oy = 0;
                                if (px < 0) {
                                    ox = -px;
                                    dx += px;
                                    dw = min(dx, (ToP(W, D)));
                                    px = 0;
                                }
                                if (py < 0) {
                                    oy = -py;
                                    dy += py;
                                    dh = min(dy, (ToP(H, D)));
                                    py = 0;
                                }
                                IMAGE::Set(mIMAGE[nImageCnt++], px, py, ox, oy, dw, dh, MANAGE_OBJ::GetCharID(n), MANAGE_OBJ::GetCharAlpha(n));
                            }
                        }
                    }
                }
            }
        }
    }
}

void World::Process(const unsigned char* pKeys)
{
    MANAGE_OBJ::Process(MapX, MapY, MapX + W, MapY + H, pKeys);
}

void World::Draw(void*)
{
    for (int n = 0; n < MANAGE_OBJ::GetObjCnt(); n++) {
        std::cout << n << "\t";
        std::cout << MANAGE_OBJ::GetCharX(n) << "\t";
        std::cout << MANAGE_OBJ::GetCharY(n) << "\t";
        std::cout << MANAGE_OBJ::GetCharW(n) << "\t";
        std::cout << MANAGE_OBJ::GetCharH(n) << std::endl;
    }
}
