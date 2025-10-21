#include "appPreviewWidget.h"
#include "TpDisplay.h"
#include "TpFont.h"
#include "TpHBoxLayout.h"
#include "TpVBoxLayout.h"
#include "TpImage.h"
#include "deskTopGlobal.hpp"

appPreviewWidget::appPreviewWidget(TpChildWidget *parent)
    : TpChildWidget(parent)
{
    init();
}

appPreviewWidget::~appPreviewWidget()
{
    // delete iconLabel_;
    // delete nameLabel_;
    // delete previewImgLabel_;
    // delete closeBtn_;

    iconLabel_->deleteLater();
    iconLabel_ = nullptr;

    nameLabel_->deleteLater();
    nameLabel_ = nullptr;

    previewImgLabel_->deleteLater();
    previewImgLabel_ = nullptr;

    closeBtn_->deleteLater();
    closeBtn_ = nullptr;
}

void appPreviewWidget::setIcon(const TpString &iconPath)
{
    iconLabel_->setBackGroundImage(TpImage(iconPath));
}

void appPreviewWidget::setName(const TpString &name)
{
    nameLabel_->setText(name);
}

void appPreviewWidget::setPreviewImg(const TpString &path)
{
    previewImgLabel_->setBackGroundImage(TpImage(path));
}

void appPreviewWidget::setPreviewImg(TpImage image)
{
    previewImgLabel_->setBackGroundImage(image);
}

void appPreviewWidget::setAppUuid(const TpString &uuid)
{
    appUuid_ = uuid;
}

TpString appPreviewWidget::appUuid()
{
    return appUuid_;
}

bool appPreviewWidget::eventFilter(TpObject *watched, TpEvent *event)
{
    if (event->eventType() == TpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        TpMouseEvent *mouseEvent = dynamic_cast<TpMouseEvent *>(event);
        onMousePressEvent(mouseEvent);

        if (watched == previewImgLabel_)
            return true;
    }
    else if (event->eventType() == TpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        TpMouseEvent *mouseEvent = dynamic_cast<TpMouseEvent *>(event);
        onMouseRleaseEvent(mouseEvent);

        if (watched == previewImgLabel_)
            return true;
    }
    else if (event->eventType() == TpEvent::EVENT_MOUSE_MOVE_TYPE)
    {
        TpMouseEvent *mouseEvent = dynamic_cast<TpMouseEvent *>(event);
        onMouseMoveEvent(mouseEvent);
    }
    else
    {
    }
    return false;
}

bool appPreviewWidget::onMousePressEvent(TpMouseEvent *event)
{
    mousePressPoint_ = event->globalPos();

    return true;
}

bool appPreviewWidget::onMouseRleaseEvent(TpMouseEvent *event)
{
    // 鼠标如果按下过程中拖动，则不触发打开事件
    TpPoint curPoint = event->globalPos();
    if (std::abs(curPoint.x() - mousePressPoint_.x()) < 5 && std::abs(curPoint.y() - mousePressPoint_.y()) < 5)
    {
        signalOpenApp.emit(appUuid_);
    }

    return true;
}

bool appPreviewWidget::onResizeEvent(TpResizeEvent *event)
{
    refreshUi();
    return true;
}

void appPreviewWidget::init()
{
    iconLabel_ = new TpLabel(this);
    iconLabel_->setFixedSize(TpDisplay::dp2Px(38), TpDisplay::dp2Px(38));
    iconLabel_->setRoundCorners(6);
    iconLabel_->setProperty("Debug", "iconLabel_");
    iconLabel_->installEventFilter(this);

    nameLabel_ = new TpLabel(this);
    nameLabel_->font()->setFontColor(_RGB(255, 255, 255), _RGB(255, 255, 255));
    nameLabel_->font()->setFontSize(13);
    nameLabel_->setText("默认应用");
    nameLabel_->setProperty("Debug", "nameLabel_");
    nameLabel_->installEventFilter(this);

    previewImgLabel_ = new TpLabel(this);
    previewImgLabel_->setProperty("Debug", "previewImgLabel_");
    previewImgLabel_->installEventFilter(this);
    // previewImgLabel_->setBackGroundColor(_RGB(255, 255, 255));

    closeBtn_ = new TpButton(this);
    closeBtn_->setEnableBackGroundColor(false);
    closeBtn_->setFixedSize(TpDisplay::dp2Px(35), TpDisplay::dp2Px(35));
    closeBtn_->setButtonStyle(TpButton::IconOnly);
    closeBtn_->setIcon(applicationDirPath() + "/../res/关闭.png");
    connect(closeBtn_, onClicked, [=](bool)
            { signalKillApp.emit(appUuid_); });
    closeBtn_->setProperty("Debug", "closeBtn_");

    TpHBoxLayout *titleLayout = new TpHBoxLayout();
    titleLayout->setSpacing(8);
    titleLayout->setContentsMargins(8, 0, 8, 0);
    titleLayout->addWidget(iconLabel_);
    titleLayout->addWidget(nameLabel_);
    titleLayout->addSpacer(new TpSpacerItem(10, 5, TpSpacerItem::Expanding));
    titleLayout->addWidget(closeBtn_);

    TpVBoxLayout *mainLayout = new TpVBoxLayout();
    mainLayout->setSpacing(5);
    mainLayout->addLayout(titleLayout, 1);
    mainLayout->addWidget(previewImgLabel_, 5);

    setLayout(mainLayout);
}

void appPreviewWidget::refreshUi()
{
    // iconLabel_->move(8, 0);
    // nameLabel_->move(iconLabel_->pos().x() + iconLabel_->width() + 8, 0);
    // closeBtn_->move(width() - closeBtn_->width(), 0);

    // previewImgLabel_->move(0, iconLabel_->pos().y() + iconLabel_->height() + 5);
    // previewImgLabel_->setSize(width(), height() - previewImgLabel_->pos().y());
}
