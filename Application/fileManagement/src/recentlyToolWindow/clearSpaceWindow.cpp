#include "clearSpaceWindow.h"
#include "tpDisplay.h"
#include "tpVBoxLayout.h"
#include "tpHBoxLayout.h"
#include "tpLabel.h"
#include "tpFont.h"
#include "tpLine.h"
#include "tpMessageBox.h"

int32_t layoutMargin = tpDisplay::dp2Px(20);

clearSpaceWindow::clearSpaceWindow() : tpDialog()
{
    setBackGroundColor(_RGB(248, 248, 248));
    // setBackGroundColor(_RGB(248, 0, 0));

    init();
    setVisible(false);
}

clearSpaceWindow::~clearSpaceWindow()
{
}

void clearSpaceWindow::SlotClearSpaceFile(bool)
{
    tpMessageBox::information("已清理垃圾文件");
}

void clearSpaceWindow::init()
{
    // 标题布局
    tpHBoxLayout *titleLayout = new tpHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 12);
    titleLayout->setSpacing(12);

    tpVBoxLayout *mainLayout = new tpVBoxLayout();
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, 0);

    // 清理按钮
    backBtn_ = new tpButton(this);
    backBtn_->setButtonStyle(tpButton::IconOnly);
    backBtn_->setEnableBackGroundColor(false);
    backBtn_->setFixedSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    backBtn_->setIcon(applicationDirPath() + "/../res/路径后退.png");
    connect(backBtn_, onClicked, [=](bool)
            { setVisible(false); });

    tpLabel *titleLabel = new tpLabel("空间清理");
    // titleLabel->setBackGroundColor(_RGB(255, 0, 0));
    titleLabel->font()->setFontSize(19);
    // titleLabel->setFixedHeight(titleLabel->font()->pixelHeight());
    titleLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    titleLabel->setText("空间清理");

    titleLayout->addWidget(backBtn_);
    titleLayout->addWidget(titleLabel);

    tpChildWidget *localSpaceWidget = genLocalSpaceWidget();

    tpChildWidget *clearWidget = genClearFileWidget();

    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(localSpaceWidget);
    mainLayout->addWidget(clearWidget);
    mainLayout->addSpacer(new tpSpacerItem(20, 20, tpSpacerItem::Minimum, tpSpacerItem::Expanding));

    setLayout(mainLayout);
}

tpChildWidget *clearSpaceWindow::genLocalSpaceWidget()
{
    tpHBoxLayout *titleLayout = new tpHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);

    tpVBoxLayout *spaceWidgetLayout = new tpVBoxLayout();
    spaceWidgetLayout->setContentsMargins(18, 35, 18, 35);

    tpChildWidget *localSpaceWidget = new tpChildWidget();
    localSpaceWidget->setBackGroundColor(_RGB(255, 255, 255));
    localSpaceWidget->setRoundCorners(20);

    tpLabel *nameLabel = new tpLabel();
    nameLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    nameLabel->font()->setFontSize(12);
    nameLabel->setText("本地空间");

    tpLabel *spaceLabel = new tpLabel();
    spaceLabel->font()->setFontColor(_RGB(89, 89, 89), _RGB(89, 89, 89));
    spaceLabel->font()->setFontSize(12);
    spaceLabel->setText("已使用 136 GB/256 GB");

    titleLayout->addWidget(nameLabel);
    titleLayout->addSpacer(new tpSpacerItem(20, 20, tpSpacerItem::Expanding));
    titleLayout->addWidget(spaceLabel);

    tpPercentProgressBar *percetProgress = new tpPercentProgressBar();
    // TODO ,查询本地存储各个类别文件存储大小
    percetProgress->setTotalValue(256);
    percetProgress->addItem("图片", 35);
    percetProgress->addItem("音频", 15);
    percetProgress->addItem("视频", 43);
    percetProgress->addItem("文档", 3);
    percetProgress->addItem("压缩包", 4);
    percetProgress->addItem("其他", 20);

    spaceWidgetLayout->addLayout(titleLayout);
    spaceWidgetLayout->addWidget(percetProgress);

    localSpaceWidget->setLayout(spaceWidgetLayout);

    return localSpaceWidget;
}

tpChildWidget *clearSpaceWindow::genClearFileWidget()
{
    tpHBoxLayout *titleLayout = new tpHBoxLayout();
    titleLayout->setContentsMargins(18, 35, 18, 35);

    tpChildWidget *clearWidget = new tpChildWidget();
    clearWidget->setBackGroundColor(_RGB(255, 255, 255));
    clearWidget->setRoundCorners(20);

    tpLabel *nameLabel = new tpLabel();
    nameLabel->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    nameLabel->font()->setFontSize(12);
    nameLabel->setText("垃圾文件");

    tpLine *subLine = new tpLine();
    subLine->setLineType(tpLine::VLine);
    subLine->setLineLength(12);
    subLine->setColor(_RGB(190, 196, 202));
    subLine->setLineWidth(2);

    tpLabel *spaceLabel = new tpLabel();
    spaceLabel->font()->setFontColor(_RGB(140, 140, 140), _RGB(140, 140, 140));
    spaceLabel->font()->setFontSize(12);
    spaceLabel->setText("416MB");

    releaseSpaceBtn_ = new tpButton();
    releaseSpaceBtn_->setProperty("type", "FunctionButton");
    releaseSpaceBtn_->setButtonStyle(tpButton::TextOnly);
    releaseSpaceBtn_->setText("确认并释放空间");
    connect(releaseSpaceBtn_, onClicked, this, clearSpaceWindow::SlotClearSpaceFile);

    titleLayout->addWidget(nameLabel);
    titleLayout->addWidget(subLine);
    titleLayout->addWidget(spaceLabel);
    titleLayout->addSpacer(new tpSpacerItem(20, 20, tpSpacerItem::Expanding));
    titleLayout->addWidget(releaseSpaceBtn_);

    clearWidget->setLayout(titleLayout);

    return clearWidget;
}
