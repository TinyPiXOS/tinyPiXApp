#include "appPreviewWidget.h"
#include "tpDisplay.h"
#include "tpFont.h"
#include "tpHBoxLayout.h"
#include "tpVBoxLayout.h"
#include "tpSurface.h"
#include "deskTopGlobal.hpp"

appPreviewWidget::appPreviewWidget(tpChildWidget *parent)
    : tpChildWidget(parent), pid_(0)
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

void appPreviewWidget::setIcon(const tpString &iconPath)
{
    tpShared<tpSurface> bgImage = tpMakeShared<tpSurface>(iconPath);
    iconLabel_->setBackGroundImage(bgImage);
}

void appPreviewWidget::setName(const tpString &name)
{
    nameLabel_->setText(name);
}

void appPreviewWidget::setPreviewImg(const tpString &path)
{
    tpShared<tpSurface> bgImage = tpMakeShared<tpSurface>(path);
    previewImgLabel_->setBackGroundImage(bgImage);
}

void appPreviewWidget::setPreviewImg(tpShared<tpSurface> image)
{
    if (!image)
        return;

    previewImgLabel_->setBackGroundImage(image);
}

void appPreviewWidget::setId(const int32_t &pid, const int32_t &winId)
{
    pid_ = pid;
    winId_ = winId;
}

int32_t appPreviewWidget::pid()
{
    return pid_;
}

int32_t appPreviewWidget::winId()
{
    return winId_;
}

bool appPreviewWidget::eventFilter(tpObject *watched, tpEvent *event)
{
    if (event->eventType() == tpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMousePressEvent(mouseEvent);

        if (watched == previewImgLabel_)
            return true;
    }
    else if (event->eventType() == tpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMouseRleaseEvent(mouseEvent);

        if (watched == previewImgLabel_)
            return true;
    }
    else if (event->eventType() == tpEvent::EVENT_MOUSE_MOVE_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMouseMoveEvent(mouseEvent);
    }
    else
    {
    }
    return false;
}

bool appPreviewWidget::onMousePressEvent(tpMouseEvent *event)
{
    mousePressPoint_ = event->globalPos();

    return true;
}

bool appPreviewWidget::onMouseRleaseEvent(tpMouseEvent *event)
{
    // 鼠标如果按下过程中拖动，则不触发打开事件
    ItpPoint curPoint = event->globalPos();
    if (std::abs(curPoint.x - mousePressPoint_.x) < 5 && std::abs(curPoint.y - mousePressPoint_.y) < 5)
    {
        signalOpenApp.emit(pid_);
    }

    return true;
}

bool appPreviewWidget::onMouseMoveEvent(tpMouseEvent *event)
{
    return true;
}

bool appPreviewWidget::onLeaveEvent(tpObjectLeaveEvent *event)
{
    return true;
}

void appPreviewWidget::init()
{
    iconLabel_ = new tpLabel(this);
    iconLabel_->setFixedSize(tpDisplay::dp2Px(38), tpDisplay::dp2Px(38));
    iconLabel_->setRoundCorners(6);
    iconLabel_->setProperty("Debug", "iconLabel_");
    iconLabel_->installEventFilter(this);

    nameLabel_ = new tpLabel(this);
    nameLabel_->font()->setFontColor(_RGB(255, 255, 255), _RGB(255, 255, 255));
    nameLabel_->font()->setFontSize(13);
    nameLabel_->setText("默认应用");
    nameLabel_->setProperty("Debug", "nameLabel_");
    nameLabel_->installEventFilter(this);

    previewImgLabel_ = new tpLabel(this);
    previewImgLabel_->setProperty("Debug", "previewImgLabel_");
    previewImgLabel_->installEventFilter(this);

    closeBtn_ = new tpButton(this);
    closeBtn_->setEnableBackGroundColor(false);
    closeBtn_->setFixedSize(tpDisplay::dp2Px(35), tpDisplay::dp2Px(35));
    closeBtn_->setButtonStyle(tpButton::IconOnly);
    closeBtn_->setIcon(applicationDirPath() + "/../res/关闭.png");
    connect(closeBtn_, onClicked, [=](bool)
            { signalKillApp.emit(pid_); });
    closeBtn_->setProperty("Debug", "closeBtn_");

    tpHBoxLayout *titleLayout = new tpHBoxLayout();
    titleLayout->setSpacing(8);
    titleLayout->setContentsMargins(8, 0, 8, 0);
    titleLayout->addWidget(iconLabel_);
    titleLayout->addWidget(nameLabel_);
    titleLayout->addSpacer(new tpSpacerItem(10, 10, tpSpacerItem::Expanding));
    titleLayout->addWidget(closeBtn_);

    tpVBoxLayout *mainLayout = new tpVBoxLayout();
    mainLayout->setSpacing(8);
    mainLayout->addLayout(titleLayout, 1);
    mainLayout->addWidget(previewImgLabel_, 10);

    setLayout(mainLayout);
}
