#include "topBar.h"
#include "tpFont.h"
#include "tpString.h"
#include "mediaPlayerGlobal.h"
#include "tpDisplay.h"
#include "tpTime.h"
#include "tpDate.h"

topBar::topBar()
    : tpDialog()
{
    setEnabledBorderColor(false);
    setBackGroundColor(_RGBA(255, 255, 255, 0));

    init();
}

topBar::~topBar()
{
}

void topBar::setFileName(const tpString &fileName)
{
    fileNameLabel_->setText(fileName);

    tpFont *nameFont = fileNameLabel_->font();

    int32_t fileNameX = (width() - nameFont->pixelWidth()) / 2.0;
    int32_t fileNameY = (height() - nameFont->pixelHeight()) / 2.0;
    fileNameLabel_->move(fileNameX, fileNameY);

    update();
    // fileNameLabel_->update();
}

void topBar::setBackBtnVisible(const bool &visible)
{
    closeMediaBtn_->setVisible(visible);
}

bool topBar::onResizeEvent(tpObjectResizeEvent *event)
{
    tpDialog::onResizeEvent(event);

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

void topBar::slotCloseMedia(bool)
{
}

void topBar::init()
{
    fileNameLabel_ = new tpLabel(this);
    fileNameLabel_->font()->setFontColor(_RGB(255, 255, 255), _RGB(255, 255, 255));
    fileNameLabel_->font()->setFontSize(18);
    fileNameLabel_->setText("");

    // 展开文件列表按钮
    fileListBtn_ = new tpButton(this);
    fileListBtn_->setButtonStyle(tpButton::IconOnly);
    fileListBtn_->setEnableBackGroundColor(false);
    fileListBtn_->setFixedSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    fileListBtn_->setIcon(applicationDirPath() + "/../res/文件列表.png");
    connect(fileListBtn_, onClicked, [=](bool) {});

    closeMediaBtn_ = new tpButton(this);
    closeMediaBtn_->setButtonStyle(tpButton::IconOnly);
    closeMediaBtn_->setEnableBackGroundColor(false);
    closeMediaBtn_->setFixedSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    closeMediaBtn_->setIcon(applicationDirPath() + "/../res/后退.png");
    connect(closeMediaBtn_, onClicked, this, topBar::slotCloseMedia);
    closeMediaBtn_->setVisible(false);
}
