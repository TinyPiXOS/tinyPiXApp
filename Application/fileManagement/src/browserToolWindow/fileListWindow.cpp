#include "fileListWindow.h"
#include "tpDisplay.h"
#include "tpVBoxLayout.h"
#include "tpHBoxLayout.h"
#include "tpLabel.h"
#include "tpFont.h"
#include "tpDir.h"
#include "tpFileInfo.h"
#include "tpMessageBox.h"
#include "tpJsonDocument.h"
#include "tpJsonObject.h"
#include "tpFile.h"
#include "tpDesktopGlobal.h"
#include "tpGateway.h"
#include "tpSystemApi.h"

fileListWindow::fileListWindow(tpChildWidget *parent)
    : tpScrollPanel(parent), rootPath_(""), curShowPath_(""), mode_(fileListWindow::Grid)
{
    init();

    initializeGateway();
}

fileListWindow::~fileListWindow()
{
}

void fileListWindow::setShowMode(const FileListMode &mode)
{
    mode_ = mode;

    if (mode_ == FileListMode::Grid)
    {
        mainLayout_->removeLayout(listFileLayout_);
        mainLayout_->insertLayout(2, gridFileLayout_);
    }
    else
    {
        mainLayout_->removeLayout(gridFileLayout_);
        mainLayout_->insertLayout(2, listFileLayout_);
    }

    refreshPath(curShowPath_);

    // update();
}

void fileListWindow::setRootPath(const tpString &path)
{
    rootPath_ = path;
    filePathWidget_->setRootPath(rootPath_);
    refreshPath(rootPath_);
}

void fileListWindow::setDeviceType(const DeviceType &deviceType)
{
    deviceType_ = deviceType;

    if (deviceType_ == LocalDevice)
    {
        popBtn_->setVisible(false);
    }
    else
    {
        popBtn_->setVisible(true);
    }

    update();
}

void fileListWindow::refreshPath(const tpString &path)
{
    // std::cout << "******************************************* " << std::endl;
    // std::cout << "dealPath " << path << std::endl;
    // std::cout << "******************************************* " << std::endl;

    tpDir showDir(path);

    // 文件夹不存在直接不处理
    if (!showDir.exists())
        return;

    // 判断刷新目录是不是根目录
    tpFileInfo rootPath(rootPath_);
    tpFileInfo curPath(path);

    tpString rootCanoicalPath = rootPath.canonicalFilePath();
    tpString curCanoicalPath = curPath.canonicalFilePath();

    filePathWidget_->setPath(curCanoicalPath);

    if (rootCanoicalPath.compare(curCanoicalPath) != 0)
    {
        // 显示路径窗体和返回箭头
        pathBackBtn_->setVisible(true);
        filePathWidget_->setVisible(true);
    }
    else
    {
        pathBackBtn_->setVisible(false);
        filePathWidget_->setVisible(false);
    }

    curShowPath_ = path;

    // 释放旧路径的文件item指针
    tpVector<tpObject *> flexChildList = gridFileLayout_->children();
    tpVector<tpObject *> vboxChildList = listFileLayout_->children();

    for (const auto &child : flexChildList)
    {
        child->deleteLater();
    }
    for (const auto &child : vboxChildList)
    {
        child->deleteLater();
    }
    gridFileLayout_->clear();
    listFileLayout_->clear();

    // 遍历文件夹下所有数据
    for (const tpFileInfo &fileInfo : showDir.entryInfoList())
    {
        if (mode_ == FileListMode::Grid)
        {
            tpCollapsibleFileItem *item = new tpCollapsibleFileItem();
            // item->setFixedSize(100, 100);
            // item->setBackGroundColor(_RGB(255, 0, 0));
            item->setName(fileInfo.fileName());
            item->setPath(fileInfo.absoluteFilePath());
            item->installEventFilter(this);

            connect(item, onClicked, this, fileListWindow::SlotClickFileItem);

            gridFileLayout_->addWidget(item);
        }
        else
        {
            listFileItem *item = new listFileItem();
            item->setFixedHeight(75);
            // item->setFixedSize(100, 100);
            // item->setBackGroundColor(_RGB(255, 0, 0));
            item->setName(fileInfo.fileName());
            item->setPath(fileInfo.absoluteFilePath());
            item->installEventFilter(this);

            connect(item, onClicked, this, fileListWindow::SlotClickListFileItem);

            listFileLayout_->addWidget(item);
        }
    }

    tpChildWidget *gridWidget = dynamic_cast<tpChildWidget *>(gridFileLayout_->parent());
    tpChildWidget *listWidget = dynamic_cast<tpChildWidget *>(listFileLayout_->parent());

    if (gridWidget)
        gridWidget->update();
    if (listWidget)
        listWidget->update();

    // gridFileLayout_->update();
    // listFileLayout_->update();

    update();

    std::cout << "fileListWindow 最小高度 " << mainLayout_->minumumSize().h << std::endl;

    widget()->setMinumumHeight(mainLayout_->minumumSize().h);
}

void fileListWindow::SlotShowSetting(bool)
{
    settingWindow_->setVisible(true);
    settingWindow_->update();

    // std::cout << "  settingWindow_  " << settingWindow_->width() << "  " << settingWindow_->height() << std::endl;
}

void fileListWindow::SlotPopUsb(bool)
{
    tpMessageBox::information("设备已退出");
}

