#include "powerManage.h"
#include "tpVariant.h"
#include "deskTopGlobal.hpp"

#define POWER_MANAGE_BG_COLOR _RGBA(0, 0, 0, 153)

powerManage::powerManage()
    : tpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    setBackGroundColor(POWER_MANAGE_BG_COLOR);
    // setAlpha(102);

    powerRestartBtn_ = new tpButton("重新启动", this);
    // powerRestartBtn_->setButtonStyle(tpButton::TextOnly);
    // powerRestartBtn_->setSize(tpDisplay::dp2Px(305), tpDisplay::dp2Px(64));
    powerRestartBtn_->setProperty("type", "ControlPanelPowerButton");

    powerOffBtn_ = new tpButton("关机", this);
    // powerOffBtn_->setButtonStyle(tpButton::TextOnly);
    // powerOffBtn_->setSize(tpDisplay::dp2Px(305), tpDisplay::dp2Px(64));
    powerOffBtn_->setProperty("type", "ControlPanelPowerButton");

    closeBtn_ = new tpButton(this);
    closeBtn_->setButtonStyle(tpButton::IconOnly);
    closeBtn_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-关闭.png");
    closeBtn_->setEnableBackGroundColor(false);
    // closeBtn_->setSize();
    closeBtn_->setProperty("type", "ControlPanelPowerCloseButton");

    connect(closeBtn_, onClicked, [=](bool)
            { close(); });

    setRect(0, 0, globalMainScreen_->width(), globalMainScreen_->height());

    uint32_t btnX = (rect().w - powerOffBtn_->width()) / 2.0;

    powerRestartBtn_->move(btnX, tpDisplay::dp2Px(240));
    powerOffBtn_->move(btnX, powerRestartBtn_->pos().y + powerRestartBtn_->height() + tpDisplay::dp2Px(37));

    uint32_t closeBtnX = (rect().w - closeBtn_->width()) / 2.0;

    closeBtn_->move(closeBtnX, powerOffBtn_->pos().y + powerOffBtn_->height() + tpDisplay::dp2Px(215));

    setVisible(false);
}

powerManage::~powerManage()
{
}

bool powerManage::onMousePressEvent(tpMouseEvent *event)
{
    setVisible(false);

    return true;
}

bool powerManage::onMouseRleaseEvent(tpMouseEvent *event)
{
    return true;
}

bool powerManage::onResizeEvent(tpObjectResizeEvent *event)
{
    return true;
}
