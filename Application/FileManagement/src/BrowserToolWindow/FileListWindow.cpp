#include "FileListWindow.h"
#include "SystemInfo/TpDisplay.h"
#include "TpVBoxLayout.h"
#include "TpHBoxLayout.h"
#include "TpLabel.h"
#include "TpFont.h"
#include "TpDir.h"
#include "TpFileInfo.h"
#include "TpMessageBox.h"
#include "TpJsonDocument.h"
#include "TpJsonObject.h"
#include "TpFile.h"
#include "TpNetDataGlobal.h"
#include "TpGateway.h"
#include "TpAppManager.h"

FileListWindow::FileListWindow(TpWidget *parent)
    : TpScrollPanel(parent), rootPath_(""), curShowPath_(""), mode_(FileListWindow::Grid)
{
    init();

    initializeGateway();
}

FileListWindow::~FileListWindow()
{
}

void FileListWindow::setShowMode(const FileListMode &mode)
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

void FileListWindow::setRootPath(const TpString &path)
{
    rootPath_ = path;
    filePathWidget_->setRootPath(rootPath_);
    refreshPath(rootPath_);
}

void FileListWindow::setDeviceType(const DeviceType &deviceType)
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

void FileListWindow::refreshPath(const TpString &path)
{
    // std::cout << "******************************************* " << std::endl;
    // std::cout << "dealPath " << path << std::endl;
    // std::cout << "******************************************* " << std::endl;

    TpDir showDir(path);

    // 文件夹不存在直接不处理
    if (!showDir.exists())
        return;

    // 判断刷新目录是不是根目录
    TpFileInfo rootPath(rootPath_);
    TpFileInfo curPath(path);

    TpString rootCanoicalPath = rootPath.canonicalFilePath();
    TpString curCanoicalPath = curPath.canonicalFilePath();

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
    TpVector<TpObject *> flexChildList = gridFileLayout_->children();
    TpVector<TpObject *> vboxChildList = listFileLayout_->children();

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
    for (const TpFileInfo &fileInfo : showDir.entryInfoList())
    {
        if (mode_ == FileListMode::Grid)
        {
            TpCollapsibleFileItem *item = new TpCollapsibleFileItem();
            // item->setFixedSize(100, 100);
            // item->setBackGroundColor(_RGB(255, 0, 0));
            item->setName(fileInfo.fileName());
            item->setPath(fileInfo.absoluteFilePath());
            item->installEventFilter(this);

            connect(item, onClicked, this, &FileListWindow::SlotClickFileItem);

            gridFileLayout_->addWidget(item);
        }
        else
        {
            ListFileItem *item = new ListFileItem();
            item->setFixedHeight(75);
            // item->setFixedSize(100, 100);
            // item->setBackGroundColor(_RGB(255, 0, 0));
            item->setName(fileInfo.fileName());
            item->setPath(fileInfo.absoluteFilePath());
            item->installEventFilter(this);

            connect(item, onClicked, this, &FileListWindow::SlotClickListFileItem);

            listFileLayout_->addWidget(item);
        }
    }

    TpWidget *gridWidget = dynamic_cast<TpWidget *>(gridFileLayout_->parent());
    TpWidget *listWidget = dynamic_cast<TpWidget *>(listFileLayout_->parent());

    if (gridWidget)
        gridWidget->update();
    if (listWidget)
    {
        listWidget->setMinumumHeight(listFileLayout_->minumumSize().height());
        listWidget->update();
    }

    // gridFileLayout_->update();
    // listFileLayout_->update();

    widget()->setMinumumHeight(mainLayout_->minumumSize().height());
    widget()->setHeight(mainLayout_->minumumSize().height());

    update();
}

void FileListWindow::SlotShowSetting(bool)
{
    settingWindow_->show();
    // settingWindow_->setVisible(true);
    // settingWindow_->update();

    // std::cout << "  settingWindow_  " << settingWindow_->width() << "  " << settingWindow_->height() << std::endl;
}

void FileListWindow::SlotPopUsb(bool)
{
    infoMsgWindow_->setMessageType(TpMessageBox::Information);
    infoMsgWindow_->setText("设备已退出");
    infoMsgWindow_->exec();
}

