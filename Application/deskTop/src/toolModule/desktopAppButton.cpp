#include "desktopAppButton.h"
#include "tpCanvas.h"

desktopAppButton::desktopAppButton(tpChildWidget *parent)
    : tpIconTopButton(parent)
{
    hollowWidget_ = new hollowWidget(this);
    hollowWidget_->setVisible(false);
}

desktopAppButton::desktopAppButton(const tpString &iconPath, const tpString &text, tpChildWidget *parent)
    : tpIconTopButton(iconPath, text, parent)
{
    hollowWidget_ = new hollowWidget(this);
    hollowWidget_->setVisible(false);
}

desktopAppButton::~desktopAppButton()
{
    hollowWidget_->deleteLater();
}

void desktopAppButton::setRoundCorners(const uint32_t &round)
{
    tpIconTopButton::setRoundCorners(round);
    hollowWidget_->setRoundCorners(round);
}

void desktopAppButton::setInstallProgress(const uint32_t &progress)
{
    hollowWidget_->setInstallProgress(progress);

    if (progress == 100)
        hollowWidget_->setVisible(false);
    else
        hollowWidget_->setVisible(true);
}

bool desktopAppButton::onResizeEvent(tpObjectResizeEvent *event)
{
    tpIconTopButton::onResizeEvent(event);

    hollowWidget_->setSize(width(), width());

    return true;
}

hollowWidget::hollowWidget(tpChildWidget *parent)
    : tpChildWidget(parent), installProgress_(100)
{
}

hollowWidget::~hollowWidget()
{
}

void hollowWidget::setInstallProgress(const uint32_t &progress)
{
    installProgress_ = progress;
}

bool hollowWidget::onPaintEvent(tpObjectPaintEvent *event)
{
    tpChildWidget::onPaintEvent(event);

    // 应用正在安装中；根据安装进度绘制遮罩层
    tpCanvas *painter = event->canvas();

    HollowMask hollowMaskData;
    HollowMask::pieHollow pieHollowData;
    pieHollowData.x = width() / 2.0;
    pieHollowData.y = height() / 2.0;
    pieHollowData.start = 0;
    pieHollowData.end = 360.0 * installProgress_ / 100;
    pieHollowData.radius = (width() / 2.0) - 3;
    hollowMaskData.addPieHollow(pieHollowData);
    painter->hollowRoundedBox(0, 0, width(), width(), roundCorners(), _RGBA(0, 0, 0, 125), hollowMaskData);

    return true;
}