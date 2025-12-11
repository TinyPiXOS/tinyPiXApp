#include "SplashScreen.h"
#include "TpAnimation.h"
#include "DeskTopGlobal.hpp"
#include "StatusBar.h"
#include "NavigationBar.h"

SplashScreen::SplashScreen()
    : TpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    setBackGroundColor(_RGB(245, 245, 245));

    waitTimer_ = new TpTimer();
    waitTimer_->setInterval(1000);

    // 如果应用超过10秒还未启动完成，则自动关闭开屏界面
    connect(waitTimer_, timeout, [this]()
            {
        int64_t currentTimeS = TpDateTime::currentSecsSinceEpoch();
        if (currentTimeS - startTimeS_ >= 10)
        {
            this->close();
            waitTimer_->stop();
        } });
}

SplashScreen::~SplashScreen()
{
    waitTimer_->stop();
    delete waitTimer_;
    waitTimer_ = nullptr;
}

void SplashScreen::showSplashScreen(const TpRect &appRect)
{
    waitTimer_->start();

    TpAnimation *scaleAnim = new TpAnimation(this, TpAnimation::Geometry);
    scaleAnim->setStartValue(appRect);
    scaleAnim->setEndValue(TpRect(TpPoint(0, globalStatusBar_->height()), globalMainScreen_->screenSize()));
    scaleAnim->setDuration(100);

    // 导航条置顶
    show();
    globalNavigationBar_->bringToTop();
    scaleAnim->start();
}

void SplashScreen::setScreenImage(const TpImage &image)
{
}

void SplashScreen::setScreenColor(const int32_t &color)
{
}

void SplashScreen::setVisible(bool visible)
{
    if (!visible)
    {
        waitTimer_->stop();
    }

    TpDialog::setVisible(visible);
}
