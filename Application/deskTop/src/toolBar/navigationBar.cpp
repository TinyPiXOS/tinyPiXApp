#include "navigationBar.h"
#include "tpCanvas.h"
#include "tpDisplay.h"
#include "tpAnimation.h"
#include "deskTopGlobal.hpp"
#include "tpApp.h"

navigationBar::navigationBar()
    : tpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    const uint32_t navigationLineWidth = globalMainScreen_->screenWidth() * 0.3;
    const uint32_t navigationLineHeight = tpDisplay::dp2Px(6);

    lineLabel_ = new tpLabel(this);
    lineLabel_->setFixedSize(navigationLineWidth, navigationLineHeight);
    lineLabel_->setEnabledBorderColor(false);
    lineLabel_->setEnableBackGroundColor(true);
    lineLabel_->setBackGroundColor(_RGB(88, 88, 88));
    lineLabel_->setRoundCorners(navigationLineHeight / 2.0);
    lineLabel_->installEventFilter(this);

    // 导航线上下各10像素
    setSize(navigationLineWidth, tpDisplay::dp2Px(20));
    setBackGroundColor(_RGBA(255, 255, 255, 0));
    // setAlpha(0);
}

navigationBar::~navigationBar()
{
}

void navigationBar::setColor(const int32_t &appColor)
{
}

bool navigationBar::onResizeEvent(tpObjectResizeEvent *event)
{
    int32_t lienY = (height() - lineLabel_->height()) / 2.0;
    lineLabel_->move(0, lienY);

    return true;
}

bool navigationBar::onMousePressEvent(tpMouseEvent *event)
{
    mousePressPoint_ = event->globalPos();
    mousePressTime_ = tpTime::currentTime();

    return true;
}

bool navigationBar::onMouseRleaseEvent(tpMouseEvent *event)
{
    ItpPoint curPos = event->globalPos();
    if (std::abs(curPos.x - mousePressPoint_.x) < 5 && std::abs(curPos.y - mousePressPoint_.y) < 5)
    {
        tpAnimation *moveAnimation = new tpAnimation(lineLabel_, tpAnimation::Pos);
        moveAnimation->setStartValue(lineLabel_->pos());
        moveAnimation->setKeyValueAt(0.5, ItpPoint(0, 0));
        moveAnimation->setEndValue(lineLabel_->pos());
        moveAnimation->setDuration(500);
        moveAnimation->start();
    }
    else
    {
        tpTime mouseReleaseTime = tpTime::currentTime();
        int32_t msTime = mousePressTime_.msecsTo(mouseReleaseTime);

        int32_t offsetY = mousePressPoint_.y - curPos.y;

        // std::cout << "Press Hour minut " << mousePressTime_.hour() << "  " << mousePressTime_.minute() << "  " << mousePressTime_.second() << "  " << mousePressTime_.msec() << std::endl;
        // std::cout << "mouseReleaseTime Hour " << mouseReleaseTime.hour() << "  " << mouseReleaseTime.minute() << "  " << mouseReleaseTime.second() << "  " << mouseReleaseTime.msec() << std::endl;
        // std::cout << "msTime " << msTime << std::endl;

        // std::cout << "Press Y : " << mousePressPoint_.y << std::endl;
        // std::cout << "Motion Y : " << curPos.y << std::endl;
        // std::cout << "offsetY : " << offsetY << std::endl;

        if (offsetY > 4)
        {
            if (msTime > 800)
            {
                // 如果滑动间隔在 800 ms内，则为返回桌面，否则为打开任务管理器
                if (!globalAppTaskWindow->visible())
                {
                    globalAppTaskWindow->showMaximum();
                }

                std::cout << " Show Task Manage " << std::endl;
            }
            else
            {
                tinyPiX_sys_send_home(globalAgent);

                // PiShmBytes *appIdList = nullptr;
                // int appSize = 0;
                // tinyPiX_sys_find_win_ids(globalAgent, &appIdList, &appSize, 1);

                // std::cout << "appSizeappSizeappSize" << appSize << std::endl;
                // for (int i = 0; i < appSize; ++i)
                // {
                //     std::cout << "IIIIIIIDDDDDDD" << appIdList[i].id << std::endl;
                // }
                // tinyPiX_sys_send_return(globalAgent, 0);

                std::cout << " Return Desktop " << std::endl;
            }
        }
    }

    return true;
}

bool navigationBar::onMouseMoveEvent(tpMouseEvent *event)
{
    static int aaa = 0;
    // std::cout << "navigationBar::onMouseMoveEvent" << aaa++ << std::endl;

    // if (event->state())
    // {

    // }

    return true;
}

bool navigationBar::onLeaveEvent(tpObjectLeaveEvent *event)
{
    return true;
}

bool navigationBar::onPaintEvent(tpObjectPaintEvent *event)
{
    tpDialog::onPaintEvent(event);
    return true;
}

bool navigationBar::eventFilter(tpObject *watched, tpEvent *event)
{
    if (event->eventType() == tpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMousePressEvent(mouseEvent);
    }
    else if (event->eventType() == tpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMouseRleaseEvent(mouseEvent);
    }
    else if (event->eventType() == tpEvent::EVENT_MOUSE_MOVE_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMouseMoveEvent(mouseEvent);
    }
    else
    {
    }

    return false;
}
