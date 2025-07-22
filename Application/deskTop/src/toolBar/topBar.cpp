#include "topBar.h"
#include "tpFont.h"
#include "tpString.h"
#include "deskTopGlobal.hpp"
#include "tpDisplay.h"
#include "tpTime.h"
#include "tpDate.h"
#include "tpEvent.h"
#include "tpSurface.h"
#include "tpBluetoothLocal.h"
#include "tpNetworkInterface.h"

#include <cmath>

bool globalSystemLockStatus = false;

#ifndef TOP_BAR_COLOR
#define TOP_BAR_COLOR _RGBA(255, 255, 255, 0)
#endif

topBar::topBar(tpScreen *topScreen)
    : tpDialog("tinyPiX_SYS_Float_0531acbf04"), topScreen_(topScreen)
{
    setEnabledBorderColor(false);
    setBackGroundColor(TOP_BAR_COLOR);

    // setAlpha(50);

    this->construct();
}

topBar::~topBar()
{
    this->destruction();
}

void topBar::construct()
{
    sysTimeLabel_ = new tpLabel(this);
    sysTimeLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    sysTimeLabel_->font()->setFontSize(tpDisplay::sp2Px(11));
    sysTimeLabel_->setText("09:43");
    sysTimeLabel_->setWidth(sysTimeLabel_->font()->pixelWidth());
    sysTimeLabel_->setHeight(tpDisplay::dp2Px(17));

    sysDateLabel_ = new tpLabel(this);
    sysDateLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    sysDateLabel_->font()->setFontSize(tpDisplay::sp2Px(11));
    sysDateLabel_->setText("2月22日 周二");
    sysDateLabel_->setWidth(sysDateLabel_->font()->pixelWidth());
    sysDateLabel_->setHeight(tpDisplay::dp2Px(17));

    wifiLabel_ = new tpLabel(this);
    wifiLabel_->setSize(tpDisplay::dp2Px(17), tpDisplay::dp2Px(17));
    wifiLabel_->setBackGroundImage(tpMakeShared<tpSurface>(applicationDirPath() + "/../res/topBar/WIFI.png"));
    wifiLabel_->setVisible(false);

    blueToothLabel_ = new tpLabel(this);
    blueToothLabel_->setSize(tpDisplay::dp2Px(17), tpDisplay::dp2Px(17));
    blueToothLabel_->setBackGroundImage(tpMakeShared<tpSurface>(applicationDirPath() + "/../res/topBar/蓝牙.png"));
    blueToothLabel_->setVisible(false);

    elecBattery_ = new tpBattery(this);
    elecBattery_->setWidth(tpDisplay::dp2Px(25));
    elecBattery_->setHeight(tpDisplay::dp2Px(16));
    elecBattery_->setValue(100);

    updateTimetimer_ = new tpTimer(50000);
    connect(updateTimetimer_, timeout, this, &topBar::slotUpdateSystemTime);
    updateTimetimer_->start();

    // 系统启动就要刷新一下时间
    slotUpdateSystemTime();
}

void topBar::destruction()
{
}

void topBar::setVisible(bool visible)
{
    // 隐藏topbar内部控件
    sysDateLabel_->setVisible(visible);
    sysTimeLabel_->setVisible(visible);

    // 蓝牙和网络连接状态
    wifiLabel_->setVisible(visible);
    blueToothLabel_->setVisible(visible);

    // 电量显示窗
    elecBattery_->setVisible(visible);

    // tpDialog::setVisible(visible);
    update();
}

void topBar::setColor(const int32_t &appColor)
{
}

bool topBar::onResizeEvent(tpObjectResizeEvent *event)
{
    caculateTopAppPos();

    // 刷新应用工具栏尺寸
    // if (appSettingBar_)
    //     appSettingBar_->resizeSettingBar(this);

    return true;
}

bool topBar::onMousePressEvent(tpMouseEvent *event)
{
    std::cout << "TopBar Press Pos ()" << event->globalPos().x << " , " << event->globalPos().y << std::endl;

    return true;
}

