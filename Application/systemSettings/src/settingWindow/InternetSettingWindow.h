#ifndef __INTERNET_SETTING_WINDOW_H
#define __INTERNET_SETTING_WINDOW_H

#include "settingWindow/settingBase.h"
#include "tpLineEdit.h"
#include "systemSettingsGlobal.h"
#include "tpListCheckBoxGroup.h"
#include "tpMenuPanelWidget.h"
#include "tpOnOffButton.h"

class InternetSettingWindow
    : public settingBase
{
public:
    InternetSettingWindow(tpChildWidget *parent = nullptr);
    virtual ~InternetSettingWindow();

    // 界面刷新数据
    virtual void refreshData() override;

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

private:
    // 点击保存IPV4配置数据
    void slotSaveIpv4Config(bool);

    // 保存DNS设置
    void slotSaveDnsConfig(bool);

    // 切换网卡选择
    void slotSwitchNetDevice(tpListCheckBoxGroupItem* item);

private:
    void initUi();

    // 构建ipv4配置菜单面板
    void configIpv4Panel();

    /// @brief 构建DNS配置菜单面板
    void configDnsPanel();

    // 刷新指定网卡信息; 网卡名称
    void refreshNetDeviceInfo(const tpString& netName);

private:
    // 网卡列表单选组
    tpListCheckBoxGroup *netDeviceListCbx_;

    // IPV4设置
    tpMenuPanelWidget *ipv4ConfigPanel_;
    tpOnOffButton* isDhcpBtn_;
    tpLineEdit *ipAddrEdit_;
    tpLineEdit *maskEdit_;
    tpLineEdit *gatewayEdit_;

    // 应用ipv4设置按钮
    tpButton *applyIpv4Btn_;

    // DNS设置
    tpMenuPanelWidget *dnsConfigPanel_;
    tpOnOffButton* autoDnseBtn_;
    tpLineEdit *firstDnsEdit_;
    tpLineEdit *secondDnsEdit_;
    tpButton *applyDnsBtn_;
};

#endif