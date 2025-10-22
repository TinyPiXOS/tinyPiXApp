#ifndef __BLUETOOTH_SETTING_WINDOW_H
#define __BLUETOOTH_SETTING_WINDOW_H

#include "SettingWindow/SettingBase.h"
#include "TpLineEdit.h"
#include "SystemSettingsGlobal.h"
#include "TpMenuPanelWidget.h"

class BluetoothSettingWindow
    : public SettingBase
{
public:
    BluetoothSettingWindow(TpChildWidget *parent = nullptr);
    virtual ~BluetoothSettingWindow();

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;

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