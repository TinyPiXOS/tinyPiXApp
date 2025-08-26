#include "settingBase.h"

settingBase::settingBase(TpChildWidget *parent)
    : TpChildWidget(parent), titleLabel_(nullptr), backBtn_(nullptr)
{
    mainLayout_ = new TpVBoxLayout();
    mainLayout_->setSpacing(10);
}

settingBase::~settingBase()
{
}

void settingBase::setTitleBackInfo(TpLabel *titleLabel, TpButton *backBtn)
{
    titleLabel_ = titleLabel;
    backBtn_ = backBtn;
}

void settingBase::setSubTile(const TpString &subTitle)
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
