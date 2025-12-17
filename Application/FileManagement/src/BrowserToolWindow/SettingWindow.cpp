#include "SettingWindow.h"
#include "TpVBoxLayout.h"
#include "TpHBoxLayout.h"
#include "SystemInfo/TpDisplay.h"
#include "TpEvent.h"
#include "TpFont.h"

#define BG_COLOR _RGBA(0, 0, 0, 153)

SettingWindow::SettingWindow() : TpDialog()
{
    setBackGroundColor(BG_COLOR);
    init();
    // setVisible(false);
}

SettingWindow::~SettingWindow()
{
}

void SettingWindow::setVisible(bool visible)
{
    TpSize screenSize = this->screenSize();
    setSize(screenSize.width(), screenSize.height());
    mainScrollPanel_->setFixedSize(TpDisplay::dp2Px(429), screenSize.height());
    mainScrollPanel_->move(screenSize.width() - mainScrollPanel_->width(), 0);

    TpDialog::setVisible(visible);

    if (visible == false)
    {
        // 关闭窗口时，通知外部数据刷新
        onChangeViewMode.emit(viewCbx_->currentIndex());
    }
}

bool SettingWindow::onMouseRleaseEvent(TpMouseEvent *event)
{
    TpDialog::onMouseRleaseEvent(event);

    if (!mainScrollPanel_->toScreen().contains(event->globalPos().x(), event->globalPos().y()))
    {
        setVisible(false);
        // update();
    }

    return true;
}

void SettingWindow::init()
{
    mainScrollPanel_ = new TpScrollPanel(this);
    mainScrollPanel_->setBackGroundColor(_RGB(248, 248, 248));

    TpWidget *scrollWidget = new TpWidget(mainScrollPanel_);
    scrollWidget->setBackGroundColor(_RGB(248, 248, 248));

    TpLabel *titleLabel = new TpLabel("功能设置");
    titleLabel->font()->setFontSize(19);
    titleLabel->setFixedHeight(titleLabel->font()->pixelHeight());
    titleLabel->font()->setFontColor(_RGB(38, 38, 38));

    closeBtn_ = new TpButton();
    closeBtn_->setButtonStyle(TpButton::IconOnly);
    closeBtn_->setEnableBackGroundColor(false);
    closeBtn_->setFixedSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    closeBtn_->setIcon(applicationDirPath() + "/../res/关闭.png");
    connect(closeBtn_, onClicked, [=](bool)
            { setVisible(false); });

    TpHBoxLayout *titleLayout = new TpHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    // titleLayout->setContentsMargins(6, 0, 6, 0);
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(closeBtn_);

    TpLabel *viewLabel = createContentLabel("视图");
    viewCbx_ = new TpComBox();
    viewCbx_->setTitle("视图模式");
    viewCbx_->addItem("视图模式");
    viewCbx_->addItem("列表模式");

    TpLabel *filterLabel = createContentLabel("筛选");
    filterCbxGroup_ = new TpListWidget();
    filterCbxGroup_->addItem("文件名称");
    filterCbxGroup_->addItem("文件大小");
    filterCbxGroup_->addItem("修改时间");
    filterCbxGroup_->addItem("文件类型");

    TpLabel *sortLabel = createContentLabel("排序");
    sortCbxGroup_ = new TpListWidget();
    sortCbxGroup_->addItem("正序");
    sortCbxGroup_->addItem("倒序");

    int32_t layoutMargin = TpDisplay::dp2Px(20);

    TpVBoxLayout *mainLayout = new TpVBoxLayout();
    mainLayout->setContentsMargins(18, layoutMargin, 18, 0);
    mainLayout->setSpacing(10);

    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(viewLabel);
    mainLayout->addWidget(viewCbx_);
    mainLayout->addWidget(filterLabel);
    mainLayout->addWidget(filterCbxGroup_);
    mainLayout->addWidget(sortLabel);
    mainLayout->addWidget(sortCbxGroup_);
    mainLayout->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Minimum, TpSpacerItem::Expanding));

    scrollWidget->setLayout(mainLayout);
    mainScrollPanel_->setWidget(scrollWidget);
}

TpLabel *SettingWindow::createContentLabel(const TpString &text)
{
    TpLabel *viewLabel = new TpLabel(text);
    // viewLabel->setAlign(TpLabel::TP_ALIGN_RIGHT);
    viewLabel->font()->setFontSize(12);
    viewLabel->setFixedHeight(viewLabel->font()->pixelHeight());
    viewLabel->font()->setFontColor(_RGB(89, 89, 89));
    viewLabel->installEventFilter(mainScrollPanel_);

    return viewLabel;
}
