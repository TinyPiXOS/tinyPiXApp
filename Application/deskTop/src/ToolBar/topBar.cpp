#include "topBar.h"
#include "TpFont.h"
#include "TpString.h"
#include "deskTopGlobal.hpp"
#include "TpDisplay.h"
#include "TpTime.h"
#include "TpDate.h"
#include "TpEvent.h"
#include "TpImage.h"
#include "TpBluetoothLocal.h"
#include "TpNetworkInterface.h"

#include <cmath>

bool globalSystemLockStatus = false;

#ifndef TOP_BAR_COLOR
#define TOP_BAR_COLOR _RGBA(255, 255, 255, 0)
#endif

topBar::topBar()
// : TpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    setBackGroundColor(TOP_BAR_COLOR);

    initUI();
}

topBar::~topBar()
{
}

void topBar::setColor(const int32_t &appColor)
{
}

bool topBar::onResizeEvent(TpResizeEvent *event)
{
    TpDialog::onResizeEvent(event);

    caculateTopAppPos();

    // 刷新应用工具栏尺寸
    // if (appSettingBar_)
    //     appSettingBar_->resizeSettingBar(this);

    return true;
}

bool topBar::onMousePressEvent(TpMouseEvent *event)
{
    TpDialog::onMousePressEvent(event);

    std::cout << "TopBar Press Pos ()" << event->globalPos().x() << " , " << event->globalPos().y() << std::endl;

    return true;
}

bool topBar::onLeaveEvent(TpLeaveEvent *event)
{
    if (event->eventType() == TpEvent::EVENT_OBJECT_LEAVE_TYPE)
    {
        if (event->leave())
        {
            update();
        }
    }

    return true;
}

void topBar::initUI()
{
    sysTimeLabel_ = new TpLabel(this);
    sysTimeLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    sysTimeLabel_->font()->setFontSize(TpDisplay::sp2Px(11));
    sysTimeLabel_->setText("09:43");
    sysTimeLabel_->setWidth(sysTimeLabel_->font()->pixelWidth());
    sysTimeLabel_->setHeight(TpDisplay::dp2Px(17));

    sysDateLabel_ = new TpLabel(this);
    sysDateLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    sysDateLabel_->font()->setFontSize(TpDisplay::sp2Px(11));
    sysDateLabel_->setText("2月22日 周二");
    sysDateLabel_->setWidth(sysDateLabel_->font()->pixelWidth());
    sysDateLabel_->setHeight(TpDisplay::dp2Px(17));

    wifiLabel_ = new TpLabel(this);
    wifiLabel_->setSize(TpDisplay::dp2Px(17), TpDisplay::dp2Px(17));
    wifiLabel_->setBackGroundImage(TpImage(applicationDirPath() + "/../res/topBar/WIFI.png"));
    wifiLabel_->setVisible(false);

    blueToothLabel_ = new TpLabel(this);
    blueToothLabel_->setSize(TpDisplay::dp2Px(17), TpDisplay::dp2Px(17));
    blueToothLabel_->setBackGroundImage(TpImage(applicationDirPath() + "/../res/topBar/蓝牙.png"));
    blueToothLabel_->setVisible(false);

    elecBattery_ = new TpBattery(this);
    elecBattery_->setWidth(TpDisplay::dp2Px(25));
    elecBattery_->setHeight(TpDisplay::dp2Px(16));
    elecBattery_->setValue(100);

    updateTimetimer_ = new TpTimer(50000);
    connect(updateTimetimer_, timeout, this, &topBar::slotUpdateSystemTime);
    updateTimetimer_->start();

    // 系统启动就要刷新一下时间
    slotUpdateSystemTime();
}

void topBar::caculateTopAppPos()
{
    TpRect topBarRect = rect();

    // std::cout << " topBarRect.h : " << topBarRect.h << " statusBtnInterval : " << statusBtnInterval << " statusBtnWh : " << std::endl;
    // std::cout << " statusBtnWh :" << statusBtnWh << std::endl;

    // 添加系统时间和系统日期
    sysTimeLabel_->move(TpDisplay::dp2Px(16), (topBarRect.height() - sysTimeLabel_->height()) / 2.0);

    uint32_t sysDateXPos = sysTimeLabel_->rect().x() + sysTimeLabel_->width() + TpDisplay::dp2Px(12);
    sysDateLabel_->move(sysDateXPos, (topBarRect.height() - sysDateLabel_->height()) / 2.0);

    elecBattery_->move(width() - elecBattery_->width() - TpDisplay::dp2Px(16), (topBarRect.height() - elecBattery_->height()) / 2.0);

    wifiLabel_->move(elecBattery_->pos().x() - wifiLabel_->width() - TpDisplay::dp2Px(5), (topBarRect.height() - wifiLabel_->height()) / 2.0);

    blueToothLabel_->move(wifiLabel_->pos().x() - blueToothLabel_->width() - TpDisplay::dp2Px(5), (topBarRect.height() - blueToothLabel_->height()) / 2.0);
}

void topBar::slotUpdateSystemTime()
{
    // 更新时间
    TpTime currentTime = TpTime::currentTime();
    TpString curTimeStr = currentTime.toString("HH:mm");
    sysTimeLabel_->setText(curTimeStr);

    // 更新日期
    TpDate currentDate = TpDate::currentDate();
    TpString curDateStr = TpString::number(currentDate.month()) + "月" + TpString::number(currentDate.day()) + "日 " + transWeekData(currentDate.dayOfWeek());
    sysDateLabel_->setText(curDateStr);

#if 0
    // 获取蓝牙设备状态;取第一个蓝牙设备的状态
    bool blueIsOpen = false;
    TpList<TpBluetoothLocal> blueToothDeviceList = TpBluetoothLocal::getAllDevice();
    if (blueToothDeviceList.size() > 0)
    {
        TpBluetoothLocal& firstBlueDevice = blueToothDeviceList.front();
        blueIsOpen = firstBlueDevice.isPowerOn();
        blueToothLabel_->setVisible(blueIsOpen);
    }

    // 设置网络状态;任意一网卡有网就显示连接状态
    bool isOnline = false;
    TpList<TpNetworkInterface> networkList = TpNetworkInterface::getAllDevice();
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

TpString topBar::transWeekData(const int32_t &dayOfWeek)
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
