#include "BluetoothSettingWindow.h"

BluetoothSettingWindow::BluetoothSettingWindow(TpWidget *parent)
    : SettingBase(parent)
{
    initUi();
}

BluetoothSettingWindow::~BluetoothSettingWindow()
{
}

bool BluetoothSettingWindow::onResizeEvent(TpResizeEvent *event)
{
    return true;
}

void BluetoothSettingWindow::initUi()
{
    // 本机信息
    TpLabel *localDeviceTitle = createGroupNameLabel("当前可被附近的蓝牙设备发现");
    localDeviceTitle->installEventFilter(this);

    // 构建IPV4配置面板
    lovalDevicePanel_ = new TpMenuPanelWidget();
    configLocalDevicePanel();

    // 构建已配对的设备
    TpLabel *connectDeviceTitle = createGroupNameLabel("已配对的设备");
    connectDeviceTitle->installEventFilter(this);

    connectDevicePanel_ = new TpMenuPanelWidget();
    connectDeviceItem_ = new TpMenuPanelItem();
    connectDeviceItem_->setTitle("无设备");
    // connectDeviceItem->setSubTitle("");
    connectDevicePanel_->addItem(connectDeviceItem_);

    // 可用设备
    TpLabel *usableDeviceTitle = createGroupNameLabel("可用设备");
    usableDeviceTitle->installEventFilter(this);

    usableDevicePanel_ = new TpMenuPanelWidget();
    TpMenuPanelItem* noDeviceItem = new TpMenuPanelItem();
    noDeviceItem->setTitle("无设备");
    // connectDeviceItem->setSubTitle("");
    usableDevicePanel_->addItem(noDeviceItem);

    mainLayout_->addWidget(localDeviceTitle);
    mainLayout_->addWidget(lovalDevicePanel_);

    mainLayout_->addWidget(connectDeviceTitle);
    mainLayout_->addWidget(connectDevicePanel_);

    mainLayout_->addWidget(usableDeviceTitle);
    mainLayout_->addWidget(usableDevicePanel_);

    setLayout(mainLayout_);
}

void BluetoothSettingWindow::configLocalDevicePanel()
{
    TpMenuPanelItem *deiveNameItem = new TpMenuPanelItem();
    deiveNameItem->setTitle("设备名称");
    localDeviceNameEdit_ = new TpLineEdit();
    localDeviceNameEdit_->setText("tinyPix OS");
    localDeviceNameEdit_->setPlaceholderText("请输入");
    localDeviceNameEdit_->setFixedSize(200, 30);
    // localDeviceNameEdit_->setAlign(Tp::AlignRight);
    deiveNameItem->setCustomizeWidget(localDeviceNameEdit_);
    lovalDevicePanel_->addItem(deiveNameItem);

    TpMenuPanelItem *recvFileItem = new TpMenuPanelItem();
    recvFileItem->setTitle("接收的文件");
    recvFileItem->setSubTitle("共0个");
    lovalDevicePanel_->addItem(recvFileItem);
}