void fileListWindow::init()
{
    tpChildWidget *scrollWidget = new tpChildWidget();
    // scrollWidget->installEventFilter(this);

    imagePreviewWidget_ = new tpImageWidget();

    mainLayout_ = new tpVBoxLayout();
    mainLayout_->setSpacing(13);
    mainLayout_->setContentsMargins(tpDisplay::dp2Px(20), 0, 0, 0);

    tpHBoxLayout *titleLayout = new tpHBoxLayout();
    titleLayout->setSpacing(20);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    gridFileLayout_ = new tpFlexLayout();
    gridFileLayout_->setContentsMargins(0, 0, 0, 0);
    gridFileLayout_->installEventFilter(scrollWidget);

    listFileLayout_ = new tpVBoxLayout();
    listFileLayout_->setContentsMargins(0, 0, 0, 0);
    listFileLayout_->installEventFilter(scrollWidget);

    tpLabel *titleLabel = new tpLabel("内部存储");
    // titleLabel->setBackGroundColor(_RGB(255, 0, 0));
    titleLabel->font()->setFontSize(19);
    titleLabel->setFixedHeight(titleLabel->font()->pixelHeight());
    titleLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    titleLabel->setText("内部存储");
    titleLabel->installEventFilter(scrollWidget);

    pathBackBtn_ = new tpButton();
    pathBackBtn_->setButtonStyle(tpButton::IconOnly);
    pathBackBtn_->setEnableBackGroundColor(false);
    pathBackBtn_->setFixedSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    pathBackBtn_->setIcon(applicationDirPath() + "/../res/路径后退.png");
    pathBackBtn_->setVisible(false);
    connect(pathBackBtn_, onClicked, [=](bool)
            { SlotBackPath(); });

    popBtn_ = new usbPopButton();
    connect((tpButton *)popBtn_, onClicked, this, fileListWindow::SlotPopUsb);

    // 功能设置窗口
    settingWindow_ = new settingWindow();
    connect(settingWindow_, onChangeViewMode, [=](uint32_t index)
            { setShowMode(FileListMode(index)); });

    settingBtn_ = new tpButton();
    settingBtn_->setButtonStyle(tpButton::IconOnly);
    settingBtn_->setEnableBackGroundColor(false);
    settingBtn_->setFixedSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    settingBtn_->setIcon(applicationDirPath() + "/../res/功能设置.png");
    connect(settingBtn_, onClicked, this, fileListWindow::SlotShowSetting);

    filePathWidget_ = new tpFilePathWidget();
    filePathWidget_->setVisible(false);
    connect(filePathWidget_, onPathChanged, [=](const tpString &lastPath, const tpString &curPath)
            { refreshPath(curPath); });

    titleLayout->addWidget(pathBackBtn_);
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(popBtn_);
    titleLayout->addSpacer(new tpSpacerItem(20, 20, tpSpacerItem::Expanding, tpSpacerItem::Minimum));
    titleLayout->addWidget(settingBtn_);
    titleLayout->installEventFilter(scrollWidget);

    mainLayout_->addLayout(titleLayout);
    mainLayout_->addWidget(filePathWidget_);

    // TODO 测试列表模式
    // mainLayout_->addLayout(listFileLayout_);
    // mode_ = FileListMode::List;
    // 默认显示网格模式
    mainLayout_->addLayout(gridFileLayout_);
    mainLayout_->addSpacer(new tpSpacerItem(20, 20, tpSpacerItem::Minimum, tpSpacerItem::Expanding));

    scrollWidget->setLayout(mainLayout_);
    // scrollWidget->setBackGroundColor(_RGB(0, 0, 255));

    // setLayout(titleLayout);
    // setLayout(mainLayout_);
    setWidget(scrollWidget);
}

void fileListWindow::SlotClickFileItem(tpCollapsibleFileItem *item)
{
    if (!item)
        return;

    tpString curItemPath = item->path();
    tpFileInfo fileInfo(curItemPath);

    if (fileInfo.isDir())
    {
        // 刷新子目录的数据
        refreshPath(curItemPath);
    }
    else
    {
        // 打开该文件
        openFile(curItemPath);
    }
}

void fileListWindow::SlotClickListFileItem(listFileItem *item)
{
    if (!item)
        return;

    tpString curItemPath = item->path();
    tpFileInfo fileInfo(curItemPath);

    if (fileInfo.isDir())
    {
        // 刷新子目录的数据
        refreshPath(curItemPath);
    }
    else
    {
        // 打开该文件
        openFile(curItemPath);
    }
}

void fileListWindow::SlotBackPath()
{
    tpString dealPath = curShowPath_;

    int32_t findIndex = dealPath.rfind("/");
    dealPath = dealPath.mid(0, findIndex);

    refreshPath(dealPath);
}

void fileListWindow::openFile(const tpString &filePath)
{
    // 判断是图片文件
    tpFileInfo fileInfo(filePath);
    if (!fileInfo.exists())
        return;

    tpString fileSuffix = fileInfo.suffix();

    if (fileSuffix.compare("png") == 0 || fileSuffix.compare("jpg") == 0 || fileSuffix.compare("svg") == 0)
    {
        imagePreviewWidget_->setImageFilePath(filePath);
        imagePreviewWidget_->showMaximum();
        return;
    }

    tpSystemApi::OpenFileError openRes = tpSystemApi::Instance()->openFile(filePath);

    if (openRes == tpSystemApi::FileNotExist)
    {
        tpMessageBox::information("文件不存在!");
        return;
    }
    else if (openRes == tpSystemApi::NotSupport)
    {
        tpMessageBox::information("文件类型不支持!");
        return;
    }
    else if (openRes == tpSystemApi::SystemFileDamage)
    {
        tpMessageBox::information("系统文件损坏!");
        return;
    }
    else
    {
    }
}
