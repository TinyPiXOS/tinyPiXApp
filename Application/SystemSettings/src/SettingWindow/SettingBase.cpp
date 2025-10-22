#include "SettingBase.h"

SettingBase::SettingBase(TpChildWidget *parent)
    : TpChildWidget(parent), titleLabel_(nullptr), backBtn_(nullptr)
{
    mainLayout_ = new TpVBoxLayout();
    mainLayout_->setSpacing(10);
}

SettingBase::~SettingBase()
{
}

void SettingBase::setTitleBackInfo(TpLabel *titleLabel, TpButton *backBtn)
{
    titleLabel_ = titleLabel;
    backBtn_ = backBtn;
}

void SettingBase::setSubTile(const TpString &subTitle)
{
    if (!titleLabel_)
        return;
    titleLabel_->setText(subTitle);
}

void SettingBase::setBackBtnVisible(const bool &visible)
{
    if (!backBtn_)
        return;
    backBtn_->setVisible(visible);
}

void SettingBase::slotBack()
{
}
