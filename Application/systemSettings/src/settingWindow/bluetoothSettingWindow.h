#ifndef __BLUETOOTH_SETTING_WINDOW_H
#define __BLUETOOTH_SETTING_WINDOW_H

#include "settingWindow/settingBase.h"
#include "tpLineEdit.h"
#include "systemSettingsGlobal.h"
#include "tpMenuPanelWidget.h"

class bluetoothSettingWindow
    : public settingBase
{
public:
    bluetoothSettingWindow(tpChildWidget *parent = nullptr);
    virtual ~bluetoothSettingWindow();

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

private:
    void initUi();

    // 构建本地设备信息面板界面
    void configLocalDevicePanel();

private:
    // 本机设备信息
    tpMenuPanelWidget *lovalDevicePanel_;

    // 本机名称输入框
    tpLineEdit* localDeviceNameEdit_;

    // 已配对设备面板
    tpMenuPanelWidget *connectDevicePanel_;
    // 已配对设备信息item
    tpMenuPanelItem *connectDeviceItem_;

    // 可用设备面板
    tpMenuPanelWidget *usableDevicePanel_;

};

#endif