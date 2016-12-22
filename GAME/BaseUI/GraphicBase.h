#pragma once
namespace GRAPHIC {
    class DIBPlane
    {
    private:
        HBITMAP	m_hBitMap;
        HDC		m_hDC;
        void*  m_pBuffer;
        DWORD  m_dwWidth;
        DWORD  m_dwHeight;
        DWORD  m_dwPitch;
        DWORD  m_dwSIZE;
        DWORD  m_dwBpp;
        BITMAPINFO m_BitmapInfo;
    public:
        DIBPlane();
        virtual ~DIBPlane();
        BOOL Create(DWORD dwWidth, DWORD dwHeight);
        void Destroy();
        void BitBlt(HDC hdc, const DWORD rop);
        void DrawR(unsigned char* pData, int size, int px, int py, int ox, int oy, int w, int h, int alpha);
        void DrawL(unsigned char* pData, int size, int px, int py, int ox, int oy, int w, int h, int alpha);
        void Erase();
        inline HDC GetDC() { return m_hDC; };
    };
    class Things {
    public:
        Things() { Init(); };
        virtual ~Things() {};
        void DrawR(GRAPHIC::DIBPlane* pDIBPlane, int px, int py, int ox, int oy, int w, int h, int id, int alpha);
        void DrawL(GRAPHIC::DIBPlane* pDIBPlane, int px, int py, int ox, int oy, int w, int h, int id, int alpha);
        void DrawBox(GRAPHIC::DIBPlane* pDIBPlane, int px, int py, int x, int y, int w, int h, int id, int alpha);
    private:
        void Init();
        void InitChar();
        BYTE* mData[512];
    };
} // end of GRAPHIC
