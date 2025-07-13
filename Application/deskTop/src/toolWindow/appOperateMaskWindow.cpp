#include "appOperateMaskWindow.h"
#include "tpCanvas.h"

appOperateMaskWindow::appOperateMaskWindow()
    : tpDialog()
{
    setAlpha(204);
}

appOperateMaskWindow::~appOperateMaskWindow()
{
}

void appOperateMaskWindow::setAppRect(const ItpRect &rect, const uint32_t &roundCorners)
{
    appRect_ = rect;
    roundCorners_ = roundCorners;
    update();
}

bool appOperateMaskWindow::onMouseRleaseEvent(tpMouseEvent *event)
{
    close();
    return true;
}

bool appOperateMaskWindow::onPaintEvent(tpObjectPaintEvent *event)
{
    tpCanvas *painter = event->canvas();

    HollowMask hollowData;
    hollowData.addRoundRectHollow(appRect_, roundCorners_);
    // painter->hollowBox(0, 0, width(), height(), _RGBA(255, 255, 255, 204), hollowData);
    painter->hollowBox(0, 0, width(), height(), _RGB(255, 255, 255), hollowData);

    return true;
}
