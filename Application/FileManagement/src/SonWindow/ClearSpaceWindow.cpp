#include "ClearSpaceWindow.h"
#include "SystemInfo/TpDisplay.h"
#include "TpVBoxLayout.h"
#include "TpHBoxLayout.h"
#include "TpLabel.h"
#include "TpFont.h"
#include "TpLine.h"
#include "TpMessageBox.h"

int32_t layoutMargin = TpDisplay::dp2Px(20);

ClearSpaceWindow::ClearSpaceWindow() : TpDesktopDialog()
{
    setBackGroundColor(_RGB(248, 248, 248));
    // setBackGroundColor(_RGB(248, 0, 0));

    init();
    setVisible(false);
}

ClearSpaceWindow::~ClearSpaceWindow()
{
    infoMsgWin_->deleteLater();
}

void ClearSpaceWindow::SlotClearSpaceFile(bool)
{
    infoMsgWin_->setMessageType(TpMessageBox::Information);
    infoMsgWin_->setText("已清理垃圾文件");
}

void ClearSpaceWindow::init()
{
    infoMsgWin_ = new TpMessageBox();

    // 标题布局
    TpHBoxLayout *titleLayout = new TpHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 12);
    titleLayout->setSpacing(12);

    TpVBoxLayout *mainLayout = new TpVBoxLayout();
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, 0);

    // 清理按钮
    backBtn_ = new TpButton(this);
    backBtn_->setButtonStyle(TpButton::IconOnly);
    backBtn_->setEnableBackGroundColor(false);
    backBtn_->setFixedSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    backBtn_->setIcon(applicationDirPath() + "/../res/路径后退.png");
    connect(backBtn_, onClicked, [=](bool)
            { std::cout << "backBtn_ " << std::endl;setVisible(false); });

    // TpLabel *titleLabel = new TpLabel("空间清理");
    // titleLabel->font()->setFontSize(19);
    // titleLabel->font()->setAlign(Tp::AlignVCenter);
    // titleLabel->font()->setFontColor(_RGB(38, 38, 38));
    // titleLabel->setText("空间清理");

    titleLayout->addWidget(backBtn_);
    // titleLayout->addWidget(titleLabel);

    // TpWidget *localSpaceWidget = genLocalSpaceWidget();

    // TpWidget *clearWidget = genClearFileWidget();

    mainLayout->addLayout(titleLayout);
    // mainLayout->addWidget(localSpaceWidget);
    // mainLayout->addWidget(clearWidget);
    // mainLayout->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Minimum, TpSpacerItem::Expanding));

    setLayout(mainLayout);
}

TpWidget *ClearSpaceWindow::genLocalSpaceWidget()
{
    TpHBoxLayout *titleLayout = new TpHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);

    TpVBoxLayout *spaceWidgetLayout = new TpVBoxLayout();
    spaceWidgetLayout->setContentsMargins(18, 35, 18, 35);

    TpWidget *localSpaceWidget = new TpWidget();
    localSpaceWidget->setBackGroundColor(_RGB(255, 255, 255));
    localSpaceWidget->setRoundCorners(20);

    TpLabel *nameLabel = new TpLabel();
    nameLabel->font()->setFontColor(_RGB(38, 38, 38));
    nameLabel->font()->setFontSize(12);
    nameLabel->setText("本地空间");

    TpLabel *spaceLabel = new TpLabel();
    spaceLabel->font()->setFontColor(_RGB(89, 89, 89));
    spaceLabel->font()->setFontSize(12);
    spaceLabel->setText("已使用 136 GB/256 GB");

    titleLayout->addWidget(nameLabel);
    titleLayout->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Expanding));
    titleLayout->addWidget(spaceLabel);

    TpPercentProgressBar *percetProgress = new TpPercentProgressBar();
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

TpWidget *ClearSpaceWindow::genClearFileWidget()
{
    TpHBoxLayout *titleLayout = new TpHBoxLayout();
    titleLayout->setContentsMargins(18, 35, 18, 35);

    TpWidget *clearWidget = new TpWidget();
    clearWidget->setBackGroundColor(_RGB(255, 255, 255));
    clearWidget->setRoundCorners(20);

    TpLabel *nameLabel = new TpLabel();
    nameLabel->font()->setFontColor(_RGB(38, 38, 38));
    nameLabel->font()->setFontSize(12);
    nameLabel->setAlign(Tp::AlignCenter);
    nameLabel->setText("垃圾文件");
    nameLabel->setMinumumWidth(nameLabel->font()->pixelWidth() + 10);

    TpLine *subLine = new TpLine();
    subLine->setLineType(TpLine::VLine);
    subLine->setLineLength(12);
    subLine->setColor(_RGB(190, 196, 202));
    subLine->setLineWidth(2);

    TpLabel *spaceLabel = new TpLabel();
    spaceLabel->font()->setFontColor(_RGB(140, 140, 140));
    spaceLabel->font()->setFontSize(12);
    spaceLabel->setAlign(Tp::AlignCenter);
    spaceLabel->setText("456MB");
    spaceLabel->setMinumumWidth(nameLabel->font()->pixelWidth() + 10);

    releaseSpaceBtn_ = new TpButton();
    releaseSpaceBtn_->setProperty("type", "FunctionButton");
    releaseSpaceBtn_->setButtonStyle(TpButton::TextOnly);
    releaseSpaceBtn_->setText("确认并释放空间");
    connect(releaseSpaceBtn_, onClicked, this, &ClearSpaceWindow::SlotClearSpaceFile);

    titleLayout->addWidget(nameLabel);
    titleLayout->addWidget(subLine);
    titleLayout->addWidget(spaceLabel);
    titleLayout->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Expanding));
    titleLayout->addWidget(releaseSpaceBtn_);

    clearWidget->setLayout(titleLayout);

    return clearWidget;
}
