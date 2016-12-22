#include "stdafx.h"
#include "GraphicBase.h"

namespace GRAPHIC {

    DIBPlane::DIBPlane() :m_hBitMap(NULL), m_hDC(NULL), m_pBuffer(0)
    {
    }

    DIBPlane::~DIBPlane()
    {
        Destroy();
    }

    void DIBPlane::Destroy()
    {
        if (m_hBitMap) DeleteObject(m_hBitMap);
        m_hBitMap = NULL;
        if (m_hDC) DeleteDC(m_hDC);
        m_hDC = NULL;
    }

    BOOL DIBPlane::Create(DWORD dwWidth, DWORD dwHeight)
    {
        DIBPlane::Destroy();
        m_dwBpp = 32;
        m_dwWidth = ((dwWidth + 15) / 4) * 4 + 64;
        m_dwHeight = ((dwHeight + 15) / 4) * 4 + 64;
        m_dwPitch = m_dwWidth * 4;
        m_dwSIZE = m_dwPitch * m_dwHeight;
        memset(&m_BitmapInfo, 0, sizeof(m_BitmapInfo));
        m_BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        m_BitmapInfo.bmiHeader.biWidth = m_dwWidth;
        m_BitmapInfo.bmiHeader.biHeight = -(long)m_dwHeight;
        m_BitmapInfo.bmiHeader.biPlanes = 1;
        m_BitmapInfo.bmiHeader.biBitCount = (WORD)m_dwBpp;
        m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
        HDC hdc = ::GetDC(NULL);
        m_hBitMap = (HBITMAP)CreateDIBSection(hdc, &m_BitmapInfo, DIB_RGB_COLORS, (VOID **)&m_pBuffer, NULL, 0);
        ::ReleaseDC(NULL, hdc);
        if (m_hBitMap == NULL) return FALSE;
        m_hDC = ::CreateCompatibleDC(NULL);
        ::SelectObject(m_hDC, m_hBitMap);
        memset(m_pBuffer, 0, m_dwSIZE);

        return TRUE;
    }

    void DIBPlane::Erase()
    {
        if (m_pBuffer) {
            for (int n = 0, m = m_dwSIZE; n < m; n++) {
                int p = ((BYTE*)m_pBuffer)[n] * 200;
                ((BYTE*)m_pBuffer)[n] = p / 1024;
            }
        }
    }

    void DIBPlane::BitBlt(HDC hdc, const DWORD rop)
    {
        ::StretchDIBits(hdc, 0, 0, m_dwWidth - 64, m_dwHeight - 64, 32, 32, m_dwWidth - 64, m_dwHeight - 64, m_pBuffer, &m_BitmapInfo, DIB_RGB_COLORS, rop);
        Erase();
    }

    void DIBPlane::DrawL(unsigned char* pData, int size, int px, int py, int ox, int oy, int w, int h, int alpha)
    {
        BYTE* pp = (BYTE*)m_pBuffer;
        BYTE* pc = (BYTE*)pData;

        if (pp) {

            for (INT i = 0; i < h; i++)
            {
                for (INT j = 0; j < w; j++)
                {
                    if (0 <= px + j && px + j < m_dwWidth) {

                        BYTE* pdata = pc + (15 * 4) - ((((j + ox)) % 16) * 4) + (((i + oy)) * (16 * 4));

                        INT mx = 0, my = 0;
                        INT nOffset = ((px + mx) * 4) + (j * 4) + ((py + my) * m_dwPitch) + (i * m_dwPitch);

                        if (nOffset < 0 || (nOffset + m_dwPitch) > m_dwSIZE) break;

                        BYTE p0 = (*(pdata + 0) * alpha) / 255;
                        BYTE p1 = (*(pdata + 1) * alpha) / 255;
                        BYTE p2 = (*(pdata + 2) * alpha) / 255;
                        BYTE p3 = (*(pdata + 3) * alpha) / 255;

                        UINT color = (p3 << 24) + (p2 << 16) + (p1 << 8) + p0;

                        if (color != 0) {
                            UINT* ds_color = (UINT*)(pp + nOffset);
                            *(ds_color + 0) = color;
                        }
                    }
                }
            }
        }
    }

    void DIBPlane::DrawR(unsigned char* pData, int size, int px, int py, int ox, int oy, int w, int h, int alpha)
    {
        BYTE* pp = (BYTE*)m_pBuffer;
        BYTE* pc = (BYTE*)pData;

        if (pp) {

            for (INT i = 0; i < h; i++)
            {
                for (INT j = 0; j < w; j++)
                {
                    if (0 <= px + j && px + j < m_dwWidth) {

                        BYTE* pdata = pc + ((((j + ox)) % 16) * 4) + (((i + oy)) * (16 * 4));

                        INT mx = 0, my = 0;
                        INT nOffset = ((px + mx) * 4) + (j * 4) + ((py + my) * m_dwPitch) + (i * m_dwPitch);

                        if (nOffset < 0 || (nOffset + m_dwPitch) > m_dwSIZE) break;

                        BYTE p0 = (*(pdata + 0) * alpha) / 255;
                        BYTE p1 = (*(pdata + 1) * alpha) / 255;
                        BYTE p2 = (*(pdata + 2) * alpha) / 255;
                        BYTE p3 = (*(pdata + 3) * alpha) / 255;

                        UINT color = (p3 << 24) + (p2 << 16) + (p1 << 8) + p0;

                        if (color != 0) {
                            UINT* ds_color0 = (UINT*)(pp + nOffset);
                            *(ds_color0 + 0) = color;
                        }
                    }
                }
            }
        }
    }

#include "data.h"

    void Things::Init()
    {
        InitChar();
    }

    void Things::DrawR(GRAPHIC::DIBPlane* pDIBPlane, int px, int py, int ox, int oy, int w, int h, int id, int alpha)
    {
        pDIBPlane->DrawR(mData[id], sizeof(mData[id]), px, py, ox, oy, w, h, alpha);
    }

    void Things::DrawL(GRAPHIC::DIBPlane* pDIBPlane, int px, int py, int ox, int oy, int w, int h, int id, int alpha)
    {
        pDIBPlane->DrawL(mData[id], sizeof(mData[id]), px, py, ox, oy, w, h, alpha);
    }

    void Things::DrawBox(GRAPHIC::DIBPlane* pDIBPlane, int px, int py, int x, int y, int w, int h, int id, int alpha)
    {
        HDC hdc = pDIBPlane->GetDC();
        ::SelectObject(hdc, GetStockObject(NULL_BRUSH));
        ::SelectObject(hdc, GetStockObject(WHITE_PEN));
        ::SetTextColor(hdc, RGB(255, 255, 255));
        char text[64]; sprintf(text, "%d", id);
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextAlign(hdc, TA_CENTER);
        ::TextOutA(hdc, px + w, py + h, text, ::strlen(text));
        ::Rectangle(hdc, px, py, px + w, py + h);
    }

} // end of GRAPHIC
