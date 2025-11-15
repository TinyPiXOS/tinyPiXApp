#include "MainWindowService.h"
#include "TpSignalSlot.h"
#include "SystemSettingsGlobal.h"
#include "TpHBoxLayout.h"
#include "SystemInfo/TpDisplay.h"
#include "TpLabel.h"
#include "TpFont.h"
#include "TpOnOffButton.h"
#include "TpLine.h"
#include "TpBluetoothLocal.h"

#include "SettingWindow/InternetSettingWindow.h"
#include "SettingWindow/BluetoothSettingWindow.h"

const TpString SettingTypeStr = "settingType";

MainWindowService::MainWindowService()
    : TpMainWindow(), curSelectItem_(nullptr)
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    initUi();

    setBackGroundColor(_RGB(248, 248, 248));
}

MainWindowService::~MainWindowService()
{
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    // 每次应用打开刷新一下一级菜单初始状态
    refreshTopMenuStatus();
    // caculateRect();

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    // 获取首页状态
    // 获取蓝牙开关状态
    // 获取蓝牙设备状态;取第一个蓝牙设备的状态
    if (allMenuItemMapper_.contains(BlueToothSetting))
    {
        TpOnOffButton *bluetoothOnOffBtn = dynamic_cast<TpOnOffButton *>(allMenuItemMapper_.value(BlueToothSetting)->customizeWidget());
        if (bluetoothOnOffBtn)
        {
            bool blueIsOpen = false;
            TpList<TpBluetoothLocal> blueToothDeviceList = TpBluetoothLocal::getAllDevice();
            if (blueToothDeviceList.size() > 0)
            {
                TpBluetoothLocal& firstBlueDevice = blueToothDeviceList.front();
                blueIsOpen = firstBlueDevice.isPowerOn();
            }

            bluetoothOnOffBtn->setOnOff(blueIsOpen);
        }
    }

    // 获取WIFI开关状态 TODO

    return true;
}

