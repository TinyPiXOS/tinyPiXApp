#include "bottomBar.h"
#include "tpDisplay.h"

#ifndef BOTTOM_MARGIN
#define BOTTOM_MARGIN tpDisplay::dp2Px(16)
#endif

#ifndef BOTTOM_CONTENT_MARGIN
#define BOTTOM_CONTENT_MARGIN tpDisplay::dp2Px(24)
#endif

#ifndef BOTTOM_BAR_COLOR
// #define BOTTOM_BAR_COLOR _RGBA(255, 255, 255, 51)
#define BOTTOM_BAR_COLOR _RGBA(255, 255, 255, 120)
// #define BOTTOM_BAR_COLOR _RGBA(255, 255, 255, 128)
#endif

bottomBar::bottomBar(tpScreen *topScreen)
    : tpDialog(), topScreen_(topScreen)
{
    setEnabledBorderColor(false);
    setBackGroundColor(BOTTOM_BAR_COLOR);
}

bottomBar::~bottomBar()
{
}

void bottomBar::setVisible(bool visible)
{
    // std::cout << "bottomBar::setVisible " << visible << std::endl;
    tpDialog::setVisible(visible);
}

void bottomBar::setBottomAppBtn(const tpList<desktopAppButton *> &appList)
{
    systemAppBtnList_ = appList;

    for (const auto &bottomAppBtn : systemAppBtnList_)
    {
        bottomAppBtn->setParent(this);
        bottomAppBtn->setTextVisible(false);
    }

    caculateBottomAppPos();
    update();
}

tpList<desktopAppButton *> bottomBar::bottomAppBtnList()
{
    // TODO: 在此处插入 return 语句
    return systemAppBtnList_;
}

void bottomBar::removeApp(desktopAppButton *removeBtn)
{
    for (const auto &curApp : systemAppBtnList_)
    {
        if (curApp == removeBtn)
        {
            // 卸载APP
            systemAppBtnList_.remove(curApp);
            caculateBottomAppPos();
            update();
            break;
        }
    }
}

bool bottomBar::onResizeEvent(tpObjectResizeEvent *event)
{
    return true;
}

bool bottomBar::onMoveEvent(tpObjectMoveEvent *event)
{
    caculateBottomAppPos();

    return true;
}

bool bottomBar::onMousePressEvent(tpMouseEvent *event)
{
    return true;
}

bool bottomBar::onMouseRleaseEvent(tpMouseEvent *event)
{
    return true;
}

void bottomBar::caculateBottomAppPos()
{
    uint32_t bottomAppCount = systemAppBtnList_.size();

    uint32_t appAllWidth = bottomAppCount * APP_WIDTH_HEIGHT + (bottomAppCount - 1) * BOTTOM_CONTENT_MARGIN;

    uint32_t appStartX = (BOTTOM_BAR_WIDTH - appAllWidth) / 2.0;

    // 因为按钮是以顶层窗口为父类，所以坐标要叠加bittomBar的XY坐标
    for (int32_t i = 0; i < systemAppBtnList_.size(); ++i)
    {
        desktopAppButton *bottomAppBtn = systemAppBtnList_.at(i);

        bottomAppBtn->move(appStartX + (APP_WIDTH_HEIGHT + BOTTOM_CONTENT_MARGIN) * i, BOTTOM_MARGIN);
        // bottomAppBtn->move(pos().x + appStartX + (APP_WIDTH_HEIGHT + BOTTOM_CONTENT_MARGIN) * i, pos().y + BOTTOM_MARGIN);
    }
}