bool topBar::onMouseRleaseEvent(tpMouseEvent *event)
{
    return true;
}

bool topBar::onMouseMoveEvent(tpMouseEvent *event)
{
    return true;
}

bool topBar::onLeaveEvent(tpObjectLeaveEvent *event)
{
    if (event->eventType() == tpEvent::EVENT_OBJECT_LEAVE_TYPE)
    {
        if (event->leave())
        {
            update();
        }
    }

    return true;
}

void topBar::caculateTopAppPos()
{
    ItpRect topBarRect = rect();

    // std::cout << " topBarRect.h : " << topBarRect.h << " statusBtnInterval : " << statusBtnInterval << " statusBtnWh : " << std::endl;
    // std::cout << " statusBtnWh :" << statusBtnWh << std::endl;

    // 添加系统时间和系统日期
    sysTimeLabel_->move(tpDisplay::dp2Px(16), (topBarRect.h - sysTimeLabel_->height()) / 2.0);

    uint32_t sysDateXPos = sysTimeLabel_->rect().x + sysTimeLabel_->width() + tpDisplay::dp2Px(12);
    sysDateLabel_->move(sysDateXPos, (topBarRect.h - sysDateLabel_->height()) / 2.0);

    elecBattery_->move(width() - elecBattery_->width() - tpDisplay::dp2Px(16), (topBarRect.h - elecBattery_->height()) / 2.0);

    wifiLabel_->move(elecBattery_->pos().x - wifiLabel_->width() - tpDisplay::dp2Px(5), (topBarRect.h - wifiLabel_->height()) / 2.0);

    blueToothLabel_->move(wifiLabel_->pos().x - blueToothLabel_->width() - tpDisplay::dp2Px(5), (topBarRect.h - blueToothLabel_->height()) / 2.0);
}

void topBar::slotUpdateSystemTime()
{
    // 更新时间
    tpTime currentTime = tpTime::currentTime();
    tpString curTimeStr = currentTime.toString("HH:mm");
    sysTimeLabel_->setText(curTimeStr);

    // 更新日期
    tpDate currentDate = tpDate::currentDate();
    tpString curDateStr = tpString::number(currentDate.month()) + "月" + tpString::number(currentDate.day()) + "日 " + transWeekData(currentDate.dayOfWeek());
    sysDateLabel_->setText(curDateStr);

#if 0
    // 获取蓝牙设备状态;取第一个蓝牙设备的状态
    bool blueIsOpen = false;
    tpList<tpBluetoothLocal> blueToothDeviceList = tpBluetoothLocal::getAllDevice();
    if (blueToothDeviceList.size() > 0)
    {
        tpBluetoothLocal& firstBlueDevice = blueToothDeviceList.front();
        blueIsOpen = firstBlueDevice.isPowerOn();
        blueToothLabel_->setVisible(blueIsOpen);
    }

    // 设置网络状态;任意一网卡有网就显示连接状态
    bool isOnline = false;
    tpList<tpNetworkInterface> networkList = tpNetworkInterface::getAllDevice();
    for (auto &internetDevice : networkList)
    {
        // 网卡关闭不处理
        // if (internetDevice.de)

        isOnline = internetDevice.isOnlineInternet();
        if (isOnline)
            break;
    }
    wifiLabel_->setVisible(isOnline);
#endif

    if (globalTopSettingBar_)
    {
        // globalTopSettingBar_->setBluetoothStatus(blueIsOpen);
        globalTopSettingBar_->updateTime(currentDate.year(), currentDate.month(), currentDate.day(), transWeekData(currentDate.dayOfWeek()));
    }
}

tpString topBar::transWeekData(const int32_t &dayOfWeek)
{
    switch (dayOfWeek)
    {
    case 1:
        return "周一";
    case 2:
        return "周二";
    case 3:
        return "周三";
    case 4:
        return "周四";
    case 5:
        return "周五";
    case 6:
        return "周六";
    case 7:
        return "周日";
    default:
        return "周一";
    }
}
