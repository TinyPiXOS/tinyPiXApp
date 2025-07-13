#include "InternetSettingWindow.h"
#include "tpNetworkInterface.h"
#include "tpVBoxLayout.h"
#include "tpMessageBox.h"

InternetSettingWindow::InternetSettingWindow(tpChildWidget *parent)
    : settingBase(parent), netDeviceListCbx_(nullptr), ipv4ConfigPanel_(nullptr)
{
    initUi();
}

InternetSettingWindow::~InternetSettingWindow()
{
}

void InternetSettingWindow::refreshData()
{
    // 刷新所有的网卡列表
    netDeviceListCbx_->clear();
    tpList<tpNetworkInterface> internetDeviceList = tpNetworkInterface::getAllDevice();
    for (const auto &internetDevice : internetDeviceList)
    {
        // std::cout << "网卡名称:" << internetDevice.getName() << std::endl;
        tpListCheckBoxGroupItem *curItem = netDeviceListCbx_->addItem(internetDevice.getName());
    }

    tpListCheckBoxGroupItem *curSelectItem = netDeviceListCbx_->currentItem().front();
    if (curSelectItem)
    {
        refreshNetDeviceInfo(curSelectItem->text());
    }
}

bool InternetSettingWindow::onResizeEvent(tpObjectResizeEvent *event)
{
    return true;
}

void InternetSettingWindow::slotSaveIpv4Config(bool)
{
    tpVector<tpListCheckBoxGroupItem *> selectItemList = netDeviceListCbx_->currentItem();
    if (selectItemList.size() == 0)
    {
        tpMessageBox::information("未选择网卡!");
        return;
    }

    tpListCheckBoxGroupItem *curSelectItem = selectItemList.front();
    if (!curSelectItem)
        return;

    tpString curDeviceName = curSelectItem->text();
    tpNetworkInterface curNetInterface(curDeviceName);

    if (isDhcpBtn_->onOff())
    {
        curNetInterface.setDhcp();
    }
    else
    {
        if (ipAddrEdit_->text().empty())
        {
            tpMessageBox::information("IP地址为空!");
            return;
        }

        if (maskEdit_->text().empty())
        {
            tpMessageBox::information("子网掩码为空!");
            return;
        }

        if (gatewayEdit_->text().empty())
        {
            tpMessageBox::information("网关为空!");
            return;
        }

        // 静态IP必须要设置DNS
        if (autoDnseBtn_->onOff())
        {
            tpMessageBox::information("DHCP模式下必须手动设置DNS!");
            return;
        }

        if (firstDnsEdit_->text().empty())
        {
            tpMessageBox::information("DNS为空!");
            return;
        }

        tpList<tpString> dnsList;
        tpString dns1 = firstDnsEdit_->text();
        if (!dns1.empty())
            dnsList.emplace_back(dns1);

        tpString dns2 = secondDnsEdit_->text();
        if (!dns2.empty())
            dnsList.emplace_back(dns2);

        curNetInterface.setStatic(ipAddrEdit_->text(), gatewayEdit_->text(), maskEdit_->text(), dnsList);
    }

    tpMessageBox::information("设置完成!");
}

void InternetSettingWindow::slotSaveDnsConfig(bool)
{
    tpVector<tpListCheckBoxGroupItem *> selectItemList = netDeviceListCbx_->currentItem();
    if (selectItemList.size() == 0)
    {
        tpMessageBox::information("未选择网卡!");
        return;
    }

    tpListCheckBoxGroupItem *curSelectItem = selectItemList.front();
    if (!curSelectItem)
        return;

    tpString curDeviceName = curSelectItem->text();
    tpNetworkInterface curNetInterface(curDeviceName);

    // 非DHCP；DNS必须手动设置
    if (isDhcpBtn_->onOff())
    {
        if (firstDnsEdit_->text().empty())
        {
            tpMessageBox::information("DNS为空!");
            return;
        }
    }
    else
    {
        if (autoDnseBtn_->onOff())
        {
            tpMessageBox::information("静态IP模式下必须手动设置DNS!");
            return;
        }
    }

    // 设置动态DNS或静态DNS
    tpList<tpString> dnsList;
    tpString dns1 = firstDnsEdit_->text();
    if (!dns1.empty())
        dnsList.emplace_back(dns1);

    tpString dns2 = secondDnsEdit_->text();
    if (!dns2.empty())
        dnsList.emplace_back(dns2);

    curNetInterface.setDns(autoDnseBtn_->onOff() ? TP_TRUE : TP_FALSE, dnsList);
}

