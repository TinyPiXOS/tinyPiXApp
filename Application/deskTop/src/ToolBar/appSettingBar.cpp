#include "appSettingBar.h"
#include "topBar.h"
#include "deskTopGlobal.hpp"
#include <iostream>
#include "TpString.h"
#include "TpVariant.h"
#include "TpBluetoothLocal.h"
#include "TpMessageBox.h"
#include "TpSound.h"

#ifndef SETTING_BAR_COLOR
#define SETTING_BAR_COLOR _RGBA(93, 97, 208, 204)
#endif

appSettingBar::appSettingBar()
    : TpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    this->setEnabledBorderColor(false);
    this->setBackGroundColor(SETTING_BAR_COLOR);
    // this->setAlpha(51);
    // setVisible(false);

    dateTimeLabel_ = new TpLabel(this);
    dateTimeLabel_->setProperty("type", "controlPanelDateTimeLabel");
    dateTimeLabel_->setText("2025年12月12日 周六");
    dateTimeLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    dateTimeLabel_->font()->setFontSize(globalMainScreen_->screenWidth() * 0.016); // TpDisplay::sp2Px(16)
    dateTimeLabel_->setSize(dateTimeLabel_->font()->pixelWidth(), dateTimeLabel_->font()->pixelHeight());

    powerOffBtn_ = new TpIconTopButton(applicationDirPath() + "/../res/controlPanel/控制面板-电源.png", "", this);
    powerOffBtn_->setIconSize(globalMainScreen_->screenWidth() * 0.032, globalMainScreen_->screenWidth() * 0.032);
    powerOffBtn_->setTextVisible(false);
    powerOffBtn_->setEnableBackGroundColor(false);
    powerOffBtn_->setEnabledBorderColor(false);
    connect(powerOffBtn_, onClicked, this, &appSettingBar::slotPowerOff);

    // 声音进度条
    voiceProgessBar_ = new TpSlideProgressBar(this);
    voiceProgessBar_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-音量.png");
    voiceProgessBar_->setRange(0, 100);
    voiceProgessBar_->setValue(50);
    voiceProgessBar_->setFixedSize(globalMainScreen_->screenWidth() * 0.2824, globalMainScreen_->screenHeight() * 0.0888);
    connect(voiceProgessBar_, onValueChanged, this, &appSettingBar::slotChangeVoice);

    // // 亮度进度条
    lightProgessBar_ = new TpSlideProgressBar(this);
    lightProgessBar_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-亮度.png");
    lightProgessBar_->setRange(0, 100);
    lightProgessBar_->setValue(100);
    lightProgessBar_->setEnabled(false);
    lightProgessBar_->setFixedSize(globalMainScreen_->screenWidth() * 0.2824, globalMainScreen_->screenHeight() * 0.0888);
    connect(lightProgessBar_, onValueChanged, this, &appSettingBar::slotChangelight);

    wifiBtn_ = new TpPanelSwitchButton(this);
    wifiBtn_->setCheckable(true);
    wifiBtn_->setText("WIFI");
    wifiBtn_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-WIFI.png");
    wifiBtn_->setFixedSize(globalMainScreen_->screenWidth() * 0.14166, globalMainScreen_->screenWidth() * 0.14166);
    connect(wifiBtn_, onClicked, this, &appSettingBar::slotSwitchWifi);

    bluetoothBtn_ = new TpPanelSwitchButton(this);
    bluetoothBtn_->setCheckable(true);
    bluetoothBtn_->setText("蓝牙");
    bluetoothBtn_->setIcon(applicationDirPath() + "/../res/controlPanel/控制面板-蓝牙.png");
    bluetoothBtn_->setFixedSize(globalMainScreen_->screenWidth() * 0.14166, globalMainScreen_->screenWidth() * 0.14166);
    connect(bluetoothBtn_, onClicked, this, &appSettingBar::slotSwitchBluetooth);

    sysLockBtn_ = new TpPanelSwitchButton(this);
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
    TpDialog::setVisible(visible);

    // 刷新音量
    TpList<TpString> soundList = TpSound::getDevices();
    if (soundList.size() > 0)
    {
        voiceProgessBar_->setEnabled(true);

        TpSound sound(soundList.front());
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

void appSettingBar::updateTime(const int32_t &year, const int32_t &month, const int32_t &day, const TpString &weekDay)
{
    dateTimeLabel_->setText(TpString::number(year) + "年" + TpString::number(month) + "月" + TpString::number(day) + "日" + weekDay);
}

void appSettingBar::setBluetoothStatus(const bool &status)
{
    bluetoothBtn_->setChecked(status);
}

bool appSettingBar::onMousePressEvent(TpMouseEvent *event)
{
    mouseLeftPress_ = event->state();
    pressPoint_ = event->globalPos();

    return true;
}

bool appSettingBar::onMouseRleaseEvent(TpMouseEvent *event)
{
    mouseLeftPress_ = event->state();

    return true;
}

bool appSettingBar::onMouseMoveEvent(TpMouseEvent *event)
{
    // TpDialog::onMouseMoveEvent(event);

    if (mouseLeftPress_)
    {
        TpPoint curMousePos = event->globalPos();

        if ((curMousePos.y() - pressPoint_.y()) <= -5)
        {
            // 触发一次上拉事件后，不再重复触发
            mouseLeftPress_ = false;

            this->setVisible(false);

            // std::cout << "appSettingBar visible false " << std::endl;
        }
    }

    return true;
}

bool appSettingBar::onResizeEvent(TpResizeEvent *event)
{
    resizeOperatorBtn();

    return true;
}

void appSettingBar::slotSwitchBluetooth(bool checked)
{
#if 0
    bool blueIsOpen = false;
    TpList<TpBluetoothLocal> blueToothDeviceList = TpBluetoothLocal::getAllDevice();
    if (blueToothDeviceList.size() == 0)
    {
        bluetoothBtn_->setChecked(false);

        TpMessageBox msg("tinyPiX_SYS_Float_0531acbf04");
        msg.setMessageType(TpMessageBox::Information);
        msg.setText("未检测到蓝牙设备!");

        TpVector<TpString> btnList;
        btnList.emplace_back("确认");

        msg.setButtonList(btnList);
        msg.exec();

        return;
    }

    for (auto &blueDevice : blueToothDeviceList)
    {
        blueDevice.powerOn();
    }
#endif
}

void appSettingBar::slotSwitchWifi(bool checked)
{
}

void appSettingBar::slotChangeVoice(int32_t value)
{
    // TODO ; ARM暂时屏蔽音量修改
    return;

    // 刷新音量
    TpList<TpString> soundList = TpSound::getDevices();
    if (soundList.size() > 0)
    {
        TpSound sound(soundList.front());
        sound.setSystemVolume(value);
    }
}

void appSettingBar::slotChangelight(int32_t value)
{
}

void appSettingBar::resizeOperatorBtn()
{
    TpRect settingBarRect = this->rect();
    uint32_t panelHMargin = (settingBarRect.width() - BOTTOM_BAR_WIDTH) / 2.0;

    uint32_t firstRowY = MAIN_PANEL_TOP_DISTANCE + TOP_BAR_HEIGHT;
    dateTimeLabel_->move(panelHMargin, firstRowY);

    powerOffBtn_->move(settingBarRect.width() - panelHMargin - powerOffBtn_->width(), firstRowY);

    uint32_t secondRowY = firstRowY + dateTimeLabel_->height() + globalMainScreen_->screenHeight() * 0.0305;
    lightProgessBar_->move(panelHMargin, secondRowY);
    voiceProgessBar_->move(panelHMargin, secondRowY + lightProgessBar_->height() + globalMainScreen_->screenHeight() * 0.03472);

    // std::cout << "lightProgessBar_->width " << lightProgessBar_->width() << " " << lightProgessBar_->height();
    // 按钮宽度161

    sysLockBtn_->move(rect().width() - panelHMargin - sysLockBtn_->width(), secondRowY);

    bluetoothBtn_->move(sysLockBtn_->pos().x() - bluetoothBtn_->width() - globalMainScreen_->screenHeight() * 0.03472, secondRowY);

    wifiBtn_->move(bluetoothBtn_->pos().x() - wifiBtn_->width() - globalMainScreen_->screenHeight() * 0.03472, secondRowY);
}

void appSettingBar::slotPowerOff(bool checked)
{
    powerManageWindow_->setVisible(true);
    powerManageWindow_->update();
}
