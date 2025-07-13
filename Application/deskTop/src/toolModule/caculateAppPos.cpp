#include "caculateAppPos.h"

caculateAppPos::caculateAppPos()
    : curPage_(0)
{
}

caculateAppPos::~caculateAppPos()
{
}

void caculateAppPos::setCurPage(const int32_t &pageNum, const tpVector<tpIconTopButton *> &curPageAppList)
{
    curPage_ = pageNum;
    appList_ = curPageAppList;
}

void caculateAppPos::setRowY(const tpVector<int32_t> &yList)
{
}

void caculateAppPos::setColumnX(const tpVector<int32_t> &xList)
{
}

void caculateAppPos::moveBtn(const ItpPoint &curPoint)
{
}

void caculateAppPos::releaseBtn()
{
}