void MainWindowService::initUi()
{
    TpString resPath = applicationDirPath() + "/../res/";

    mainScrollPanel_ = new TpScrollPanel(this);
    // mainScrollPanel_->setFixedWidth(TpDisplay::dp2Px(405));
    mainScrollPanel_->setFixedWidth(width() * 0.375);
    mainScrollPanel_->setBackGroundColor(_RGB(248, 248, 248));

    TpWidget *scrollWidget = new TpWidget(mainScrollPanel_);
    scrollWidget->setBackGroundColor(_RGB(248, 248, 248));

    TpLabel *titleLabel = new TpLabel("设置");
    titleLabel->font()->setFontSize(19);
    titleLabel->setFixedHeight(titleLabel->font()->pixelHeight());
    titleLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    titleLabel->installEventFilter(scrollWidget);
    // titleLabel->setBackGroundColor(_RGB(255, 0, 0));

    // 搜索
    searchEdit_ = new TpLineEdit();
    searchEdit_->setProperty("type", "fileManageEdit");
    searchEdit_->setIcon(applicationDirPath() + "/../res/搜索.png");
    searchEdit_->setPlaceholderText("搜索设置项");

    // 右侧显示功能窗口
    settingWindow_ = new TpScrollPanel(this);

    // 右侧功能窗口title布局
    TpHBoxLayout *subTitleLayout = new TpHBoxLayout();
    subTitleLayout->setSpacing(20);
    subTitleLayout->setContentsMargins(0, 0, 0, 0);

    subTitleLabel_ = new TpLabel("");
    // titleLabel->setBackGroundColor(_RGB(255, 0, 0));
    subTitleLabel_->font()->setFontSize(19);
    subTitleLabel_->setFixedHeight(titleLabel->font()->pixelHeight());
    subTitleLabel_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    subTitleLabel_->setText("");
    subTitleLabel_->installEventFilter(settingWindow_);

    subBackBtn_ = new TpButton();
    subBackBtn_->setButtonStyle(TpButton::IconOnly);
    subBackBtn_->setEnableBackGroundColor(false);
    subBackBtn_->setFixedSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    subBackBtn_->setIcon(applicationDirPath() + "/../res/路径后退.png");
    subBackBtn_->setVisible(false);
    connect(subBackBtn_, onClicked, this, &MainWindowService::slotClickBackBtn);

    subTitleLayout->addWidget(subBackBtn_);
    subTitleLayout->addWidget(subTitleLabel_);
    subTitleLayout->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Expanding, TpSpacerItem::Minimum));

    int32_t layoutMargin = TpDisplay::dp2Px(20);

    TpVBoxLayout *rightWindowLayout = new TpVBoxLayout();
    rightWindowLayout->setContentsMargins(layoutMargin, 0, layoutMargin, 0);
    rightWindowLayout->addLayout(subTitleLayout);
    rightWindowLayout->addWidget(settingWindow_);

    TpVBoxLayout *menuLayout = new TpVBoxLayout();
    // menuLayout->setContentsMargins(0, 0, 0, 0);
    menuLayout->setContentsMargins(0, 0, layoutMargin, 0);
    menuLayout->setSpacing(18);

    menuLayout->addWidget(titleLabel);
    menuLayout->addWidget(searchEdit_);
    createAllSettingTopMenu(menuLayout);

    // 最后添加一个弹簧，确保控件紧密排列
    // menuLayout->addWidget(searchEdit_);

    TpHBoxLayout *mainLayout = new TpHBoxLayout();
    mainLayout->setSpacing(0);
    // mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, 0);

    // 绑定滚动窗体，将滚动窗体放入主布局
    scrollWidget->setLayout(menuLayout);
    mainScrollPanel_->setWidget(scrollWidget);

    TpLine *subLine = new TpLine();
    subLine->setLineType(TpLine::VLine);
    subLine->setColor(_RGB(190, 196, 202));
    subLine->setFixedWidth(2);

    mainLayout->addWidget(mainScrollPanel_);
    mainLayout->addWidget(subLine);
    mainLayout->addLayout(rightWindowLayout);

    std::cout << "*******" << width() << "  " << height();

    setLayout(mainLayout);

    // 第一次显示刷新默认状态
    refreshTopMenuStatus();

    // 创建子窗口
    createAllSettingWindow();
}

void MainWindowService::slotClickMenuItem(TpMenuPanelItem *deviceBtn)
{
    TpMenuPanelWidget *curPanel = dynamic_cast<TpMenuPanelWidget *>(deviceBtn->parent());
    for (const auto &panel : allMenuPanelList_)
    {
        if (panel == curPanel)
            continue;

        panel->clearSelection();
    }

    curSelectItem_ = deviceBtn;

    // 获取点击的设置类型
    SettingType clickSettingType = static_cast<SettingType>(deviceBtn->property(SettingTypeStr).toInt32());
    subTitleLabel_->setText(generalSettingNames(clickSettingType));

    if (allMenuWidgetMapper_.contains(clickSettingType))
    {
        const auto &curMenuWidget = allMenuWidgetMapper_.value(clickSettingType);
        curMenuWidget->refreshData();
        settingWindow_->setWidget(curMenuWidget);
    }
    else
    {
        settingWindow_->setWidget(nullptr);
    }

    // if (!deviceBtn->checked())
    // deviceBtn->setChecked(true);
}

void MainWindowService::slotClickBackBtn(bool)
{
    if (!curSelectItem_)
        return;

    // 获取当前选中的一级功能菜单
    SettingType clickSettingType = static_cast<SettingType>(curSelectItem_->property(SettingTypeStr).toInt32());

    if (allMenuWidgetMapper_.contains(clickSettingType))
    {
        allMenuWidgetMapper_[clickSettingType]->slotBack();
    }
}