void InternetSettingWindow::slotSwitchNetDevice(tpListCheckBoxGroupItem *item)
{
    if (!item)
        return;

    if (!item->checked())
        return;

    tpString curNetDeviceName = item->text();

    // 刷新UI显示
    refreshNetDeviceInfo(curNetDeviceName);
}

void InternetSettingWindow::initUi()
{
    tpLabel *selectNetDeviceTitle = createGroupNameLabel("选择网卡");
    selectNetDeviceTitle->installEventFilter(this);

    // 网卡列表
    netDeviceListCbx_ = new tpListCheckBoxGroup();
    netDeviceListCbx_->installEventFilter(this);
    connect(netDeviceListCbx_, onStatusChanged, this, InternetSettingWindow::slotSwitchNetDevice);

    tpList<tpNetworkInterface> internetDeviceList = tpNetworkInterface::getAllDevice();
    for (const auto &internetDevice : internetDeviceList)
    {
        netDeviceListCbx_->addItem(internetDevice.getName());
    }

    // IPV4设置
    tpLabel *ipv4ConfigTitle = createGroupNameLabel("IPV4设置");
    ipv4ConfigTitle->installEventFilter(this);

    // 构建IPV4配置面板
    ipv4ConfigPanel_ = new tpMenuPanelWidget();
    configIpv4Panel();

    applyIpv4Btn_ = new tpButton();
    applyIpv4Btn_->setFixedHeight(50);
    applyIpv4Btn_->setRoundCorners(20);
    applyIpv4Btn_->setText("确定");
    applyIpv4Btn_->installEventFilter(this);
    connect(applyIpv4Btn_, onClicked, this, InternetSettingWindow::slotSaveIpv4Config);

    // 构建DNS配置面板
    tpLabel *dnsConfigTitle = createGroupNameLabel("DNS设置");
    dnsConfigTitle->installEventFilter(this);

    dnsConfigPanel_ = new tpMenuPanelWidget();
    configDnsPanel();

    applyDnsBtn_ = new tpButton();
    applyDnsBtn_->setFixedHeight(50);
    applyDnsBtn_->setRoundCorners(20);
    applyDnsBtn_->setText("确定");
    applyDnsBtn_->installEventFilter(this);
    connect(applyDnsBtn_, onClicked, this, InternetSettingWindow::slotSaveDnsConfig);

    mainLayout_->addWidget(selectNetDeviceTitle);
    mainLayout_->addWidget(netDeviceListCbx_);

    mainLayout_->addWidget(ipv4ConfigTitle);
    mainLayout_->addWidget(ipv4ConfigPanel_);
    mainLayout_->addWidget(applyIpv4Btn_);

    mainLayout_->addWidget(dnsConfigTitle);
    mainLayout_->addWidget(dnsConfigPanel_);
    mainLayout_->addWidget(applyDnsBtn_);

    setLayout(mainLayout_);
}

void InternetSettingWindow::configIpv4Panel()
{
    ipv4ConfigPanel_->installEventFilter(this);

    tpMenuPanelItem *menuPanelItem = new tpMenuPanelItem();
    menuPanelItem->setTitle("DHCP");
    isDhcpBtn_ = new tpOnOffButton();
    isDhcpBtn_->setFixedSize(45, 24);
    menuPanelItem->setCustomizeWidget(isDhcpBtn_);
    ipv4ConfigPanel_->addItem(menuPanelItem);

    tpMenuPanelItem *ipAddrItem = new tpMenuPanelItem();
    ipAddrItem->setTitle("IP地址");
    ipAddrEdit_ = new tpLineEdit();
    ipAddrEdit_->setPlaceholderText("请输入");
    ipAddrEdit_->setFixedSize(200, 30);
    ipAddrEdit_->setAlign(tinyPiX::AlignRight);
    ipAddrItem->setCustomizeWidget(ipAddrEdit_);
    ipv4ConfigPanel_->addItem(ipAddrItem);

    tpMenuPanelItem *maskItem = new tpMenuPanelItem();
    maskItem->setTitle("子网掩码");
    maskEdit_ = new tpLineEdit();
    maskEdit_->setPlaceholderText("请输入");
    maskEdit_->setFixedSize(200, 30);
    maskEdit_->setAlign(tinyPiX::AlignRight);
    maskItem->setCustomizeWidget(maskEdit_);
    ipv4ConfigPanel_->addItem(maskItem);

    tpMenuPanelItem *gatewayItem = new tpMenuPanelItem();
    gatewayItem->setTitle("网关");
    gatewayEdit_ = new tpLineEdit();
    gatewayEdit_->setPlaceholderText("请输入");
    gatewayEdit_->setFixedSize(200, 30);
    gatewayEdit_->setAlign(tinyPiX::AlignRight);
    gatewayItem->setCustomizeWidget(gatewayEdit_);
    ipv4ConfigPanel_->addItem(gatewayItem);
}