void FileListWindow::init()
{
    TpWidget *scrollWidget = new TpWidget();
    // scrollWidget->installEventFilter(this);

    imagePreviewWidget_ = new TpImageWidget();

    mainLayout_ = new TpVBoxLayout();
    mainLayout_->setSpacing(13);
    mainLayout_->setContentsMargins(TpDisplay::dp2Px(20), 0, 0, 0);

    TpHBoxLayout *titleLayout = new TpHBoxLayout();
    titleLayout->setSpacing(15);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    gridFileLayout_ = new TpFlexLayout();
    gridFileLayout_->setContentsMargins(0, 0, 0, 0);
    gridFileLayout_->setSpacing(25);
    gridFileLayout_->installEventFilter(scrollWidget);

    listFileLayout_ = new TpVBoxLayout();
    listFileLayout_->setContentsMargins(0, 0, 0, 0);
    listFileLayout_->installEventFilter(scrollWidget);

    TpLabel *titleLabel = new TpLabel("内部存储");
    titleLabel->setAlign(Tp::AlignCenter);
    titleLabel->font()->setFontSize(19);
    titleLabel->setFixedHeight(TpDisplay::dp2Px(30)); // titleLabel->font()->pixelHeight()
    titleLabel->font()->setFontColor(_RGB(38, 38, 38));
    titleLabel->setText("内部存储");
    titleLabel->installEventFilter(scrollWidget);

    pathBackBtn_ = new TpButton();
    pathBackBtn_->setEnableBackGroundColor(false);
    pathBackBtn_->setButtonStyle(TpButton::IconOnly);
    pathBackBtn_->setFixedSize(TpDisplay::dp2Px(30), TpDisplay::dp2Px(30));
    // pathBackBtn_->setFixedSize(titleLabel->font()->pixelHeight(), titleLabel->font()->pixelHeight());
    pathBackBtn_->setIcon(applicationDirPath() + "/../res/路径后退.png");
    // pathBackBtn_->setVisible(false);
    connect(pathBackBtn_, onClicked, [=](bool)
            { SlotBackPath(); });

    popBtn_ = new UsbPopButton();
    connect((TpButton *)popBtn_, onClicked, this, &FileListWindow::SlotPopUsb);

    // 功能设置窗口
    settingWindow_ = new SettingWindow();
    connect(settingWindow_, onChangeViewMode, [=](uint32_t index)
            { setShowMode(FileListMode(index)); });

    settingBtn_ = new TpButton();
    settingBtn_->setButtonStyle(TpButton::IconOnly);
    settingBtn_->setEnableBackGroundColor(false);
    settingBtn_->setFixedSize(TpDisplay::dp2Px(30), TpDisplay::dp2Px(30));
    settingBtn_->setIcon(applicationDirPath() + "/../res/功能设置.png");
    connect(settingBtn_, onClicked, this, &FileListWindow::SlotShowSetting);

    filePathWidget_ = new TpFilePathWidget();
    filePathWidget_->setVisible(false);
    connect(filePathWidget_, onPathChanged, [=](const TpString &lastPath, const TpString &curPath)
            { refreshPath(curPath); });

    titleLayout->addWidget(pathBackBtn_);
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(popBtn_);
    titleLayout->addSpacer(new TpSpacerItem(20, 10, TpSpacerItem::Expanding, TpSpacerItem::Minimum));
    titleLayout->addWidget(settingBtn_);
    titleLayout->installEventFilter(scrollWidget);

    mainLayout_->addLayout(titleLayout);
    mainLayout_->addWidget(filePathWidget_);

    // TODO 测试列表模式
    // mainLayout_->addLayout(listFileLayout_);
    // mode_ = FileListMode::List;
    // 默认显示网格模式
    mainLayout_->addLayout(gridFileLayout_);
    mainLayout_->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Minimum, TpSpacerItem::Expanding));

    scrollWidget->setLayout(mainLayout_);
    // scrollWidget->setBackGroundColor(_RGB(0, 0, 255));

    // setLayout(titleLayout);
    // setLayout(mainLayout_);
    setWidget(scrollWidget);

    infoMsgWindow_ = new TpMessageBox();
}

void FileListWindow::SlotClickFileItem(TpCollapsibleFileItem *item)
{
    if (!item)
        return;

    TpString curItemPath = item->path();
    TpFileInfo fileInfo(curItemPath);

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

void FileListWindow::SlotClickListFileItem(ListFileItem *item)
{
    if (!item)
        return;

    TpString curItemPath = item->path();
    TpFileInfo fileInfo(curItemPath);

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

void FileListWindow::SlotBackPath()
{
    TpString dealPath = curShowPath_;

    int32_t findIndex = dealPath.rfind("/");
    dealPath = dealPath.mid(0, findIndex);

    refreshPath(dealPath);
}

void FileListWindow::openFile(const TpString &filePath)
{
    // 判断是图片文件
    TpFileInfo fileInfo(filePath);
    if (!fileInfo.exists())
        return;

    TpString fileSuffix = fileInfo.suffix();

    if (fileSuffix.compare("png") == 0 || fileSuffix.compare("jpg") == 0 || fileSuffix.compare("svg") == 0)
    {
        imagePreviewWidget_->setImageFilePath(filePath);
        imagePreviewWidget_->showMaximum();
        return;
    }

    TpAppManager::OpenFileError openRes = TpAppManager::Instance()->openFile(filePath);

    infoMsgWindow_->setMessageType(TpMessageBox::Error);
    if (openRes == TpAppManager::FileNotExist)
    {
        infoMsgWindow_->setText("文件不存在!");
        infoMsgWindow_->exec();
        return;
    }
    else if (openRes == TpAppManager::NotSupport)
    {
        infoMsgWindow_->setText("文件类型不支持!");
        infoMsgWindow_->exec();
        return;
    }
    else if (openRes == TpAppManager::SystemFileDamage)
    {
        infoMsgWindow_->setText("系统文件损坏!");
        infoMsgWindow_->exec();
        return;
    }
    else
    {
    }
}
