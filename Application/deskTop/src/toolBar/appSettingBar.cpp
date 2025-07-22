#include "appSettingBar.h"
#include "topBar.h"
#include "deskTopGlobal.hpp"
#include <iostream>
#include "tpString.h"
#include "tpVariant.h"
#include "tpBluetoothLocal.h"
#include "tpMessageBox.h"
#include "tpSound.h"

#ifndef SETTING_BAR_COLOR
#define SETTING_BAR_COLOR _RGBA(93, 97, 208, 204)
#endif

appSettingBar::appSettingBar()
    : tpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    this->setEnabledBorderColor(false);
    this->setBackGroundColor(SETTING_BAR_COLOR);
    // this->setAlpha(51);
    // setVisible(false);

    dateTimeLabel_ = new tpLabel(this);
    dateTimeLabel_->setProperty("type", "controlPanelDateTimeLabel");
    dateTimeLabel_->setText("2025年12月12日 周六");
    dateTimeLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    dateTimeLabel_->font()->setFontSize(globalMainScreen_->screenWidth() * 0.016); // tpDisplay::sp2Px(16)
    dateTimeLabel_->setSize(dateTimeLabel_->font()->pixelWidth(), dateTimeLabel_->font()->pixelHeight());

    powerOffBtn_ = new tpIconTopButton(applicationDirPath() + "/../res/controlPanel/控制面板-电源.png", "", this);
    powerOffBtn_->setIconSize(globalMainScreen_->screenWidth() * 0.032, globalMainScreen_->screenWidth() * 0.032);
    powerOffBtn_->setTextVisible(false);
    powerOffBtn_->setEnableBackGroundColor(false);
    powerOffBtn_->setEnabledBorderColor(false);
    connect(powerOffBtn_, onClicked, this, &appSettingBar::slotPowerOff);

    // 声音进度条
    voiceProgessBar_ = new tpSlideProgressBar(this);
    voiceProgessBar_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-音量.png");
    voiceProgessBar_->setRange(0, 100);
    voiceProgessBar_->setValue(50);
    voiceProgessBar_->setFixedSize(globalMainScreen_->screenWidth() * 0.2824, globalMainScreen_->screenHeight() * 0.0888);
    connect(voiceProgessBar_, onValueChanged, this, &appSettingBar::slotChangeVoice);

    // // 亮度进度条
    lightProgessBar_ = new tpSlideProgressBar(this);
    lightProgessBar_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-亮度.png");
    lightProgessBar_->setRange(0, 100);
    lightProgessBar_->setValue(100);
    lightProgessBar_->setEnabled(false);
    lightProgessBar_->setFixedSize(globalMainScreen_->screenWidth() * 0.2824, globalMainScreen_->screenHeight() * 0.0888);
    connect(lightProgessBar_, onValueChanged, this, &appSettingBar::slotChangelight);

    wifiBtn_ = new tpPanelSwitchButton(this);
    wifiBtn_->setCheckable(true);
    wifiBtn_->setText("WIFI");
    wifiBtn_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-WIFI.png");
    wifiBtn_->setFixedSize(globalMainScreen_->screenWidth() * 0.14166, globalMainScreen_->screenWidth() * 0.14166);
    connect(wifiBtn_, onClicked, this, &appSettingBar::slotSwitchWifi);

    bluetoothBtn_ = new tpPanelSwitchButton(this);
    bluetoothBtn_->setCheckable(true);
    bluetoothBtn_->setText("蓝牙");
    bluetoothBtn_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-蓝牙.png");
    bluetoothBtn_->setFixedSize(globalMainScreen_->screenWidth() * 0.14166, globalMainScreen_->screenWidth() * 0.14166);
    connect(bluetoothBtn_, onClicked, this, &appSettingBar::slotSwitchBluetooth);

    sysLockBtn_ = new tpPanelSwitchButton(this);
    sysLockBtn_->setCheckable(true);
    sysLockBtn_->setText("锁定");
    sysLockBtn_->setFixedSize(globalMainScreen_->screenWidth() * 0.14166, globalMainScreen_->screenWidth() * 0.14166);
    sysLockBtn_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-锁定.png");

    powerManageWindow_ = new powerManage();

    setRect(0, 0, globalMainScreen_->width(), globalMainScreen_->height());

    resizeOperatorBtn();
}

appSettingBar::~appSettingBar()
{
}