void InternetSettingWindow::configDnsPanel()
{
    dnsConfigPanel_->installEventFilter(this);

    tpMenuPanelItem *menuPanelItem = new tpMenuPanelItem();
    menuPanelItem->setTitle("自动获取");
    autoDnseBtn_ = new tpOnOffButton();
    autoDnseBtn_->setFixedSize(45, 24);
    menuPanelItem->setCustomizeWidget(autoDnseBtn_);
    dnsConfigPanel_->addItem(menuPanelItem);

    tpMenuPanelItem *firstDnsAddrItem = new tpMenuPanelItem();
    firstDnsAddrItem->setTitle("首选DNS服务器");
    firstDnsEdit_ = new tpLineEdit();
    firstDnsEdit_->setPlaceholderText("请输入");
    firstDnsEdit_->setFixedSize(200, 30);
    firstDnsEdit_->setAlign(tinyPiX::AlignRight);
    firstDnsAddrItem->setCustomizeWidget(firstDnsEdit_);
    dnsConfigPanel_->addItem(firstDnsAddrItem);

    tpMenuPanelItem *secondDnsItem = new tpMenuPanelItem();
    secondDnsItem->setTitle("备选DNS服务器");
    secondDnsEdit_ = new tpLineEdit();
    secondDnsEdit_->setPlaceholderText("请输入");
    secondDnsEdit_->setFixedSize(200, 30);
    secondDnsEdit_->setAlign(tinyPiX::AlignRight);
    secondDnsItem->setCustomizeWidget(secondDnsEdit_);
    dnsConfigPanel_->addItem(secondDnsItem);
}

void InternetSettingWindow::refreshNetDeviceInfo(const tpString &netName)
{
    // 设置网络状态;任意一网卡有网就显示连接状态
    tpNetworkInterface curNetInterface(netName);

    isDhcpBtn_->setOnOff(curNetInterface.isDhcp());
    if (isDhcpBtn_->onOff())
    {
        ipAddrEdit_->setText("");
        maskEdit_->setText("");
        gatewayEdit_->setText("");

        ipAddrEdit_->setReadOnly(true);
        maskEdit_->setReadOnly(true);
        gatewayEdit_->setReadOnly(true);

        autoDnseBtn_->setOnOff(!curNetInterface.isStaticDns());
        if (autoDnseBtn_->onOff())
        {
            firstDnsEdit_->setText("");
            secondDnsEdit_->setText("");

            firstDnsEdit_->setReadOnly(true);
            secondDnsEdit_->setReadOnly(true);
        }
        else
        {
            tpList<tpString> dnsStrList = curNetInterface.getDns();

            if (dnsStrList.size() > 0)
                firstDnsEdit_->setText(dnsStrList.front());
            if (dnsStrList.size() > 1)
                secondDnsEdit_->setText(dnsStrList.at(1));

            firstDnsEdit_->setReadOnly(false);
            secondDnsEdit_->setReadOnly(false);
        }
    }
    else
    {
        // 获取静态IP信息
        tpString ipStr = curNetInterface.getAddr();
        tpString maskStr = curNetInterface.getNetmask();
        tpString gatewayStr = curNetInterface.getGatway();

        tpList<tpString> dnsStrList = curNetInterface.getDns();

        ipAddrEdit_->setText(ipStr);
        maskEdit_->setText(maskStr);
        gatewayEdit_->setText(gatewayStr);

        if (dnsStrList.size() > 0)
            firstDnsEdit_->setText(dnsStrList.front());

        if (dnsStrList.size() > 1)
            secondDnsEdit_->setText(dnsStrList.at(1));

        ipAddrEdit_->setReadOnly(false);
        maskEdit_->setReadOnly(false);
        gatewayEdit_->setReadOnly(false);

        firstDnsEdit_->setReadOnly(false);
        secondDnsEdit_->setReadOnly(false);

        // 非DHCP；DNS必须手动设置
        autoDnseBtn_->setOnOff(false);
    }
}
