#include "recentlyWindow.h"
#include "TpVBoxLayout.h"
#include "TpHBoxLayout.h"
#include "TpLabel.h"
#include "TpFont.h"
#include "TpDisplay.h"
#include "TpButton.h"

recentlyWindow::recentlyWindow(TpChildWidget *parent)
    : TpChildWidget(parent)
{
    init();

    setBackGroundColor(_RGB(248, 248, 248));
    // setBackGroundColor(_RGB(255, 0, 0));
}

recentlyWindow::~recentlyWindow()
{
}

bool recentlyWindow::onMousePressEvent(TpMouseEvent *event)
{
    TpChildWidget::onMousePressEvent(event);

    pressPopMenu_->setVisible(false);

    for (const auto &fileWidget : fileWidgetList_)
    {
        fileWidget->setSelectMode(TpCollapsibleFileWidget::Normal);
    }

    return true;
}

bool recentlyWindow::onMouseRleaseEvent(TpMouseEvent *event)
{
    TpChildWidget::onMouseRleaseEvent(event);

    return true;
}

bool recentlyWindow::eventFilter(TpObject *watched, TpEvent *event)
{
    if (event->eventType() == TpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        TpMouseEvent *mouseEvent = dynamic_cast<TpMouseEvent *>(event);
        onMousePressEvent(mouseEvent);
    }
    
    return false;
}

void recentlyWindow::SlotLongPressItem(TpCollapsibleFileWidget *fileWidget)
{
    ItpSize popMenuSize = pressPopMenu_->screenSize();

    pressPopMenu_->setRect(0, popMenuSize.h - 75, popMenuSize.w, 75);

    pressPopMenu_->setVisible(true);

    // 所有当前item添加cbx
    fileWidget->setSelectMode(TpCollapsibleFileWidget::MultiSelection);
}

void recentlyWindow::init()
{
    TpVBoxLayout *mainLayout = new TpVBoxLayout();

    // 初始化长按弹出窗口
    pressPopMenu_ = new pressPopWindow();

    uint32_t layoutMargin = TpDisplay::dp2Px(25);
    mainLayout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, 0);

    TpLabel *titleLabel = new TpLabel("最近", this);
    titleLabel->font()->setFontSize(19);
    titleLabel->setFixedHeight(titleLabel->font()->pixelHeight());
    titleLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    // titleLabel->setBackGroundColor(_RGB(0, 255, 0));
    titleLabel->installEventFilter(this);

    // 清理窗口
    clearWindow_ = new clearSpaceWindow();

    // 清理按钮
    clearButton_ = new TpButton(this);
    clearButton_->setButtonStyle(TpButton::IconOnly);
    clearButton_->setEnableBackGroundColor(false);
    clearButton_->setFixedSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    clearButton_->setIcon(applicationDirPath() + "/../res/清理.png");
    connect(clearButton_, onClicked, [=](bool)
            { clearWindow_->showMaximum(); });
    clearButton_->installEventFilter(this);

    TpHBoxLayout *titleLayout = new TpHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->addWidget(titleLabel, 10);
    titleLayout->addWidget(clearButton_, 1);

    mainScrollPanel_ = new TpScrollPanel(this);
    mainScrollPanel_->installEventFilter(this);
    // mainScrollPanel_->setBackGroundColor(_RGB(0, 0, 255));

    TpChildWidget *scrollWidget = new TpChildWidget(mainScrollPanel_);
    TpVBoxLayout *scrollLayout = new TpVBoxLayout();
    scrollWidget->setSize(600, 1500);
    scrollWidget->installEventFilter(this);

    // 测试数据
    // diskDeviceCheckBox *testDevice = new diskDeviceCheckBox();
    // testDevice->setIcon(applicationDirPath() + "/../res/USB设备-未选中.png", applicationDirPath() + "/../res/USB设备-选中.png");
    // testDevice->setName("USB设备");
    // testDevice->setSpace(100.12, 250);
    // testDevice->installEventFilter(scrollWidget);

    // deviceList_.emplace_back(testDevice);

    searchEdit_ = new TpLineEdit();
    searchEdit_->setProperty("type", "fileManageEdit");
    searchEdit_->setIcon(applicationDirPath() + "/../res/搜索.png");
    searchEdit_->setPlaceholderText("搜索文件");
    searchEdit_->installEventFilter(this);

    scrollLayout->addWidget(searchEdit_);
    // scrollLayout->addWidget(testDevice);

    // 创建最近访问文件菜单，内部会对scrollLayout布局末尾追加widget
    createRecentlyFileList(scrollLayout);
    createRecentlyFileList(scrollLayout);

    scrollLayout->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Minimum, TpSpacerItem::Expanding));

    // 将布局设置到滚动widget
    scrollWidget->setLayout(scrollLayout);

    mainScrollPanel_->setWidget(scrollWidget);

    mainLayout->addLayout(titleLayout, 1);
    mainLayout->addWidget(mainScrollPanel_, 20);

    setLayout(mainLayout);
}

void recentlyWindow::createRecentlyFileList(TpVBoxLayout *scrollLayout)
{
    TpCollapsibleFileWidget *testFileWidget = new TpCollapsibleFileWidget();
    testFileWidget->setTitle("今天");
    testFileWidget->setSubTitle("2月14日");

    connect(testFileWidget, onLongPress, [=]()
            { SlotLongPressItem(testFileWidget); });

    for (int i = 0; i < 10; ++i)
    {
        TpCollapsibleFileItem *item = new TpCollapsibleFileItem();
        // item->setFixedSize(100, 100);
        // item->setBackGroundColor(_RGB(255, 0, 0));
        item->setName(TpString::number(i) + ".png");
        item->setPath("/root/" + TpString::number(i) + ".png");

        testFileWidget->addFileItem(item);
    }

    scrollLayout->addWidget(testFileWidget);
    fileWidgetList_.emplace_back(testFileWidget);
}
