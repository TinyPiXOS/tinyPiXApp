#ifndef __INTERNET_SETTING_WINDOW_H
#define __INTERNET_SETTING_WINDOW_H

#include "SettingWindow/SettingBase.h"
#include "TpLineEdit.h"
#include "SystemSettingsGlobal.h"
#include "TpListWidget.h"
#include "TpMenuPanelWidget.h"
#include "TpOnOffButton.h"

class InternetSettingWindow
    : public SettingBase
{
public:
    InternetSettingWindow(TpWidget *parent = nullptr);
    virtual ~InternetSettingWindow();

    // 界面刷新数据
    virtual void refreshData() override;

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;

private:
    // 点击保存IPV4配置数据
    void slotSaveIpv4Config(bool);

    // 保存DNS设置
    void slotSaveDnsConfig(bool);

    // 切换网卡选择
    void slotSwitchNetDevice(TpListWidgetItem* item);

private:
    void initUi();

    // 构建ipv4配置菜单面板
    void configIpv4Panel();

    /// @brief 构建DNS配置菜单面板
    void configDnsPanel();

    // 刷新指定网卡信息; 网卡名称
    void refreshNetDeviceInfo(const TpString& netName);

private:
    // 网卡列表单选组
    TpListWidget *netDeviceListCbx_;

    // IPV4设置
    TpMenuPanelWidget *ipv4ConfigPanel_;
    TpOnOffButton* isDhcpBtn_;
    TpLineEdit *ipAddrEdit_;
    TpLineEdit *maskEdit_;
    TpLineEdit *gatewayEdit_;

    // 应用ipv4设置按钮
    TpButton *applyIpv4Btn_;

    // DNS设置
    TpMenuPanelWidget *dnsConfigPanel_;
    TpOnOffButton* autoDnseBtn_;
    TpLineEdit *firstDnsEdit_;
    TpLineEdit *secondDnsEdit_;
    TpButton *applyDnsBtn_;
};

#endif