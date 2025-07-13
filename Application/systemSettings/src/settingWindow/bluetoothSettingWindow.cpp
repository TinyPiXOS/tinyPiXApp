#include "bluetoothSettingWindow.h"

bluetoothSettingWindow::bluetoothSettingWindow(tpChildWidget *parent)
    : settingBase(parent)
{
    initUi();
}

bluetoothSettingWindow::~bluetoothSettingWindow()
{
}

bool bluetoothSettingWindow::onResizeEvent(tpObjectResizeEvent *event)
{
    return true;
}

void bluetoothSettingWindow::initUi()
{
    // 本机信息
    tpLabel *localDeviceTitle = createGroupNameLabel("当前可被附近的蓝牙设备发现");
    localDeviceTitle->installEventFilter(this);

    // 构建IPV4配置面板
    lovalDevicePanel_ = new tpMenuPanelWidget();
    configLocalDevicePanel();

    // 构建已配对的设备
    tpLabel *connectDeviceTitle = createGroupNameLabel("已配对的设备");
    connectDeviceTitle->installEventFilter(this);

    connectDevicePanel_ = new tpMenuPanelWidget();
    connectDeviceItem_ = new tpMenuPanelItem();
    connectDeviceItem_->setTitle("无设备");
    // connectDeviceItem->setSubTitle("");
    connectDevicePanel_->addItem(connectDeviceItem_);

    // 可用设备
    tpLabel *usableDeviceTitle = createGroupNameLabel("可用设备");
    usableDeviceTitle->installEventFilter(this);

    usableDevicePanel_ = new tpMenuPanelWidget();
    tpMenuPanelItem* noDeviceItem = new tpMenuPanelItem();
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

void bluetoothSettingWindow::configLocalDevicePanel()
{
    tpMenuPanelItem *deiveNameItem = new tpMenuPanelItem();
    deiveNameItem->setTitle("设备名称");
    localDeviceNameEdit_ = new tpLineEdit();
    localDeviceNameEdit_->setText("tinyPix OS");
    localDeviceNameEdit_->setPlaceholderText("请输入");
    localDeviceNameEdit_->setFixedSize(200, 30);
    // localDeviceNameEdit_->setAlign(tinyPiX::AlignRight);
    deiveNameItem->setCustomizeWidget(localDeviceNameEdit_);
    lovalDevicePanel_->addItem(deiveNameItem);

    tpMenuPanelItem *recvFileItem = new tpMenuPanelItem();
    recvFileItem->setTitle("接收的文件");
    recvFileItem->setSubTitle("共0个");
    lovalDevicePanel_->addItem(recvFileItem);
}
