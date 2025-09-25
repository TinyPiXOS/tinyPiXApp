#include "browseWindow.h"
#include "TpHBoxLayout.h"
#include "TpVBoxLayout.h"
#include "TpDisplay.h"
#include "TpPainter.h"
#include "fileManagementGlobal.h"

static const TpString ITEM_PATH_TYPE = "FastPath";

browseWindow::browseWindow(TpChildWidget *parent)
    : TpChildWidget(parent)
{
    init();

    setBackGroundColor(_RGB(255, 255, 255));
    // setBackGroundColor(_RGB(248, 248, 248));

    connect(diskManager_, diskAdd, [=](TpDisk *)
            { std::cout << "******************diskAdd*****************" << std::endl; });
}

browseWindow::~browseWindow()
{
}

void browseWindow::setVisible(bool visible)
{
    TpChildWidget::setVisible(visible);

    if (!visible)
        return;

    TpChildWidget *scrollWidget = mainScrollPanel_->widget();
    TpVBoxLayout *deviceListLayout = dynamic_cast<TpVBoxLayout *>(scrollWidget->layout());

    if (!deviceListLayout)
        return;

    std::cout << "刷新外置存储设备" << std::endl;

    TpVector<TpObject *> layoutChildList = deviceListLayout->children();

    // 刷新USB设备数据
    TpVector<diskDeviceCheckBox *> childUsbItemList;
    for (const auto &childObj : layoutChildList)
    {
        diskDeviceCheckBox *childUsb = dynamic_cast<diskDeviceCheckBox *>(childObj);
        if (!childUsb)
            continue;

        childUsbItemList.emplace_back(childUsb);
    }

    for (const auto &delChild : childUsbItemList)
    {
        delChild->deleteLater();
        deviceListLayout->removeWidget(delChild);
    }

    deviceList_.clear();

    layoutChildList = deviceListLayout->children();

    // 获取所有外置磁盘数据
    TpList<TpDisk *> externDiskList = diskManager_->getList();
    std::cout << "externDiskList.Size  " << externDiskList.size() << std::endl;

    for (const auto &diskInfo : externDiskList)
    {
        if (!diskInfo->getRemovable())
            continue;

        // 总空间；已使用空间
        uint64_t allSpaceByte = diskInfo->getSpace();
        uint64_t usedSpaceByte = diskInfo->getUsedSize();

        std::cout << "allDiskInfo->getSectorSize() " << diskInfo->getSectorSize() << std::endl;
        std::cout << "allDiskInfo->getSectorNum() " << diskInfo->getSectorNum() << std::endl;

        std::cout << "allDiskInfo->getName() " << diskInfo->getName() << std::endl;
        std::cout << "allSpaceByte " << allSpaceByte << std::endl;
        std::cout << "usedSpaceByte " << usedSpaceByte << std::endl
                  << std::endl
                  << std::endl;

        // byte转GB
        double allSpaceGb = 1.0 * allSpaceByte / 1024 / 1024 / 1024;
        double usedSpaceGb = 1.0 * usedSpaceByte / 1024 / 1024 / 1024;

        TpString usbPath = diskInfo->getMount();

        diskDeviceCheckBox *testDevice = new diskDeviceCheckBox();
        testDevice->setIcon(applicationDirPath() + "/../res/USB设备-未选中.png", applicationDirPath() + "/../res/USB设备-选中.png");
        testDevice->setName(diskInfo->getName());
        testDevice->setSpace(usedSpaceGb, allSpaceGb);
        testDevice->installEventFilter(scrollWidget);
        testDevice->setProperty(ITEM_PATH_TYPE, usbPath);

        connect(testDevice, onClicked, [=](diskDeviceCheckBox *deviceBtn)
                { devicePathBtnClicked(deviceBtn); });
        deviceList_.emplace_back(testDevice);

        int32_t insertIndex = layoutChildList.size();
        deviceListLayout->insertWidget((insertIndex > 2) ? insertIndex - 2 : insertIndex, testDevice);

        // std::cout << "scrollWidget ... " << deviceListLayout->minumumSize().h << std::endl;
        scrollWidget->setMinumumHeight(deviceListLayout->minumumSize().height());
    }
}

