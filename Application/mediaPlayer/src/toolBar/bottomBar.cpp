#include "bottomBar.h"
#include "tpFont.h"
#include "tpString.h"
#include "tpDisplay.h"
#include "tpTime.h"
#include "tpDate.h"

// 按钮之间间距
const int32_t buttonMargin = 31;

bottomBar::bottomBar()
    : tpDialog(), allTimeS_(0), curTimeS_(0), isPause_(false)
{
    setEnabledBorderColor(false);
    setBackGroundColor(_RGBA(255, 255, 255, 0));

    init();
}

void bottomBar::setCurTimeS(const double &timeS)
{
    curTimeS_ = timeS;
    caculatePlayProgressStr();
}

void bottomBar::setDurationTimeS(const double &allTime)
{
    allTimeS_ = allTime;

    progressSlider_->setRange(0, allTimeS_);
    caculatePlayProgressStr();
}

bottomBar::~bottomBar()
{
}

bool bottomBar::onResizeEvent(tpObjectResizeEvent *event)
{
    tpDialog::onResizeEvent(event);

    // 调整布局
    // 进度条
    progressSlider_->setSize(width() - globalLeftMargin - globalRightMargin, 6);
    progressSlider_->move(globalLeftMargin, 0);

    int32_t operateBtnY = (height() - previousFileBtn_->height()) / 2.0 + 5;
    int32_t curBtnX = globalLeftMargin;

    previousFileBtn_->move(curBtnX, operateBtnY);
    curBtnX += previousFileBtn_->width() + buttonMargin;

    backwardBtn_->move(curBtnX, operateBtnY);
    curBtnX += backwardBtn_->width() + buttonMargin;

    pauseBtn_->move(curBtnX, operateBtnY);
    curBtnX += pauseBtn_->width() + buttonMargin;

    forwardBtn_->move(curBtnX, operateBtnY);
    curBtnX += forwardBtn_->width() + buttonMargin;

    nextFileBtn_->move(curBtnX, operateBtnY);
    curBtnX += nextFileBtn_->width() + buttonMargin;

    int32_t progressLabelY = (height() - progressLabel_->height()) / 2.0 + 5;
    progressLabel_->move(curBtnX, progressLabelY);

    speedBtn_->move(width() - globalRightMargin - speedBtn_->width(), operateBtnY);

    return true;
}

void bottomBar::slotSwitchSpeed(uint32_t speedIndex)
{
    /*
        "3X"
        "2X"
        "1.5X"
        "1X"
    */
    tpString speedName = speedMenu_->itemText(speedIndex);
    speedBtn_->setText(speedName);

    // 切换倍速 TODO
    switchSpeed.emit(speedIndex);
}

void bottomBar::slotChangeProgress(int32_t curValue)
{
    alterPostion.emit(curValue);
}

void bottomBar::init()
{
    tpString resPath = applicationDirPath() + "/../res/";

    // 上一个、下一个
    previousFileBtn_ = generalIconBtn(applicationDirPath() + "/../res/上一个.png");
    connect(previousFileBtn_, onClicked, [=](bool)
            { medioOperate.emit(PreviousFile); });
    nextFileBtn_ = generalIconBtn(applicationDirPath() + "/../res/下一个.png");
    connect(nextFileBtn_, onClicked, [=](bool)
            { medioOperate.emit(NextFile); });

    // 快进快退
    forwardBtn_ = generalIconBtn(applicationDirPath() + "/../res/快进.png");
    connect(forwardBtn_, onClicked, [=](bool)
            { medioOperate.emit(Forward); });
    backwardBtn_ = generalIconBtn(applicationDirPath() + "/../res/快退.png");
    connect(backwardBtn_, onClicked, [=](bool)
            { medioOperate.emit(Backward); });

    // 暂停
    pauseBtn_ = generalIconBtn(applicationDirPath() + "/../res/暂停.png");
    connect(pauseBtn_, onClicked, [=](bool)
            { medioOperate.emit(isPause_ ? Continue : Pause);
                isPause_ = !isPause_;
                pauseBtn_->setIcon(applicationDirPath() + (isPause_ ? "/../res/播放.png": "/../res/暂停.png")); });

    // 倍速
    speedMenu_ = new tpMenu();
    speedMenu_->addItem("3X");
    speedMenu_->addItem("2X");
    speedMenu_->addItem("1.5X");
    speedMenu_->addItem("1X");
    speedMenu_->setMinumumWidth(126);
    connect(speedMenu_, onClicked, this, bottomBar::slotSwitchSpeed);

    speedBtn_ = new tpButton(this);
    speedBtn_->setButtonStyle(tpButton::TextOnly);
    speedBtn_->setBackGroundColor(_RGB(255, 255, 255));
    speedBtn_->setFixedSize(tpDisplay::dp2Px(53), tpDisplay::dp2Px(28));
    speedBtn_->setRoundCorners(tpDisplay::dp2Px(7));
    speedBtn_->setText("1X");
    connect(speedBtn_, onClicked, [=](bool)
            {
        ItpRect btnGlobalRect = speedBtn_->toScreen();
        int32_t menuX = btnGlobalRect.x + btnGlobalRect.w- speedMenu_->width() ;
        int32_t menuY = btnGlobalRect.y - 7 - speedMenu_->height();

        std::cout << "menuX  " << menuX << " menuY " << menuY <<std::endl;
        speedMenu_->exec(menuX, menuY); });

    // 播放进度
    progressLabel_ = new tpLabel(this);
    progressLabel_->setText("0:0 / 0:0");
    progressLabel_->setEnableBackGroundColor(false);
    progressLabel_->font()->setFontColor(_RGB(255, 255, 255), _RGB(255, 255, 255));
    progressLabel_->font()->setFontSize(15);

    progressSlider_ = new tpSlider(this);
    progressSlider_->setValue(0);
    connect(progressSlider_, valueChanged, this, bottomBar::slotChangeProgress);
}

tpButton *bottomBar::generalIconBtn(const tpString &iconPath)
{
    tpButton *newButton = new tpButton(this);
    newButton->setButtonStyle(tpButton::IconOnly);
    newButton->setEnableBackGroundColor(false);
    newButton->setFixedSize(tpDisplay::dp2Px(34), tpDisplay::dp2Px(34));
    newButton->setIcon(iconPath);
    return newButton;
}

void bottomBar::caculatePlayProgressStr()
{
    if (std::abs(allTimeS_ - 0) < 1e-5)
        return;

    // 1:24 / 4:56
    int32_t curTimeMinute = curTimeS_ / 60;
    int32_t curTimeSecond = (int32_t)curTimeS_ % 60;

    int32_t allTimeMinute = allTimeS_ / 60;
    int32_t allTimeSecond = (int32_t)allTimeS_ % 60;

    tpString progresStr = tpString::number(curTimeMinute) + ":" + tpString::number(curTimeSecond) + " / " + tpString::number(allTimeMinute) + ":" + tpString::number(allTimeSecond);

    progressLabel_->setText(progresStr);

    // double playPercent = 100.0 * curTimeS_ / allTimeS_;
    // progressSlider_->setValue(playPercent);
    progressSlider_->setValue(curTimeS_);
}
