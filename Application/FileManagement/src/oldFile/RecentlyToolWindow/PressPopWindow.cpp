#include "PressPopWindow.h"
#include "TpHBoxLayout.h"
#include "SystemInfo/TpDisplay.h"
#include "TpFont.h"

PressPopWindow::PressPopWindow()
    : TpDesktopDialog()
{
    setBackGroundColor(_RGB(248, 248, 248));

    init();
    setVisible(false);
}

PressPopWindow::~PressPopWindow()
{
}

void PressPopWindow::init()
{
    moreConfigMenu_ = new TpMenu();
    moreConfigMenu_->addItem("重命名");
    moreConfigMenu_->addItem("收藏");

    TpString iconButtonCss = "  \
        TpIconTopButton \
        {   \
        icon-size : 34; \
        font-size : 10; \
        color: rgb(38, 38, 38);  \
        }";
    setStyleSheet(iconButtonCss);

    // 选中item后的操作按钮
    delSelectBtn_ = new TpIconTopButton(this);
    // delSelectBtn_->font()->setFontSize(10);
    // delSelectBtn_->font()->setFontColor(_RGB(38, 38, 38));
    // delSelectBtn_->setIconSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    delSelectBtn_->setText("删除");
    delSelectBtn_->setIcon(applicationDirPath() + "/../res/删除.png");
    connect(delSelectBtn_, onClicked, [=](bool) {});

    moveSelectBtn_ = new TpIconTopButton(this);
    // moveSelectBtn_->font()->setFontSize(10);
    // moveSelectBtn_->font()->setFontColor(_RGB(38, 38, 38));
    // moveSelectBtn_->setIconSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    moveSelectBtn_->setText("移动");
    moveSelectBtn_->setIcon(applicationDirPath() + "/../res/移动.png");
    connect(moveSelectBtn_, onClicked, [=](bool) {});

    moreSelectBtn_ = new TpIconTopButton(this);
    // moreSelectBtn_->font()->setFontSize(10);
    // moreSelectBtn_->font()->setFontColor(_RGB(38, 38, 38));
    // moreSelectBtn_->setIconSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    moreSelectBtn_->setText("更多");
    moreSelectBtn_->setIcon(applicationDirPath() + "/../res/更多.png");
    connect(moreSelectBtn_, onClicked, [=](bool)
            { moreConfigMenu_->exec(moreSelectBtn_->toScreen().x(), moreSelectBtn_->toScreen().y()); });

    buttonList_.emplace_back(delSelectBtn_);
    buttonList_.emplace_back(moveSelectBtn_);
    buttonList_.emplace_back(moreSelectBtn_);
}

bool PressPopWindow::onResizeEvent(TpResizeEvent *event)
{
    TpDesktopDialog::onResizeEvent(event);

    if (buttonList_.size() > 0)
    {
        uint32_t singleButonWidth = width() / buttonList_.size();
        uint32_t singleBtnX = (singleButonWidth - TpDisplay::dp2Px(34)) / 2.0;

        for (int i = 0; i < buttonList_.size(); ++i)
        {
            TpIconTopButton *curBtn = buttonList_.at(i);

            uint32_t singleBtnY = (height() - curBtn->height()) / 2.0;
            curBtn->move(i * singleButonWidth + singleBtnX, singleBtnY);
        }
    }

    return true;
}

bool PressPopWindow::eventFilter(TpObject *watched, TpEvent *event)
{
    return false;
}
