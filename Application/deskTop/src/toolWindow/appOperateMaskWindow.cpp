#include "appOperateMaskWindow.h"
#include "TpPainter.h"

appOperateMaskWindow::appOperateMaskWindow()
    : TpDialog()
{
    setAlpha(204);
    setVisible(false);
}

appOperateMaskWindow::~appOperateMaskWindow()
{
}

void appOperateMaskWindow::setAppRect(const TpRect &rect, const uint32_t &roundCorners)
{
    appRect_ = rect;
    roundCorners_ = roundCorners;
    update();
}

bool appOperateMaskWindow::onMouseRleaseEvent(TpMouseEvent *event)
{
    close();
    return true;
}

bool appOperateMaskWindow::onPaintEvent(TpPaintEvent *event)
{
    TpPainter *painter = event->canvas();

    TpHollowMask hollowData;
    hollowData.addRectHollow(appRect_, roundCorners_);
    // painter->hollowBox(0, 0, width(), height(), _RGBA(255, 255, 255, 204), hollowData);

    painter->setPen(_RGB(255, 255, 255));
    painter->setBrush(TpBrush(_RGB(255, 255, 255)));
    painter->drawRect(0, 0, width(), height(), 0, hollowData);

    return true;
}
