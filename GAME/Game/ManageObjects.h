#pragma once
class OBJ;
class MANAGE_OBJ {
public:
    bool Add(OBJ* obj);
    const OBJ& GetObj(const OBJ* obj, int nCheckType, int& index);
protected:
    MANAGE_OBJ();
    virtual void Initialize(MANAGE_OBJ&);
    bool Process(int sx, int sy, int ex, int ey, const unsigned char* pKeys);
    const int GetObjCnt() { return nCnt; };
    const int GetCharTYPE(int n);
    const int GetCharAlpha(int n);
    const int GetCharID(int n);
    const int GetCharX(int n);
    const int GetCharY(int n);
    const int GetCharCX(int n);
    const int GetCharCY(int n);
    const int GetCharW(int n);
    const int GetCharH(int n);
    int W, H, MapX, MapY;
private:
    bool TryMove(int sx, int sy, int ex, int ey, const unsigned char* pKeys);
    bool Move(int sx, int sy, int ex, int ey, const unsigned char* pKeys);
    bool TryMove(int n, const unsigned char* pKeys);
    void Swap(int i, int j);
    void Sort(int left, int right);
    int nCnt;
    static const int SIZE = 2048;
    OBJ* OBJs[SIZE];
};
