#include "TopBar.h"
#include "TpFont.h"
#include "TpString.h"
#include "MediaPlayerGlobal.h"
#include "TpDisplay.h"
#include "TpTime.h"
#include "TpDate.h"

TopBar::TopBar()
    : TpDialog()
{
    setEnabledBorderColor(false);
    setBackGroundColor(_RGBA(255, 255, 255, 0));

    init();
}

TopBar::~TopBar()
{
}

void TopBar::setFileName(const TpString &fileName)
{
    fileNameLabel_->setText(fileName);

    TpFont *nameFont = fileNameLabel_->font();

    int32_t fileNameX = (width() - nameFont->pixelWidth()) / 2.0;
    int32_t fileNameY = (height() - nameFont->pixelHeight()) / 2.0;
    fileNameLabel_->move(fileNameX, fileNameY);

    update();
    // fileNameLabel_->update();
}

void TopBar::setBackBtnVisible(const bool &visible)
{
    closeMediaBtn_->setVisible(visible);
}

bool TopBar::onResizeEvent(TpResizeEvent *event)
{
    TpDialog::onResizeEvent(event);

    // 调整标题和列表按钮位置
    if (fileNameLabel_)
    {
        int32_t fileNameX = (width() - fileNameLabel_->width()) / 2.0;
        int32_t fileNameY = (height() - fileNameLabel_->height()) / 2.0;
        fileNameLabel_->move(fileNameX, fileNameY);
    }

    if (fileListBtn_)
    {
        int32_t fileListBtnY = (height() - fileListBtn_->height()) / 2.0;
        fileListBtn_->move(width() - fileListBtn_->width() - globalRightMargin, fileListBtnY);

        closeMediaBtn_->move(globalLeftMargin, fileListBtnY);
    }

    return true;
}

void TopBar::slotCloseMedia(bool)
{
}

void TopBar::init()
{
    fileNameLabel_ = new TpLabel(this);
    fileNameLabel_->font()->setFontColor(_RGB(255, 255, 255), _RGB(255, 255, 255));
    fileNameLabel_->font()->setFontSize(18);
    fileNameLabel_->setText("");

    // 展开文件列表按钮
    fileListBtn_ = new TpButton(this);
    fileListBtn_->setButtonStyle(TpButton::IconOnly);
    fileListBtn_->setEnableBackGroundColor(false);
    fileListBtn_->setFixedSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    fileListBtn_->setIcon(applicationDirPath() + "/../res/文件列表.png");
    connect(fileListBtn_, onClicked, [=](bool) {});

    closeMediaBtn_ = new TpButton(this);
    closeMediaBtn_->setButtonStyle(TpButton::IconOnly);
    closeMediaBtn_->setEnableBackGroundColor(false);
    closeMediaBtn_->setFixedSize(TpDisplay::dp2Px(34), TpDisplay::dp2Px(34));
    closeMediaBtn_->setIcon(applicationDirPath() + "/../res/后退.png");
    connect(closeMediaBtn_, onClicked, this, &TopBar::slotCloseMedia);
    closeMediaBtn_->setVisible(false);
}
