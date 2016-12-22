#pragma once

#define CLINER4TREEMANAGER_MAXLEVEL 5
#define CLINER4TREEMANAGER_MAXCELL  341
#define CLINER4TREEMANAGER_MAXOBJS  1024

class CollisionManager
{
public:
    static inline CollisionManager& GetCollisionManager()
    {
        static CollisionManager theCollisionManager;
        return theCollisionManager;
    }

protected:

    unsigned long m_nCellNum;
    int m_uiLevel;
    int m_iPow[CLINER4TREEMANAGER_MAXLEVEL + 1];

    int m_nW;
    int m_nH;
    int m_nUnit_W;
    int m_nUnit_H;

    short m_cells[CLINER4TREEMANAGER_MAXCELL][CLINER4TREEMANAGER_MAXOBJS];
    short m_cellscnt[CLINER4TREEMANAGER_MAXCELL];

    class Stack {
    public:
        Stack() :nCnt(0) {};
        int nCnt;
        short objs[CLINER4TREEMANAGER_MAXOBJS * 8];
    };

    class List {
    public:
        List() :nCnt(0) {};
        class Pair {
        public:
            short objs1, objs2, elem;
        };
        int nCnt;
        Pair aPair[CLINER4TREEMANAGER_MAXOBJS * 256];
    };

    Stack theStack;
    List  theList;

    CollisionManager();
    virtual ~CollisionManager();

public:

    bool Init(int nWidth, int nHeight, int nLevel);

    void Clear();

    bool Regist(int left, int top, int right, int bottom, int id);

    int GetCollisionList(int nCurElem, List& aList, Stack& aStack);

    int GetAllCollisionList();

    unsigned long GetMortonNumber(int left, int top, int right, int bottom);

protected:
    inline unsigned long Morton_2D_Encode_16bit(unsigned short index1, unsigned short index2)
    {
        index1 &= 0x0000ffff;
        index2 &= 0x0000ffff;
        index1 |= (index1 << 8);
        index2 |= (index2 << 8);
        index1 &= 0x00ff00ff;
        index2 &= 0x00ff00ff;
        index1 |= (index1 << 4);
        index2 |= (index2 << 4);
        index1 &= 0x0f0f0f0f;
        index2 &= 0x0f0f0f0f;
        index1 |= (index1 << 2);
        index2 |= (index2 << 2);
        index1 &= 0x33333333;
        index2 &= 0x33333333;
        index1 |= (index1 << 1);
        index2 |= (index2 << 1);
        index1 &= 0x55555555;
        index2 &= 0x55555555;
        return(index1 | (index2 << 1));
    }
    inline unsigned long Get2DMortonNumber(unsigned short x, unsigned short y)
    {
        return Morton_2D_Encode_16bit(x, y);
    }
    inline unsigned long GetPointElem(int pos_x, int pos_y)
    {
        return Get2DMortonNumber((unsigned short)((pos_x) / m_nUnit_W), (unsigned short)((pos_y) / m_nUnit_H));
    }
};
