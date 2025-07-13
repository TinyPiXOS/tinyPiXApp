#include "usbPopButton.h"
#include "tpEvent.h"
#include "tpHBoxLayout.h"
#include "tpFont.h"
#include "tpDisplay.h"
#include "tpSurface.h"

usbPopButton::usbPopButton(tpChildWidget *parent)
    : tpButton(parent)
{
    init();
}

usbPopButton::~usbPopButton()
{
}

void usbPopButton::init()
{
    nameLabel_ = new tpLabel();
    nameLabel_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    nameLabel_->font()->setFontSize(12);
    nameLabel_->setText("安全弹出");
    nameLabel_->installEventFilter(this);

    iconLabel_ = new tpLabel();
    iconLabel_->setFixedSize(tpDisplay::dp2Px(23), tpDisplay::dp2Px(23));
    iconLabel_->installEventFilter(this);

    tpShared<tpSurface> expandIcon = tpMakeShared<tpSurface>();
    expandIcon->fromFile(applicationDirPath() + "/../res/箭头-未展开.png");
    iconLabel_->setBackGroundImage(expandIcon);

    tpHBoxLayout* mainLayout = new tpHBoxLayout();
    mainLayout->addWidget(nameLabel_);
    mainLayout->addWidget(iconLabel_);

    setLayout(mainLayout);
}