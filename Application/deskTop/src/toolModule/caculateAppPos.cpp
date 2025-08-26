#include "caculateAppPos.h"

caculateAppPos::caculateAppPos()
    : curPage_(0)
{
}

caculateAppPos::~caculateAppPos()
{
}

void caculateAppPos::setCurPage(const int32_t &pageNum, const TpVector<TpIconTopButton *> &curPageAppList)
{
    curPage_ = pageNum;
    appList_ = curPageAppList;
}

void caculateAppPos::setRowY(const TpVector<int32_t> &yList)
{
}

void caculateAppPos::setColumnX(const TpVector<int32_t> &xList)
{
}

void caculateAppPos::moveBtn(const ItpPoint &curPoint)
{
}

void caculateAppPos::releaseBtn()
{
}
