#include "InternetSettingWindow.h"
#include "TpNetworkInterface.h"
#include "TpVBoxLayout.h"
#include "TpMessageBox.h"

InternetSettingWindow::InternetSettingWindow(TpWidget *parent)
    : SettingBase(parent), netDeviceListCbx_(nullptr), ipv4ConfigPanel_(nullptr)
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
    TpList<TpNetworkInterface> internetDeviceList = TpNetworkInterface::getAllDevice();
    for (const auto &internetDevice : internetDeviceList)
    {
        // std::cout << "网卡名称:" << internetDevice.getName() << std::endl;
        TpListWidgetItem *curItem = netDeviceListCbx_->addItem(internetDevice.getName());
    }

    TpListWidgetItem *curSelectItem = netDeviceListCbx_->currentItem().front();
    if (curSelectItem)
    {
        refreshNetDeviceInfo(curSelectItem->text());
    }
}

bool InternetSettingWindow::onResizeEvent(TpResizeEvent *event)
{
    return true;
}

void InternetSettingWindow::slotSaveIpv4Config(bool)
{
    TpVector<TpListWidgetItem *> selectItemList = netDeviceListCbx_->currentItem();
    if (selectItemList.size() == 0)
    {
        infoMsgWin_->setMessageType(TpMessageBox::Information);
        infoMsgWin_->setText("未选择网卡!");
        infoMsgWin_->exec();
        return;
    }

    TpListWidgetItem *curSelectItem = selectItemList.front();
    if (!curSelectItem)
        return;

    TpString curDeviceName = curSelectItem->text();
    TpNetworkInterface curNetInterface(curDeviceName);

    if (isDhcpBtn_->onOff())
    {
        curNetInterface.setDhcp();
    }
    else
    {
        infoMsgWin_->setMessageType(TpMessageBox::Information);

        if (ipAddrEdit_->text().empty())
        {
            infoMsgWin_->setText("IP地址为空!");
            infoMsgWin_->exec();
            return;
        }

        if (maskEdit_->text().empty())
        {
            infoMsgWin_->setText("子网掩码为空!");
            infoMsgWin_->exec();
            return;
        }

        if (gatewayEdit_->text().empty())
        {
            infoMsgWin_->setText("网关为空!");
            infoMsgWin_->exec();
            return;
        }

        // 静态IP必须要设置DNS
        if (autoDnseBtn_->onOff())
        {
            infoMsgWin_->setText("DHCP模式下必须手动设置DNS!");
            infoMsgWin_->exec();
            return;
        }

        if (firstDnsEdit_->text().empty())
        {
            infoMsgWin_->setText("DNS为空!");
            infoMsgWin_->exec();
            return;
        }

        TpList<TpString> dnsList;
        TpString dns1 = firstDnsEdit_->text();
        if (!dns1.empty())
            dnsList.emplace_back(dns1);

        TpString dns2 = secondDnsEdit_->text();
        if (!dns2.empty())
            dnsList.emplace_back(dns2);

        curNetInterface.setStatic(ipAddrEdit_->text(), gatewayEdit_->text(), maskEdit_->text(), dnsList);
    }

    infoMsgWin_->setText("设置完成!");
    infoMsgWin_->exec();
}

void InternetSettingWindow::slotSaveDnsConfig(bool)
{
    TpVector<TpListWidgetItem *> selectItemList = netDeviceListCbx_->currentItem();
    if (selectItemList.size() == 0)
    {
        infoMsgWin_->setText("未选择网卡!");
        infoMsgWin_->exec();
        return;
    }

    TpListWidgetItem *curSelectItem = selectItemList.front();
    if (!curSelectItem)
        return;

    TpString curDeviceName = curSelectItem->text();
    TpNetworkInterface curNetInterface(curDeviceName);

    // 非DHCP；DNS必须手动设置
    if (isDhcpBtn_->onOff())
    {
        if (firstDnsEdit_->text().empty())
        {
            infoMsgWin_->setText("DNS为空!");
            infoMsgWin_->exec();
            return;
        }
    }
    else
    {
        if (autoDnseBtn_->onOff())
        {
            infoMsgWin_->setText("静态IP模式下必须手动设置DNS!");
            infoMsgWin_->exec();
            return;
        }
    }

    // 设置动态DNS或静态DNS
    TpList<TpString> dnsList;
    TpString dns1 = firstDnsEdit_->text();
    if (!dns1.empty())
        dnsList.emplace_back(dns1);

    TpString dns2 = secondDnsEdit_->text();
    if (!dns2.empty())
        dnsList.emplace_back(dns2);

    curNetInterface.setDns(autoDnseBtn_->onOff() ? TP_TRUE : TP_FALSE, dnsList);
}

void InternetSettingWindow::slotSwitchNetDevice(TpListWidgetItem *item)
{
    if (!item)
        return;

    if (!item->checked())
        return;

    TpString curNetDeviceName = item->text();

    // 刷新UI显示
    refreshNetDeviceInfo(curNetDeviceName);
}

