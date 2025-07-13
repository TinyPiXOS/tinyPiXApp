#include "settingWindow.h"
#include "tpVBoxLayout.h"
#include "tpHBoxLayout.h"
#include "tpDisplay.h"
#include "tpEvent.h"
#include "tpFont.h"

#define BG_COLOR _RGBA(0, 0, 0, 153)

settingWindow::settingWindow() : tpDialog()
{
    setBackGroundColor(BG_COLOR);
    init();
    setVisible(false);
}

settingWindow::~settingWindow()
{
}

void settingWindow::setVisible(bool visible)
{
    setSize(tpScreen::screenWidth(), tpScreen::screenHeight());
    mainScrollPanel_->setFixedSize(tpDisplay::dp2Px(429), tpScreen::screenHeight());
    mainScrollPanel_->move(tpScreen::screenWidth() - mainScrollPanel_->width(), 0);

    tpDialog::setVisible(visible);

    if (visible == false)
    {
        // 关闭窗口时，通知外部数据刷新
        onChangeViewMode.emit(viewCbx_->currentIndex());
    }
}

bool settingWindow::onPaintEvent(tpObjectPaintEvent *event)
{
    tpDialog::onPaintEvent(event);
    return true;
}

bool settingWindow::onMouseRleaseEvent(tpMouseEvent *event)
{
    tpDialog::onMouseRleaseEvent(event);

    if (!mainScrollPanel_->toScreen().contains(event->globalPos().x, event->globalPos().y))
    {
        setVisible(false);
        // update();
    }

    return true;
}

void settingWindow::init()
{
    mainScrollPanel_ = new tpScrollPanel(this);
    mainScrollPanel_->setBackGroundColor(_RGB(248, 248, 248));

    tpChildWidget *scrollWidget = new tpChildWidget(mainScrollPanel_);
    scrollWidget->setBackGroundColor(_RGB(248, 248, 248));

    tpLabel *titleLabel = new tpLabel("功能设置");
    titleLabel->font()->setFontSize(19);
    titleLabel->setFixedHeight(titleLabel->font()->pixelHeight());
    titleLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));

    closeBtn_ = new tpButton();
    closeBtn_->setButtonStyle(tpButton::IconOnly);
    closeBtn_->setEnableBackGroundColor(false);
    closeBtn_->setFixedSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    closeBtn_->setIcon(applicationDirPath() + "/../res/关闭.png");
    connect(closeBtn_, onClicked, [=](bool)
            { setVisible(false); });

    tpHBoxLayout *titleLayout = new tpHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    // titleLayout->setContentsMargins(6, 0, 6, 0);
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(closeBtn_);

    tpLabel *viewLabel = createContentLabel("视图");
    viewCbx_ = new tpComBox();
    viewCbx_->setTitle("视图模式");
    viewCbx_->addItem("视图模式");
    viewCbx_->addItem("列表模式");

    tpLabel *filterLabel = createContentLabel("筛选");
    filterCbxGroup_ = new tpListCheckBoxGroup();
    filterCbxGroup_->addItem("文件名称");
    filterCbxGroup_->addItem("文件大小");
    filterCbxGroup_->addItem("修改时间");
    filterCbxGroup_->addItem("文件类型");

    tpLabel *sortLabel = createContentLabel("排序");
    sortCbxGroup_ = new tpListCheckBoxGroup();
    sortCbxGroup_->addItem("正序");
    sortCbxGroup_->addItem("倒序");

    int32_t layoutMargin = tpDisplay::dp2Px(20);

    tpVBoxLayout *mainLayout = new tpVBoxLayout();
    mainLayout->setContentsMargins(18, layoutMargin, 18, 0);
    mainLayout->setSpacing(10);

    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(viewLabel);
    mainLayout->addWidget(viewCbx_);
    mainLayout->addWidget(filterLabel);
    mainLayout->addWidget(filterCbxGroup_);
    mainLayout->addWidget(sortLabel);
    mainLayout->addWidget(sortCbxGroup_);
    mainLayout->addSpacer(new tpSpacerItem(20, 20, tpSpacerItem::Minimum, tpSpacerItem::Expanding));

    scrollWidget->setLayout(mainLayout);
    mainScrollPanel_->setWidget(scrollWidget);
}

tpLabel *settingWindow::createContentLabel(const tpString &text)
{
    tpLabel *viewLabel = new tpLabel(text);
    // viewLabel->setAlign(tpLabel::TP_ALIGN_RIGHT);
    viewLabel->font()->setFontSize(12);
    viewLabel->setFixedHeight(viewLabel->font()->pixelHeight());
    viewLabel->font()->setFontColor(_RGB(89, 89, 89), _RGB(89, 89, 89));
    viewLabel->installEventFilter(mainScrollPanel_);

    return viewLabel;
}
