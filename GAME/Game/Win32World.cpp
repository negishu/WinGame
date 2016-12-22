#include "stdafx.h"
#include "Win32World.h"

void Win32World::DebugDraw(void* hdc)
{
}

void Win32World::Draw(void* p)
{	
	GRAPHIC::DIBPlane * const pDIBPlane = (GRAPHIC::DIBPlane*)p;

	int MapXE = MapX + ToL(W,D);
	int MapYE = MapY + ToL(H,D);

	for (int n = 0; n < mMAP.SIZE; n++) {
		
		if (mMAP._B[n].n == 0) break;

		int DX = ToL(32,D);
		int DY = ToL(32,D);

		int lmxs = ToL((mMAP._B[n].x + 0), D);
		int lmxe = ToL((mMAP._B[n].x + mMAP._B[n].w), D);		
		int lmys = ToL((mMAP._B[n].y + 0), D);
		int lmye = ToL((mMAP._B[n].y + mMAP._B[n].h), D);

		if (lmxs <= (MapX + ToL(W,D)) && MapX <= lmxe) {

			if (lmys <= (MapY + ToL(H,D)) && MapY <= lmye) {

				for (int y = 0, ey = ToL(mMAP._B[n].h, D); y < ey; y += DY) {

					if (MapY <= (lmys + y + DY) && (lmys + y) <= MapYE) {
				
						for (int x = 0, ex = ToL(mMAP._B[n].w, D); x < ex; x += DX) {

							if (MapX <= (lmxs + x + DX) && (lmxs + x) <= MapXE) {

								int px = ToP((lmxs - MapX + x), D);
								int py = ToP((lmys - MapY + y), D);
																	
								int dx = 32;
								int dw = min(dx,(W-px));
								int dy = 32;
								int dh = min(dy,(H-py));
						
								int ox = dx - ((px - 0) % dx);
								if (px >= 0) ox = 0;
								int oy = dy - ((py - 0) % dy);
								if (py >= 0) oy = 0;
								
								theThings.DrawR(pDIBPlane, px, py, ox/2, oy/2, dw/2, dh/2, mMAP._B[n].n);
							}
						}
					}
				}
			}
		}
	}

	for (int n = 0; n < KAMEMAX; n++) {
		int DX = ToL(32,D);
		int DY = ToL(32,D);
		int lmxs = mKame[n].x;
		int lmxe = mKame[n].x + mKame[n].w;
		int lmys = mKame[n].y;
		int lmye = mKame[n].y + mKame[n].h;
		if (lmxs <= (MapX + ToL(W,D)) && MapX <= lmxe) {
			if (lmys <= (MapY + ToL(H,D)) && MapY <= lmye) {
				for (int y = 0, ey = mKame[n].h; y < ey; y += DY) {
					if (MapY <= (lmys + y + DY) && (lmys + y) <= MapYE) {
						for (int x = 0, ex = mKame[n].w; x < ex; x += DX) {
							if (MapX <= (lmxs + x + DX) && (lmxs + x) <= MapXE) {
								int px = ToP((lmxs - MapX + x), D);
								int py = ToP((lmys - MapY + y), D);
								int dx = ToP(mKame[n].w,D);
								int dw = min(dx,(W-px));
								int dy = ToP(mKame[n].h,D);
								int dh = min(dy,(H-py));
								int ox = dx - ((px - 0) % dx);
								if (px >= 0) ox = 0;
								int oy = dy - ((py - 0) % dy);
								if (py >= 0) oy = 0;
								if (mKame[n].info < 0x1000) {
									theThings.DrawR(pDIBPlane, px, py, ox/2, oy/2, dw/2, dh/2, mKame[n].info);
								}
								else {
									theThings.DrawL(pDIBPlane, px, py, ox/2, oy/2, dw/2, dh/2, mKame[n].info-0x1000);
								}
							}
						}
					}
				}
			}
		}
	}

	if (mMario.info < 0x1000) {
		theThings.DrawR(pDIBPlane, ToP((mMario.x-MapX),D), ToP(mMario.y,D), 0, 0, 16, 16, mMario.info);
	}
	else {
		theThings.DrawL(pDIBPlane, ToP((mMario.x-MapX),D), ToP(mMario.y,D), 0, 0, 16, 16, mMario.info-0x1000);
	}
}
