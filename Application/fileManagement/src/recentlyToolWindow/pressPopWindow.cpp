#include "pressPopWindow.h"
#include "tpHBoxLayout.h"
#include "tpDisplay.h"
#include "tpFont.h"

pressPopWindow::pressPopWindow()
    : tpDialog()
{
    setBackGroundColor(_RGB(248, 248, 248));

    init();
    setVisible(false);
}

pressPopWindow::~pressPopWindow()
{
}

void pressPopWindow::init()
{
    moreConfigMenu_ = new tpMenu();
    moreConfigMenu_->addItem("重命名");
    moreConfigMenu_->addItem("收藏");

    tpString iconButtonCss = "  \
        tpIconTopButton \
        {   \
        icon-size : 34; \
        font-size : 10; \
        color: rgb(38, 38, 38);  \
        }";
    setStyleSheet(iconButtonCss);

    // 选中item后的操作按钮
    delSelectBtn_ = new tpIconTopButton(this);
    // delSelectBtn_->font()->setFontSize(10);
    // delSelectBtn_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    // delSelectBtn_->setIconSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    delSelectBtn_->setText("删除");
    delSelectBtn_->setIcon(applicationDirPath() + "/../res/删除.png");
    connect(delSelectBtn_, onClicked, [=](bool) {});

    moveSelectBtn_ = new tpIconTopButton(this);
    // moveSelectBtn_->font()->setFontSize(10);
    // moveSelectBtn_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    // moveSelectBtn_->setIconSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    moveSelectBtn_->setText("移动");
    moveSelectBtn_->setIcon(applicationDirPath() + "/../res/移动.png");
    connect(moveSelectBtn_, onClicked, [=](bool) {});

    moreSelectBtn_ = new tpIconTopButton(this);
    // moreSelectBtn_->font()->setFontSize(10);
    // moreSelectBtn_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    // moreSelectBtn_->setIconSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    moreSelectBtn_->setText("更多");
    moreSelectBtn_->setIcon(applicationDirPath() + "/../res/更多.png");
    connect(moreSelectBtn_, onClicked, [=](bool)
            { moreConfigMenu_->exec(moreSelectBtn_->toScreen().x, moreSelectBtn_->toScreen().y); });

    buttonList_.emplace_back(delSelectBtn_);
    buttonList_.emplace_back(moveSelectBtn_);
    buttonList_.emplace_back(moreSelectBtn_);
}

bool pressPopWindow::onResizeEvent(tpObjectResizeEvent *event)
{
    tpDialog::onResizeEvent(event);

    if (buttonList_.size() > 0)
    {
        uint32_t singleButonWidth = width() / buttonList_.size();
        uint32_t singleBtnX = (singleButonWidth - tpDisplay::dp2Px(34)) / 2.0;

        for (int i = 0; i < buttonList_.size(); ++i)
        {
            tpIconTopButton *curBtn = buttonList_.at(i);

            uint32_t singleBtnY = (height() - curBtn->height()) / 2.0;
            curBtn->move(i * singleButonWidth + singleBtnX, singleBtnY);
        }
    }

    return true;
}

bool pressPopWindow::eventFilter(tpObject *watched, tpEvent *event)
{
    return false;
}
