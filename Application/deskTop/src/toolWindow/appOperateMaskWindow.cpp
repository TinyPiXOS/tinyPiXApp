#include "appOperateMaskWindow.h"
#include "TpCanvas.h"

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

bool appOperateMaskWindow::onPaintEvent(TpObjectPaintEvent *event)
{
    TpCanvas *painter = event->canvas();

    HollowMask hollowData;
    hollowData.addRectHollow(appRect_, roundCorners_);
    // painter->hollowBox(0, 0, width(), height(), _RGBA(255, 255, 255, 204), hollowData);
    painter->box(0, 0, width(), height(), _RGB(255, 255, 255), hollowData);

    return true;
}