void appSettingBar::setVisible(bool visible)
{
    tpDialog::setVisible(visible);

    // 刷新音量
    tpList<tpString> soundList = tpSound::getDevices();
    if (soundList.size() > 0)
    {
        voiceProgessBar_->setEnabled(true);

        tpSound sound(soundList.front());
        voiceProgessBar_->setValue(sound.getSystemVolume());
    }
    else
    {
        voiceProgessBar_->setValue(0);
        voiceProgessBar_->setEnabled(false);
    }

    // 所有子组件的显隐
    // dateTimeLabel_->setVisible(visible);
    // powerOffBtn_->setVisible(visible);
    // voiceProgessBar_->setVisible(visible);
    // lightProgessBar_->setVisible(visible);
    // wifiBtn_->setVisible(visible);
    // bluetoothBtn_->setVisible(visible);
    // sysLockBtn_->setVisible(visible);
}

void appSettingBar::updateTime(const int32_t &year, const int32_t &month, const int32_t &day, const tpString &weekDay)
{
    dateTimeLabel_->setText(tpString::number(year) + "年" + tpString::number(month) + "月" + tpString::number(day) + "日" + weekDay);
}

void appSettingBar::setBluetoothStatus(const bool &status)
{
    bluetoothBtn_->setChecked(status);
}

bool appSettingBar::onMousePressEvent(tpMouseEvent *event)
{
    mouseLeftPress_ = event->state();
    pressPoint_ = event->globalPos();

    return true;
}

bool appSettingBar::onMouseRleaseEvent(tpMouseEvent *event)
{
    mouseLeftPress_ = event->state();

    return true;
}

bool appSettingBar::onMouseMoveEvent(tpMouseEvent *event)
{
    // tpDialog::onMouseMoveEvent(event);

    if (mouseLeftPress_)
    {
        ItpPoint curMousePos = event->globalPos();

        if ((curMousePos.y - pressPoint_.y) <= -5)
        {
            // 触发一次上拉事件后，不再重复触发
            mouseLeftPress_ = false;

            this->setVisible(false);

            // std::cout << "appSettingBar visible false " << std::endl;
        }
    }

    return true;
}

bool appSettingBar::onResizeEvent(tpObjectResizeEvent *event)
{
    resizeOperatorBtn();

    return true;
}

void appSettingBar::slotSwitchBluetooth(bool checked)
{
    bool blueIsOpen = false;
    tpList<tpBluetoothLocal> blueToothDeviceList = tpBluetoothLocal::getAllDevice();
    if (blueToothDeviceList.size() == 0)
    {
        bluetoothBtn_->setChecked(false);

        tpMessageBox msg("tinyPiX_SYS_Float_0531acbf04");
        msg.setMessageType(tpMessageBox::Information);
        msg.setText("未检测到蓝牙设备!");

        tpVector<tpString> btnList;
        btnList.emplace_back("确认");

        msg.setButtonList(btnList);
        msg.exec();

        return;
    }

    for (auto &blueDevice : blueToothDeviceList)
    {
        blueDevice.powerOn();
    }
}

void appSettingBar::slotSwitchWifi(bool checked)
{
}

void appSettingBar::slotChangeVoice(int32_t value)
{
    // TODO ; ARM暂时屏蔽音量修改
    return;

    // 刷新音量
    tpList<tpString> soundList = tpSound::getDevices();
    if (soundList.size() > 0)
    {
        tpSound sound(soundList.front());
        sound.setSystemVolume(value);
    }
}

void appSettingBar::slotChangelight(int32_t value)
{
}

void appSettingBar::resizeOperatorBtn()
{
    ItpRect settingBarRect = this->rect();
    uint32_t panelHMargin = (settingBarRect.w - BOTTOM_BAR_WIDTH) / 2.0;

    uint32_t firstRowY = MAIN_PANEL_TOP_DISTANCE + TOP_BAR_HEIGHT;
    dateTimeLabel_->move(panelHMargin, firstRowY);

    powerOffBtn_->move(settingBarRect.w - panelHMargin - powerOffBtn_->width(), firstRowY);

    uint32_t secondRowY = firstRowY + dateTimeLabel_->height() + tpDisplay::dp2Px(22);
    lightProgessBar_->move(panelHMargin, secondRowY);
    voiceProgessBar_->move(panelHMargin, secondRowY + lightProgessBar_->height() + tpDisplay::dp2Px(25));

    // std::cout << "lightProgessBar_->width " << lightProgessBar_->width() << " " << lightProgessBar_->height();
    // 按钮宽度161

    sysLockBtn_->move(rect().w - panelHMargin - sysLockBtn_->width(), secondRowY);

    bluetoothBtn_->move(sysLockBtn_->pos().x - bluetoothBtn_->width() - tpDisplay::dp2Px(25), secondRowY);

    wifiBtn_->move(bluetoothBtn_->pos().x - wifiBtn_->width() - tpDisplay::dp2Px(25), secondRowY);
}

void appSettingBar::slotPowerOff(bool checked)
{
    powerManageWindow_->setVisible(true);
    powerManageWindow_->update();
}
