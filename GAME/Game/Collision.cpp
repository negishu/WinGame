#include "Collision.h"

CollisionManager::CollisionManager()
{
    m_nW = 0;
    m_nH = 0;
    m_nUnit_W = 0;
    m_nUnit_H = 0;
    m_nCellNum = 0;
    m_iPow[0] = 1;
    for (int i = 1; i < CLINER4TREEMANAGER_MAXLEVEL + 1; i++) m_iPow[i] = m_iPow[i - 1] * 4;
    for (int i = 0; i < CLINER4TREEMANAGER_MAXCELL; i++) {
        for (int n = 0; n < CLINER4TREEMANAGER_MAXOBJS; n++) {
            m_cells[i][n] = 0;
        }
        m_cellscnt[i] = -1;
    }
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::Init(int nWidth, int nHeight, int nLevel)
{
    nLevel = nLevel < 0 ? 4 : nLevel;
    m_uiLevel = nLevel < CLINER4TREEMANAGER_MAXLEVEL ? nLevel : 4;
    m_nCellNum = (m_iPow[m_uiLevel + 1] - 1) / 3;
    m_nW = (nWidth >> m_uiLevel) << m_uiLevel;
    m_nH = (nHeight >> m_uiLevel) << m_uiLevel;
    m_nW = m_nW < (1 << m_uiLevel) ? (1 << m_uiLevel) : m_nW;
    m_nH = m_nH < (1 << m_uiLevel) ? (1 << m_uiLevel) : m_nH;
    m_nUnit_W = m_nW / (1 << m_uiLevel);
    m_nUnit_H = m_nH / (1 << m_uiLevel);
    return true;
}

void CollisionManager::Clear()
{
    for (int i = 0; i < CLINER4TREEMANAGER_MAXCELL; i++) {
        m_cellscnt[i] = -1;
    }
}

bool CollisionManager::Regist(int left, int top, int right, int bottom, int id)
{
    unsigned long Elem = GetMortonNumber(left, top, right, bottom);

    if (Elem < m_nCellNum) {

        if (m_cellscnt[Elem] == -1) m_cellscnt[Elem]++;

        m_cells[Elem][m_cellscnt[Elem]] = id;

        m_cellscnt[Elem]++;

        while (Elem <= m_nCellNum) {

            Elem = (Elem - 1) >> 2;

            if (Elem >= m_nCellNum) break;

            if (m_cellscnt[Elem] == -1) {

                m_cellscnt[Elem]++;
            }
        }

        return true;
    }

    return false;
}

int CollisionManager::GetCollisionList(int nCurElem, List& aList, Stack& aStack)
{
    int num = m_cellscnt[nCurElem];

    if (num > 0) {

        for (int i = 0; i < num; i++)
        {
            for (int j = i + 1; j < num; j++)
            {
                aList.aPair[aList.nCnt].objs1 = m_cells[nCurElem][i];
                aList.aPair[aList.nCnt].objs2 = m_cells[nCurElem][j];
                aList.aPair[aList.nCnt].elem = nCurElem;

                aList.nCnt++;
            }

            for (int j = 0; j < aStack.nCnt; j++)
            {
                aList.aPair[aList.nCnt].objs1 = m_cells[nCurElem][i];
                aList.aPair[aList.nCnt].objs2 = aStack.objs[j];
                aList.aPair[aList.nCnt].elem = nCurElem;

                aList.nCnt++;
            }
        }
    }

    bool ChildFlag = false;

    int nObjNum = 0;

    for (int i = 0; i < 4; i++) {

        unsigned long nNextElem = (nCurElem * 4 + 1 + i);

        if (nNextElem < m_nCellNum && m_cellscnt[nNextElem] >= 0) {

            if (!ChildFlag) {

                for (int i = 0; i < num; i++) {

                    aStack.objs[aStack.nCnt++] = m_cells[nCurElem][i];

                    nObjNum++;
                }
            }

            ChildFlag = true;

            GetCollisionList(nNextElem, aList, aStack);
        }
    }

    if (ChildFlag) {

        aStack.nCnt -= nObjNum;
    }

    return aList.nCnt;
}

int CollisionManager::GetAllCollisionList()
{
    theStack.nCnt = 0;
    theList.nCnt = 0;

    int m = 0, nCnt = GetCollisionList(0, theList, theStack);

    for (int i = 0; i < nCnt; i++) {

        if (m_cellscnt[i] >= 0) {

            m += m_cellscnt[i];
        }
    }

    return nCnt;
}

unsigned long CollisionManager::GetMortonNumber(int left, int top, int right, int bottom)
{
    unsigned long LT = GetPointElem(left, top);
    unsigned long RB = GetPointElem(right, bottom);
    unsigned long Def = RB ^ LT;
    unsigned int HiLevel = 0;
    for (int i = 0; i < m_uiLevel; i++) {
        unsigned long Check = (Def >> (i * 2)) & 0x3;
        if (Check != 0) {
            HiLevel = i + 1;
        }
    }
    unsigned long SpaceNum = RB >> (HiLevel * 2);
    unsigned long AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 3;
    SpaceNum += AddNum;
    if (SpaceNum > m_nCellNum) return 0;
    return SpaceNum;
}
