#include "mainWindowService.h"
#include "tpSignalSlot.h"
#include "fileManagementGlobal.h"
#include "tpVBoxLayout.h"
#include "tpTabBar.h"

enum TreeNodeType
{
    MainPageType,
    PicturesType,
    MusicType,
    VideoType,
    DownloadType,
    HardDiskType,
    RecycleType
};

static const uint32_t TreeNodeDataTypeRole = 0;

mainWindowService::mainWindowService()
    : tpFixScreen()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    initUi();

    setBackGroundColor(_RGB(248, 248, 248));
    setVisible(true);
}

mainWindowService::~mainWindowService()
{
}

bool mainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "mainWindowService::appChange" << std::endl;

    // caculateRect();

    return true;
}

bool mainWindowService::onResizeEvent(tpObjectResizeEvent *event)
{
    std::cout << "mainWindowService::onResizeEvent" << std::endl;

    caculateRect();

    return true;
}

bool mainWindowService::onRotateEvent(tpObjectRotateEvent *event)
{
    std::cout << "mainWindowService::onRotateEvent" << std::endl;

    caculateRect();

    return true;
}

bool mainWindowService::onActiveEvent(tpObjectActiveEvent *event)
{
    std::cout << "mainWindowService::onActiveEvent" << std::endl;

    caculateRect();

    return true;
}

void mainWindowService::initUi()
{
    tpString resPath = applicationDirPath() + "/../res/";

    recentlyWindow_ = new recentlyWindow();
    // recentlyWindow_->setVisible(true);
    // recentlyWindow_->setBackGroundColor(_RGB(255, 0, 0));

    browseWindow_ = new browseWindow();
    // recentlyWindow_->setVisible(false);

    mainTabWidget_ = new tpTabWidget(this);
    mainTabWidget_->addTab(recentlyWindow_, "最近");
    mainTabWidget_->addTab(browseWindow_, "浏览");

    tpTabBar *tabBar = mainTabWidget_->tabBar();
    tabBar->setIcon(0, applicationDirPath() + "/../res/最近-normal.png", applicationDirPath() + "/../res/最近-focus.png");
    tabBar->setIcon(1, applicationDirPath() + "/../res/浏览-normal.png", applicationDirPath() + "/../res/浏览-focus.png");

    tpVBoxLayout *mainLayout = new tpVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(mainTabWidget_);

    setLayout(mainLayout);
}

void mainWindowService::slotPathChanged(const tpString &path)
{
    tpString changePath = "";
    if (path.startsWith(RootPath))
    {
        changePath = RootPath;
    }
    else if (path.startsWith(MusicPath))
    {
        changePath = MusicPath;
    }
    else if (path.startsWith(VideoPath))
    {
        changePath = VideoPath;
    }
    else if (path.startsWith(PicturesPath))
    {
        changePath = PicturesPath;
    }
    else if (path.startsWith(DownloadPath))
    {
        changePath = DownloadPath;
    }
    else if (path.startsWith(RecycleTypePath))
    {
        changePath = RecycleTypePath;
    }
    else
    {
        // 还要检测属于哪个磁盘的文件，切换到对应磁盘树节点 TODO
        changePath = "";
    }

    // if (fileTreeItemMap_.contains(RootPath))
    // fileTreeItemMap_.value(RootPath)->setSelected(true);
}

void mainWindowService::caculateRect()
{
    ItpRect mainWindowRect = rect();

    // bottomMenu_->move(0, height() - bottomMenu_->height());

    // fileTreeWidget_->setRect(0, 0, 210, mainWindowRect.h);

    // titleBar_->setRect(fileTreeWidget_->width(), 0, mainWindowRect.w - fileTreeWidget_->width(), 72);

    // mainPageWindow_->setRect(fileTreeWidget_->width(), titleBar_->height(), mainWindowRect.w - fileTreeWidget_->width(), mainWindowRect.h - titleBar_->height());
    // fileListWindow_->setRect(fileTreeWidget_->width(), titleBar_->height(), mainWindowRect.w - fileTreeWidget_->width(), mainWindowRect.h - titleBar_->height());

    // update();
}