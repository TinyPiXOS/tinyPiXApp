#include "MainWindowService.h"
#include "CalendarGlobal.h"
#include "TpFont.h"
#include "TpDisplay.h"

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setBackGroundColor(_RGB(255, 255, 255));

    initUi();
    initSlots();
}

MainWindowService::~MainWindowService()
{
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}

void MainWindowService::initUi()
{
    currentShowDate_ = TpDate::currentDate();

    int32_t leftRightMargin = width() * 0.024;
    int32_t topMargin = height() * 0.03555;

    todayBtn_ = new TpButton(this);
    todayBtn_->setText("今天");
    todayBtn_->setEnableBackGroundColor(false);
    todayBtn_->setProperty("type", "TextButton");
    todayBtn_->font()->setFontSize(13);
    todayBtn_->setFixedSize(todayBtn_->font()->pixelWidth() , TpDisplay::dp2Px(30));
    todayBtn_->move(leftRightMargin, topMargin);

    nextMonthBtn_ = new TpButton(this);
    nextMonthBtn_->setButtonStyle(TpButton::IconOnly);
    // nextMonthBtn_->setBackGroundColor(_RGB(247, 248, 248));
    nextMonthBtn_->setIcon(applicationDirPath() + "/../res/下一月.png");
    nextMonthBtn_->setFixedSize(TpDisplay::dp2Px(30), TpDisplay::dp2Px(30));
    nextMonthBtn_->setIconSize(nextMonthBtn_->width() * 0.5, nextMonthBtn_->width() * 0.5);
    nextMonthBtn_->setRoundCorners(5);
    nextMonthBtn_->move(width() - leftRightMargin - nextMonthBtn_->width(), topMargin);

    curYearMonthLabel_ = new TpLabel(this);
    curYearMonthLabel_->setText(parseShowMonthText(currentShowDate_));
    curYearMonthLabel_->setAlign(Tp::AlignCenter);
    curYearMonthLabel_->font()->setFontColor(_RGB(54, 59, 100));
    curYearMonthLabel_->font()->setFontSize(11);
    curYearMonthLabel_->setFixedSize(curYearMonthLabel_->font()->pixelWidth() + 60, TpDisplay::dp2Px(30));
    curYearMonthLabel_->move(nextMonthBtn_->pos().x() - curYearMonthLabel_->width(), topMargin);

    previousMonthBtn_ = new TpButton(this);
    previousMonthBtn_->setButtonStyle(TpButton::IconOnly);
    // previousMonthBtn_->setBackGroundColor(_RGB(247, 248, 248));
    previousMonthBtn_->setIcon(applicationDirPath() + "/../res/上一月.png");
    previousMonthBtn_->setFixedSize(TpDisplay::dp2Px(30), TpDisplay::dp2Px(30));
    previousMonthBtn_->setIconSize(nextMonthBtn_->width() * 0.5, nextMonthBtn_->width() * 0.5);
    previousMonthBtn_->setRoundCorners(5);
    previousMonthBtn_->move(curYearMonthLabel_->pos().x() - previousMonthBtn_->width(), topMargin);

    calendarWin_ = new CalendarWidget(this);
    calendarWin_->setFixedSize(width() - leftRightMargin * 2, height() - previousMonthBtn_->pos().y() - previousMonthBtn_->width() - topMargin * 2);
    calendarWin_->move(leftRightMargin, previousMonthBtn_->pos().y() + previousMonthBtn_->height() + topMargin);

    detailScheduleWin_ = new DetailSchedule(this);
    detailScheduleWin_->setVisible(false);
}

void MainWindowService::initSlots()
{
    connect(todayBtn_, onClicked, [=](bool)
            {
                currentShowDate_ = TpDate::currentDate();
                curYearMonthLabel_->setText(parseShowMonthText(currentShowDate_));
                calendarWin_->setDate(currentShowDate_.year(), currentShowDate_.month()); });

    // 上个月、下个月
    connect(previousMonthBtn_, onClicked, [=](bool)
            {
                currentShowDate_ = currentShowDate_.addMonths(-1);
                curYearMonthLabel_->setText(parseShowMonthText(currentShowDate_));
                calendarWin_->setDate(currentShowDate_.year(), currentShowDate_.month()); });
    connect(nextMonthBtn_, onClicked, [=](bool)
            {
                currentShowDate_ = currentShowDate_.addMonths(1);
                curYearMonthLabel_->setText(parseShowMonthText(currentShowDate_));
                calendarWin_->setDate(currentShowDate_.year(), currentShowDate_.month()); });
}

TpString MainWindowService::parseShowMonthText(const TpDate &showDate)
{
    TpString chineseMonth = parseMonthChinese(showDate.month());
    return chineseMonth + " " + TpString::number(showDate.year());
}

TpString MainWindowService::parseMonthChinese(int32_t month)
{
    if (month <= 0)
        month = 1;
    if (month > 12)
        month = 12;

    switch (month)
    {
    case 1:
        return "一月";
    case 2:
        return "二月";
    case 3:
        return "三月";
    case 4:
        return "四月";
    case 5:
        return "五月";
    case 6:
        return "六月";
    case 7:
        return "七月";
    case 8:
        return "八月";
    case 9:
        return "九月";
    case 10:
        return "十月";
    case 11:
        return "十一月";
    case 12:
        return "十二月";
    default:
        return "一月";
    }

    return "一月";
}
