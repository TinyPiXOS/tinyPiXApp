#include "settingBase.h"

settingBase::settingBase(tpChildWidget *parent)
    : tpChildWidget(parent), titleLabel_(nullptr), backBtn_(nullptr)
{
    mainLayout_ = new tpVBoxLayout();
    mainLayout_->setSpacing(10);
}

settingBase::~settingBase()
{
}

void settingBase::setTitleBackInfo(tpLabel *titleLabel, tpButton *backBtn)
{
    titleLabel_ = titleLabel;
    backBtn_ = backBtn;
}

void settingBase::setSubTile(const tpString &subTitle)
{
    if (!titleLabel_)
        return;
    titleLabel_->setText(subTitle);
}

void settingBase::setBackBtnVisible(const bool &visible)
{
    if (!backBtn_)
        return;
    backBtn_->setVisible(visible);
}

void settingBase::slotBack()
{
}
