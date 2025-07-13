#include "diskDeviceCheckBox.h"
#include "tpHBoxLayout.h"
#include "tpDisplay.h"
#include "tpFont.h"
#include "tpCanvas.h"
#include "tpLine.h"
#include "tpEvent.h"

diskDeviceCheckBox::diskDeviceCheckBox(tpChildWidget *parent)
    : tpChildWidget(parent)
{
    refreshBaseCss();

    init();
}

diskDeviceCheckBox::~diskDeviceCheckBox()
{
}

void diskDeviceCheckBox::setIcon(const tpString &normalIcon, const tpString &checkedIcon)
{
    if (!normalIcon.empty())
        normalIcon_->fromFile(normalIcon);

    if (!checkedIcon.empty())
        checkedIcon_->fromFile(checkedIcon);
}

void diskDeviceCheckBox::setName(const tpString &name)
{
    deviceNameLabel_->setText(name);
}

void diskDeviceCheckBox::setSpace(const float &curSpace, const float &totalSpace)
{
    tpString spaceText = tpString::number(curSpace, 2) + "GB/" + tpString::number(totalSpace, 2) + "GB";
    spaceLabel_->setText(spaceText);
}

bool diskDeviceCheckBox::onMouseRleaseEvent(tpMouseEvent *event)
{
    tpChildWidget::onMouseRleaseEvent(event);

    onClicked.emit(this);

    return true;
}

bool diskDeviceCheckBox::onResizeEvent(tpObjectResizeEvent *event)
{
    tpChildWidget::onResizeEvent(event);
    return true;
}

bool diskDeviceCheckBox::onPaintEvent(tpObjectPaintEvent *event)
{
    tpChildWidget::onPaintEvent(event);

    if (checked())
    {
        iconLabel_->setBackGroundImage(checkedIcon_);
    }
    else
    {
        iconLabel_->setBackGroundImage(normalIcon_);
    }

    // tpCanvas *paintCanvas = event->canvas();

    // 在name和space中间画一个竖线
    // std::cout << "spaceLabel_->pos().x " << spaceLabel_->pos().x << std::endl;
    // std::cout << "deviceNameLabel_->pos().x " << deviceNameLabel_->pos().x << std::endl;
    // std::cout << "deviceNameLabel_->width() " << deviceNameLabel_->width() << std::endl;

    // uint32_t deviceNameWidth = deviceNameLabel_->width() > deviceNameLabel_->font()->pixelWidth() ? deviceNameLabel_->font()->pixelWidth() : deviceNameLabel_->width();
    // uint32_t lineX = deviceNameLabel_->pos().x + deviceNameWidth + (spaceLabel_->pos().x - (deviceNameLabel_->pos().x + deviceNameWidth)) / 2.0;
    // paintCanvas->vline(lineX, 25, height() - 25, _RGB(190, 196, 202), 2);

    return true;
}

bool diskDeviceCheckBox::eventFilter(tpObject *watched, tpEvent *event)
{
    if (event->eventType() == tpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMousePressEvent(mouseEvent);
    }
    else if (event->eventType() == tpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMouseRleaseEvent(mouseEvent);
    }
    else
    {
    }

    return false;
}

void diskDeviceCheckBox::init()
{
    setCheckable(true);

    tpShared<tpCssData> cssData = currentStatusCss();

    iconLabel_ = new tpLabel(this);
    iconLabel_->setFixedSize(tpDisplay::dp2Px(33), tpDisplay::dp2Px(33));

    deviceNameLabel_ = new tpLabel(this);
    deviceNameLabel_->font()->setFontColor(cssData->color(), cssData->color());
    deviceNameLabel_->font()->setFontSize(cssData->fontSize());

    tpLine *subLine = new tpLine();
    subLine->setLineType(tpLine::VLine);
    subLine->setLineLength(18);
    subLine->setColor(_RGB(190, 196, 202));
    subLine->setLineWidth(2);

    spaceLabel_ = new tpLabel(this);
    spaceLabel_->font()->setFontColor(_RGB(140, 140, 140), _RGB(140, 140, 140));
    spaceLabel_->font()->setFontSize(cssData->fontSize());

    expandLabel_ = new tpLabel(this);
    expandLabel_->setFixedSize(tpDisplay::dp2Px(23), tpDisplay::dp2Px(23));

    tpShared<tpSurface> expandIcon = tpMakeShared<tpSurface>();
    expandIcon->fromFile(applicationDirPath() + "/../res/箭头-未展开.png");
    expandLabel_->setBackGroundImage(expandIcon);

    normalIcon_ = tpMakeShared<tpSurface>();
    checkedIcon_ = tpMakeShared<tpSurface>();

    tpShared<tpCssData> curCssData = currentStatusCss();

    tpHBoxLayout *mainLayout = new tpHBoxLayout();
    mainLayout->setContentsMargins(curCssData->paddingLeft(), curCssData->paddingTop(), curCssData->paddingRight(), curCssData->paddingBottom());
    mainLayout->setSpacing(10);

    iconLabel_->installEventFilter(this);
    deviceNameLabel_->installEventFilter(this);
    spaceLabel_->installEventFilter(this);
    expandLabel_->installEventFilter(this);

    mainLayout->addWidget(iconLabel_);
    mainLayout->addWidget(deviceNameLabel_);
    mainLayout->addWidget(subLine);
    mainLayout->addWidget(spaceLabel_);
    mainLayout->addSpacer(new tpSpacerItem(20, 40, tpSpacerItem::Expanding, tpSpacerItem::Minimum));
    mainLayout->addWidget(expandLabel_);
    setLayout(mainLayout);
}