void MainWindowService::createAllSettingTopMenu(TpVBoxLayout *menuLayout)
{
    TpList<TpList<SettingType>> itemNameList;
    itemNameList.emplace_back(TpList<SettingType>{WLANSetting, BlueToothSetting, InternetSetting, HotspotSetting});
    itemNameList.emplace_back(TpList<SettingType>{WallPaperSetting, DisplaySetting, DesktopSetting});
    itemNameList.emplace_back(TpList<SettingType>{VoiceSetting});
    itemNameList.emplace_back(TpList<SettingType>{DeviceSetting, MoreSetting, ApplicationSetting});

    for (const auto &menuPanel : itemNameList)
    {
        // 功能菜单
        TpMenuPanelWidget *menuPanelWidget = new TpMenuPanelWidget();
        menuPanelWidget->installEventFilter(mainScrollPanel_);
        connect(menuPanelWidget, onClicked, this, &MainWindowService::slotClickMenuItem);
        // menuPanelWidget->setBackGroundColor(_RGB(255, 0, 0));

        for (const auto &menuType : menuPanel)
        {
            TpString menuItemName = generalSettingNames(menuType);

            TpMenuPanelItem *menuPanelItem = new TpMenuPanelItem();
            menuPanelItem->setIcon(applicationDirPath() + "/../res/menuItem/" + menuItemName + ".png");
            menuPanelItem->setTitle(menuItemName);
            menuPanelItem->setProperty(SettingTypeStr, (int32_t)menuType);

            TpWidget *customWidget = generalCustomWidget(menuType);
            if (customWidget)
            {
                menuPanelItem->setCustomizeWidget(customWidget);
            }

            menuPanelWidget->addItem(menuPanelItem);
            allMenuItemMapper_[menuType] = menuPanelItem;
        }

        allMenuPanelList_.emplace_back(menuPanelWidget);
        menuLayout->addWidget(menuPanelWidget);
    }
}

TpString MainWindowService::generalSettingNames(const SettingType &type)
{
    switch (type)
    {
    case WLANSetting:
        return "WLAN";
    case BlueToothSetting:
        return "蓝牙";
    case InternetSetting:
        return "以太网";
    case HotspotSetting:
        return "热点";
    case WallPaperSetting:
        return "壁纸设置";
    case DisplaySetting:
        return "显示设置";
    case DesktopSetting:
        return "桌面设置";
    case VoiceSetting:
        return "声音设置";
    case DeviceSetting:
        return "设备信息";
    case MoreSetting:
        return "更多设置";
    case ApplicationSetting:
        return "应用设置";
    }

    return "未命名设置";
}

TpWidget *MainWindowService::generalCustomWidget(const SettingType &type)
{
    if (type == BlueToothSetting)
    {
        TpOnOffButton *onOffBnt = new TpOnOffButton();
        onOffBnt->setFixedSize(45, 24);
        return onOffBnt;
    }

    return nullptr;
}

void MainWindowService::createAllSettingWindow()
{
    allMenuWidgetMapper_[InternetSetting] = new InternetSettingWindow();
    allMenuWidgetMapper_[InternetSetting]->setTitleBackInfo(subTitleLabel_, subBackBtn_);

    allMenuWidgetMapper_[BlueToothSetting] = new BluetoothSettingWindow();
    allMenuWidgetMapper_[BlueToothSetting]->setTitleBackInfo(subTitleLabel_, subBackBtn_);
}

void MainWindowService::refreshTopMenuStatus()
{
    // TODO 获取真实设备状态刷新
    allMenuItemMapper_[WLANSetting]->setSubTitle("关闭");
    allMenuItemMapper_[HotspotSetting]->setSubTitle("关闭");

    TpOnOffButton *bluetoothOnOffBtn = dynamic_cast<TpOnOffButton *>(allMenuItemMapper_[BlueToothSetting]->customizeWidget());
    if (bluetoothOnOffBtn)
    {
        bluetoothOnOffBtn->setOnOff(false);
    }

    TpList<TpBluetoothLocal> bluetoothDeviceList = TpBluetoothLocal::getAllDevice();
    for (auto &bluetoothDevice : bluetoothDeviceList)
    {
        std::cout << "蓝牙设备名称:" << bluetoothDevice.getName() << std::endl;
    }
}