void InternetSettingWindow::initUi()
{
    TpLabel *selectNetDeviceTitle = createGroupNameLabel("选择网卡");
    selectNetDeviceTitle->installEventFilter(this);

    // 网卡列表
    netDeviceListCbx_ = new TpListWidget();
    netDeviceListCbx_->installEventFilter(this);
    connect(netDeviceListCbx_, onStatusChanged, this, &InternetSettingWindow::slotSwitchNetDevice);

    TpList<TpNetworkInterface> internetDeviceList = TpNetworkInterface::getAllDevice();
    for (const auto &internetDevice : internetDeviceList)
    {
        netDeviceListCbx_->addItem(internetDevice.getName());
    }

    // IPV4设置
    TpLabel *ipv4ConfigTitle = createGroupNameLabel("IPV4设置");
    ipv4ConfigTitle->installEventFilter(this);

    // 构建IPV4配置面板
    ipv4ConfigPanel_ = new TpMenuPanelWidget();
    configIpv4Panel();

    applyIpv4Btn_ = new TpButton();
    applyIpv4Btn_->setFixedHeight(50);
    applyIpv4Btn_->setRoundCorners(20);
    applyIpv4Btn_->setText("确定");
    applyIpv4Btn_->installEventFilter(this);
    connect(applyIpv4Btn_, onClicked, this, &InternetSettingWindow::slotSaveIpv4Config);

    // 构建DNS配置面板
    TpLabel *dnsConfigTitle = createGroupNameLabel("DNS设置");
    dnsConfigTitle->installEventFilter(this);

    dnsConfigPanel_ = new TpMenuPanelWidget();
    configDnsPanel();

    applyDnsBtn_ = new TpButton();
    applyDnsBtn_->setFixedHeight(50);
    applyDnsBtn_->setRoundCorners(20);
    applyDnsBtn_->setText("确定");
    applyDnsBtn_->installEventFilter(this);
    connect(applyDnsBtn_, onClicked, this, &InternetSettingWindow::slotSaveDnsConfig);

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

    TpMenuPanelItem *menuPanelItem = new TpMenuPanelItem();
    menuPanelItem->setTitle("DHCP");
    isDhcpBtn_ = new TpSwitchButton();
    isDhcpBtn_->setFixedSize(45, 24);
    menuPanelItem->setCustomizeWidget(isDhcpBtn_);
    ipv4ConfigPanel_->addItem(menuPanelItem);

    TpMenuPanelItem *ipAddrItem = new TpMenuPanelItem();
    ipAddrItem->setTitle("IP地址");
    ipAddrEdit_ = new TpLineEdit();
    ipAddrEdit_->setPlaceholderText("请输入");
    ipAddrEdit_->setFixedSize(200, 30);
    ipAddrEdit_->setAlign(Tp::AlignRight);
    ipAddrItem->setCustomizeWidget(ipAddrEdit_);
    ipv4ConfigPanel_->addItem(ipAddrItem);

    TpMenuPanelItem *maskItem = new TpMenuPanelItem();
    maskItem->setTitle("子网掩码");
    maskEdit_ = new TpLineEdit();
    maskEdit_->setPlaceholderText("请输入");
    maskEdit_->setFixedSize(200, 30);
    maskEdit_->setAlign(Tp::AlignRight);
    maskItem->setCustomizeWidget(maskEdit_);
    ipv4ConfigPanel_->addItem(maskItem);

    TpMenuPanelItem *gatewayItem = new TpMenuPanelItem();
    gatewayItem->setTitle("网关");
    gatewayEdit_ = new TpLineEdit();
    gatewayEdit_->setPlaceholderText("请输入");
    gatewayEdit_->setFixedSize(200, 30);
    gatewayEdit_->setAlign(Tp::AlignRight);
    gatewayItem->setCustomizeWidget(gatewayEdit_);
    ipv4ConfigPanel_->addItem(gatewayItem);
}

void InternetSettingWindow::configDnsPanel()
{
    dnsConfigPanel_->installEventFilter(this);

    TpMenuPanelItem *menuPanelItem = new TpMenuPanelItem();
    menuPanelItem->setTitle("自动获取");
    autoDnseBtn_ = new TpSwitchButton();
    autoDnseBtn_->setFixedSize(45, 24);
    menuPanelItem->setCustomizeWidget(autoDnseBtn_);
    dnsConfigPanel_->addItem(menuPanelItem);

    TpMenuPanelItem *firstDnsAddrItem = new TpMenuPanelItem();
    firstDnsAddrItem->setTitle("首选DNS服务器");
    firstDnsEdit_ = new TpLineEdit();
    firstDnsEdit_->setPlaceholderText("请输入");
    firstDnsEdit_->setFixedSize(200, 30);
    firstDnsEdit_->setAlign(Tp::AlignRight);
    firstDnsAddrItem->setCustomizeWidget(firstDnsEdit_);
    dnsConfigPanel_->addItem(firstDnsAddrItem);

    TpMenuPanelItem *secondDnsItem = new TpMenuPanelItem();
    secondDnsItem->setTitle("备选DNS服务器");
    secondDnsEdit_ = new TpLineEdit();
    secondDnsEdit_->setPlaceholderText("请输入");
    secondDnsEdit_->setFixedSize(200, 30);
    secondDnsEdit_->setAlign(Tp::AlignRight);
    secondDnsItem->setCustomizeWidget(secondDnsEdit_);
    dnsConfigPanel_->addItem(secondDnsItem);
}

void InternetSettingWindow::refreshNetDeviceInfo(const TpString &netName)
{
    // 设置网络状态;任意一网卡有网就显示连接状态
    TpNetworkInterface curNetInterface(netName);

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
            TpList<TpString> dnsStrList = curNetInterface.getDns();

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
        TpString ipStr = curNetInterface.getAddr();
        TpString maskStr = curNetInterface.getNetmask();
        TpString gatewayStr = curNetInterface.getGatway();

        TpList<TpString> dnsStrList = curNetInterface.getDns();

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