bool browseWindow::onPaintEvent(TpPaintEvent *event)
{
    TpChildWidget::onPaintEvent(event);

    // 绘制左侧菜单滚动区域的底色
    TpPainter *paintCanvas = event->painter();

    paintCanvas->setPen( _RGB(248, 248, 248));
    paintCanvas->setBrush(TpBrush( _RGB(248, 248, 248)));

    paintCanvas->drawRect(0, 0, fileListWindow_->pos().x(), height());
    // paintCanvas->box(0, 0, fileListWindow_->pos().x()(), height(), _RGB(0, 0, 248));

    // 绘制分割线
    // paintCanvas->vline(mainScrollPanel_->width(), 0, height(), _RGB(190, 196, 202));

    return true;
}

void browseWindow::init()
{
    diskManager_ = new TpDiskManage(TP_TRUE);

    mainScrollPanel_ = new TpScrollPanel(this);
    mainScrollPanel_->setFixedWidth(TpDisplay::dp2Px(440));
    mainScrollPanel_->setBackGroundColor(_RGB(248, 248, 248));

    TpChildWidget *scrollWidget = new TpChildWidget();
    scrollWidget->setBackGroundColor(_RGB(248, 248, 248));

    fileListWindow_ = new fileListWindow();
    fileListWindow_->setRootPath(RootPath);
    fileListWindow_->setDeviceType(fileListWindow::LocalDevice);

    TpLabel *titleLabel = new TpLabel("浏览");
    titleLabel->font()->setFontSize(19);
    titleLabel->setFixedHeight(titleLabel->font()->pixelHeight());
    titleLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    titleLabel->installEventFilter(scrollWidget);
    // titleLabel->setBackGroundColor(_RGB(255, 0, 0));

    // 搜索
    searchEdit_ = new TpLineEdit();
    searchEdit_->setProperty("type", "fileManageEdit");
    searchEdit_->setIcon(applicationDirPath() + "/../res/搜索.png");
    searchEdit_->setPlaceholderText("搜索文件");

    // 快捷访问
    fastPathScroll_ = new TpScrollPanel();
    fastPathScroll_->setScrollMode(false);
    // fastPathScroll_->setBackGroundColor(_RGB(255, 0, 0));

    // 将快捷访问按钮放进中间widget，再放入滚动窗口
    TpChildWidget *fastScrollWidget = new TpChildWidget();

    pictureTileBtn_ = new TpMediaTileButton(fastPathScroll_);
    pictureTileBtn_->setProperty(ITEM_PATH_TYPE, "/System/data/Pictures");
    pictureTileBtn_->setText("图片");
    pictureTileBtn_->setIcon(applicationDirPath() + "/../res/fastAccess/图片.png");
    pictureTileBtn_->installEventFilter(fastScrollWidget);
    mediaBtnList_.emplace_back(pictureTileBtn_);
    connect(pictureTileBtn_, onClicked, [=](bool)
            { fastPathBtnClicked(pictureTileBtn_); });

    videoTileBtn_ = new TpMediaTileButton(fastPathScroll_);
    videoTileBtn_->setProperty(ITEM_PATH_TYPE, "/System/data/Videos");
    videoTileBtn_->setText("视频");
    videoTileBtn_->setIcon(applicationDirPath() + "/../res/fastAccess/视频.png");
    videoTileBtn_->installEventFilter(fastScrollWidget);
    mediaBtnList_.emplace_back(videoTileBtn_);
    connect(videoTileBtn_, onClicked, [=](bool)
            { fastPathBtnClicked(videoTileBtn_); });

    textTileBtn_ = new TpMediaTileButton(fastPathScroll_);
    textTileBtn_->setProperty(ITEM_PATH_TYPE, "/System/data/Documents");
    textTileBtn_->setText("文本");
    textTileBtn_->setIcon(applicationDirPath() + "/../res/fastAccess/文本.png");
    textTileBtn_->installEventFilter(fastScrollWidget);
    mediaBtnList_.emplace_back(textTileBtn_);
    connect(textTileBtn_, onClicked, [=](bool)
            { fastPathBtnClicked(textTileBtn_); });

    musicTileBtn_ = new TpMediaTileButton(fastPathScroll_);
    musicTileBtn_->setProperty(ITEM_PATH_TYPE, "/System/data/Music");
    musicTileBtn_->setText("音频");
    musicTileBtn_->setIcon(applicationDirPath() + "/../res/fastAccess/音频.png");
    musicTileBtn_->installEventFilter(fastScrollWidget);
    mediaBtnList_.emplace_back(musicTileBtn_);
    connect(musicTileBtn_, onClicked, [=](bool)
            { fastPathBtnClicked(musicTileBtn_); });

    applicationTileBtn_ = new TpMediaTileButton(fastPathScroll_);
    applicationTileBtn_->setProperty(ITEM_PATH_TYPE, "/System/data/Packages");
    applicationTileBtn_->setText("应用");
    applicationTileBtn_->setIcon(applicationDirPath() + "/../res/fastAccess/应用.png");
    applicationTileBtn_->installEventFilter(fastScrollWidget);
    mediaBtnList_.emplace_back(applicationTileBtn_);
    connect(applicationTileBtn_, onClicked, [=](bool)
            { fastPathBtnClicked(applicationTileBtn_); });

    // 将快速访问按钮添加到中间窗体
    fastPathScroll_->setFixedHeight(applicationTileBtn_->height() /* + TpDisplay::dp2Px(25)*/);
    // fastPathScroll_->setBackGroundColor(_RGB(255, 0, 0));

    fastScrollWidget->setLayout(new TpHBoxLayout());
    fastScrollWidget->layout()->setSpacing(12);
    fastScrollWidget->layout()->addWidget(pictureTileBtn_);
    fastScrollWidget->layout()->addWidget(videoTileBtn_);
    fastScrollWidget->layout()->addWidget(textTileBtn_);
    fastScrollWidget->layout()->addWidget(musicTileBtn_);
    fastScrollWidget->layout()->addWidget(applicationTileBtn_);
    fastScrollWidget->layout()->setContentsMargins(0, 0, 0, 0);
    // fastScrollWidget->setBackGroundColor(_RGB(255, 0, 0));
    fastPathScroll_->setWidget(fastScrollWidget);

    // 来源
    TpLabel *sourceLabel = new TpLabel("来源");
    sourceLabel->font()->setFontSize(11);
    sourceLabel->setFixedHeight(sourceLabel->font()->pixelHeight());
    sourceLabel->font()->setFontColor(_RGB(89, 89, 89), _RGB(89, 89, 89));
    sourceLabel->installEventFilter(scrollWidget);

    menuPanelWidget_ = new TpMenuPanelWidget();
    // menuPanelWidget_->setBackGroundColor(_RGB(255, 0, 0));
    menuPanelWidget_->installEventFilter(scrollWidget);
    connect(menuPanelWidget_, onClicked, this, &browseWindow::sourceMenuChanged);

#if 1 // 临时数据，TODO，要修改为从配置文件读取
    TpMenuPanelItem *downloadPanelItem = new TpMenuPanelItem();
    downloadPanelItem->setProperty(ITEM_PATH_TYPE, "/System/data/Downloads");
    downloadPanelItem->setIcon(applicationDirPath() + "/../res/fileSource/下载与接收.png");
    downloadPanelItem->setTitle("下载与接收");
    menuPanelWidget_->addItem(downloadPanelItem);
#endif

    TpVBoxLayout *menuLayout = new TpVBoxLayout();
    int32_t layoutMargin = TpDisplay::dp2Px(20);
    // menuLayout->setContentsMargins(0, 0, 0, 0);
    menuLayout->setContentsMargins(0, 0, layoutMargin, 0);
    menuLayout->setSpacing(10);

    menuLayout->addWidget(titleLabel);
    menuLayout->addWidget(searchEdit_);
    menuLayout->addWidget(fastPathScroll_);
    // menuLayout->addWidget(testDevice);
    menuLayout->addWidget(sourceLabel);
    menuLayout->addWidget(menuPanelWidget_);

    TpHBoxLayout *mainLayout = new TpHBoxLayout();
    mainLayout->setSpacing(0);
    // mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, 0);

    // 绑定滚动窗体，将滚动窗体放入主布局
    scrollWidget->setLayout(menuLayout);
    mainScrollPanel_->setWidget(scrollWidget);

    mainLayout->addWidget(mainScrollPanel_);
    mainLayout->addWidget(fileListWindow_);
    // mainLayout->addWidget(scrollFileWidget);

    setLayout(mainLayout);
}

