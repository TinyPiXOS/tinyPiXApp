#ifndef __BLUETOOTH_SETTING_WINDOW_H
#define __BLUETOOTH_SETTING_WINDOW_H

#include "settingWindow/settingBase.h"
#include "TpLineEdit.h"
#include "systemSettingsGlobal.h"
#include "TpMenuPanelWidget.h"

class bluetoothSettingWindow
    : public settingBase
{
public:
    bluetoothSettingWindow(TpChildWidget *parent = nullptr);
    virtual ~bluetoothSettingWindow();

protected:
    virtual bool onResizeEvent(TpObjectResizeEvent *event) override;

private:
    void initUi();

    // 构建本地设备信息面板界面
    void configLocalDevicePanel();

private:
    // 本机设备信息
    TpMenuPanelWidget *lovalDevicePanel_;

    // 本机名称输入框
    TpLineEdit* localDeviceNameEdit_;

    // 已配对设备面板
    TpMenuPanelWidget *connectDevicePanel_;
    // 已配对设备信息item
    TpMenuPanelItem *connectDeviceItem_;

    // 可用设备面板
    TpMenuPanelWidget *usableDevicePanel_;

};

#endif