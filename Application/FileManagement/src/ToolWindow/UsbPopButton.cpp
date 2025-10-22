#include "UsbPopButton.h"
#include "TpEvent.h"
#include "TpHBoxLayout.h"
#include "TpFont.h"
#include "TpDisplay.h"
#include "TpImage.h"

UsbPopButton::UsbPopButton(TpChildWidget *parent)
    : TpButton(parent)
{
    init();
}

UsbPopButton::~UsbPopButton()
{
}

void UsbPopButton::init()
{
    nameLabel_ = new TpLabel();
    nameLabel_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    nameLabel_->font()->setFontSize(12);
    nameLabel_->setText("安全弹出");
    nameLabel_->installEventFilter(this);

    iconLabel_ = new TpLabel();
    iconLabel_->setFixedSize(TpDisplay::dp2Px(23), TpDisplay::dp2Px(23));
    iconLabel_->installEventFilter(this);

    iconLabel_->setBackGroundImage(TpImage(applicationDirPath() + "/../res/箭头-未展开.png"));

    TpHBoxLayout* mainLayout = new TpHBoxLayout();
    mainLayout->addWidget(nameLabel_);
    mainLayout->addWidget(iconLabel_);

    setLayout(mainLayout);
}