void browseWindow::fastPathBtnClicked(TpMediaTileButton *clickBtn)
{
    // 屏蔽选中态重复选中
    bool isSelected = clickBtn->checked();
    if (!isSelected)
    {
        // clickBtn->setChecked(true);
        return;
    }

    // 移除所有媒体按钮选中
    for (const auto &mediaBtn : mediaBtnList_)
    {
        if (mediaBtn != clickBtn)
        {
            mediaBtn->setChecked(false);
        }
    }

    // 移除所有设备节点选中
    for (const auto &deviceBtn : deviceList_)
    {
        deviceBtn->setChecked(false);
    }

    // 移除所有来源按钮选中
    menuPanelWidget_->clearSelection();

    TpString curPath = clickBtn->property(ITEM_PATH_TYPE).toString();

    fileListWindow_->setRootPath(RootPath);

    fileListWindow_->setDeviceType(fileListWindow::LocalDevice);
    fileListWindow_->refreshPath(curPath);
}

void browseWindow::devicePathBtnClicked(diskDeviceCheckBox *clickBtn)
{
    std::cout << "devicePathBtnClicked " << std::endl;

    // 屏蔽选中态重复选中
    bool isSelected = clickBtn->checked();
    if (!isSelected)
    {
        // clickBtn->setChecked(true);
        return;
    }

    // 移除所有媒体按钮选中
    for (const auto &mediaBtn : mediaBtnList_)
    {
        mediaBtn->setChecked(false);
    }

    // 移除所有设备节点选中
    for (const auto &deviceBtn : deviceList_)
    {
        if (clickBtn != deviceBtn)
        {
            deviceBtn->setChecked(false);
        }
    }

    // 移除所有来源按钮选中
    menuPanelWidget_->clearSelection();

    TpString curPath = clickBtn->property(ITEM_PATH_TYPE).toString();
    std::cout << "curPath " << curPath << std::endl;

    // USB设备重新设置根目录
    fileListWindow_->setRootPath(curPath);

    // 显示安全弹出按钮
    fileListWindow_->setDeviceType(fileListWindow::USBDevice);
    fileListWindow_->refreshPath(curPath);
}

void browseWindow::sourceMenuChanged(TpMenuPanelItem *sourceItem)
{
    // 屏蔽选中态重复选中
    bool isSelected = sourceItem->checked();
    if (!isSelected)
    {
        // sourceItem->setChecked(true);
        return;
    }

    // 移除所有媒体按钮选中
    for (const auto &mediaBtn : mediaBtnList_)
    {
        mediaBtn->setChecked(false);
    }

    // 移除所有设备节点选中
    for (const auto &deviceBtn : deviceList_)
    {
        deviceBtn->setChecked(false);
    }

    TpString curPath = sourceItem->property(ITEM_PATH_TYPE).toString();
    fileListWindow_->setRootPath(RootPath);

    fileListWindow_->setDeviceType(fileListWindow::LocalDevice);
    fileListWindow_->refreshPath(curPath);
}
