#include "desktopAppButton.h"
#include "TpCanvas.h"

desktopAppButton::desktopAppButton(TpChildWidget *parent)
    : TpIconTopButton(parent)
{
    hollowWidget_ = new hollowWidget(this);
    hollowWidget_->setVisible(false);
}

desktopAppButton::desktopAppButton(const TpString &iconPath, const TpString &text, TpChildWidget *parent)
    : TpIconTopButton(iconPath, text, parent)
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
    TpIconTopButton::setRoundCorners(round);
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

bool desktopAppButton::onResizeEvent(TpObjectResizeEvent *event)
{
    TpIconTopButton::onResizeEvent(event);

    hollowWidget_->setSize(width(), width());

    return true;
}

hollowWidget::hollowWidget(TpChildWidget *parent)
    : TpChildWidget(parent), installProgress_(100)
{
}

hollowWidget::~hollowWidget()
{
}

void hollowWidget::setInstallProgress(const uint32_t &progress)
{
    installProgress_ = progress;
}

bool hollowWidget::onPaintEvent(TpObjectPaintEvent *event)
{
    TpChildWidget::onPaintEvent(event);

    // 应用正在安装中；根据安装进度绘制遮罩层
    TpCanvas *painter = event->canvas();

    HollowMask hollowMaskData;
    HollowMask::PieHollow pieHollowData;
    pieHollowData.x = width() / 2.0;
    pieHollowData.y = height() / 2.0;
    pieHollowData.start = 0;
    pieHollowData.end = 360.0 * installProgress_ / 100;
    pieHollowData.radius = (width() / 2.0) - 3;
    hollowMaskData.addPieHollow(pieHollowData);
    painter->roundedBox(0, 0, width(), width(), roundCorners(), _RGBA(0, 0, 0, 125), hollowMaskData);

    return true;
}