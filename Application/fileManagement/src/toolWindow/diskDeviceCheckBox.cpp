#include "diskDeviceCheckBox.h"
#include "TpHBoxLayout.h"
#include "TpDisplay.h"
#include "TpFont.h"
#include "TpCanvas.h"
#include "TpLine.h"
#include "TpEvent.h"

diskDeviceCheckBox::diskDeviceCheckBox(TpChildWidget *parent)
    : TpChildWidget(parent)
{
    refreshBaseCss();

    init();
}

diskDeviceCheckBox::~diskDeviceCheckBox()
{
}

void diskDeviceCheckBox::setIcon(const TpString &normalIcon, const TpString &checkedIcon)
{
    if (!normalIcon.empty())
        normalIcon_.load(normalIcon);

    if (!checkedIcon.empty())
        checkedIcon_.load(checkedIcon);
}

void diskDeviceCheckBox::setName(const TpString &name)
{
    deviceNameLabel_->setText(name);
}

void diskDeviceCheckBox::setSpace(const float &curSpace, const float &totalSpace)
{
    TpString spaceText = TpString::number(curSpace, 2) + "GB/" + TpString::number(totalSpace, 2) + "GB";
    spaceLabel_->setText(spaceText);
}

bool diskDeviceCheckBox::onMouseRleaseEvent(TpMouseEvent *event)
{
    TpChildWidget::onMouseRleaseEvent(event);

    onClicked.emit(this);

    return true;
}

bool diskDeviceCheckBox::onResizeEvent(TpObjectResizeEvent *event)
{
    TpChildWidget::onResizeEvent(event);
    return true;
}

bool diskDeviceCheckBox::onPaintEvent(TpObjectPaintEvent *event)
{
    TpChildWidget::onPaintEvent(event);

    if (checked())
    {
        iconLabel_->setBackGroundImage(checkedIcon_);
    }
    else
    {
        iconLabel_->setBackGroundImage(normalIcon_);
    }

    // TpCanvas *paintCanvas = event->canvas();

    // 在name和space中间画一个竖线
    // std::cout << "spaceLabel_->pos().x()() " << spaceLabel_->pos().x()() << std::endl;
    // std::cout << "deviceNameLabel_->pos().x()() " << deviceNameLabel_->pos().x()() << std::endl;
    // std::cout << "deviceNameLabel_->width() " << deviceNameLabel_->width() << std::endl;

    // uint32_t deviceNameWidth = deviceNameLabel_->width() > deviceNameLabel_->font()->pixelWidth() ? deviceNameLabel_->font()->pixelWidth() : deviceNameLabel_->width();
    // uint32_t lineX = deviceNameLabel_->pos().x()() + deviceNameWidth + (spaceLabel_->pos().x()() - (deviceNameLabel_->pos().x()() + deviceNameWidth)) / 2.0;
    // paintCanvas->vline(lineX, 25, height() - 25, _RGB(190, 196, 202), 2);

    return true;
}

bool diskDeviceCheckBox::eventFilter(TpObject *watched, TpEvent *event)
{
    if (event->eventType() == TpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        TpMouseEvent *mouseEvent = dynamic_cast<TpMouseEvent *>(event);
        onMousePressEvent(mouseEvent);
    }
    else if (event->eventType() == TpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        TpMouseEvent *mouseEvent = dynamic_cast<TpMouseEvent *>(event);
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

    tpShared<TpCssData> cssData = currentStatusCss();

    iconLabel_ = new TpLabel(this);
    iconLabel_->setFixedSize(TpDisplay::dp2Px(33), TpDisplay::dp2Px(33));

    deviceNameLabel_ = new TpLabel(this);
    deviceNameLabel_->font()->setFontColor(cssData->color(), cssData->color());
    deviceNameLabel_->font()->setFontSize(cssData->fontSize());

    TpLine *subLine = new TpLine();
    subLine->setLineType(TpLine::VLine);
    subLine->setLineLength(18);
    subLine->setColor(_RGB(190, 196, 202));
    subLine->setLineWidth(2);

    spaceLabel_ = new TpLabel(this);
    spaceLabel_->font()->setFontColor(_RGB(140, 140, 140), _RGB(140, 140, 140));
    spaceLabel_->font()->setFontSize(cssData->fontSize());

    expandLabel_ = new TpLabel(this);
    expandLabel_->setFixedSize(TpDisplay::dp2Px(23), TpDisplay::dp2Px(23));

    expandLabel_->setBackGroundImage(TpImage(applicationDirPath() + "/../res/箭头-未展开.png"));

    tpShared<TpCssData> curCssData = currentStatusCss();

    TpHBoxLayout *mainLayout = new TpHBoxLayout();
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
    mainLayout->addSpacer(new TpSpacerItem(20, 40, TpSpacerItem::Expanding, TpSpacerItem::Minimum));
    mainLayout->addWidget(expandLabel_);
    setLayout(mainLayout);
}
