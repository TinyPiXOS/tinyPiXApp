#include "CalendarWidget.h"
#include "TpPainter.h"
#include "TpChineseCalendar.h"
#include "TpEvent.h"

int32_t globalTableRowCount = 6;
int32_t globalTableColumnCount = 7;

int32_t globalTextLeftRightMargin = 15;

CalendarWidget::CalendarWidget(TpWidget *parent) : TpWidget(parent)
{
    curShowDate_ = TpDate::currentDate();
    curShowDate_.setDay(1);

    weekNameList_ = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};

    weekNameFont_.setFontColor(_RGB(150, 150, 150));
    weekNameFont_.setFontSize(13);

    curMonthTextFont_.setFontColor(_RGB(0, 0, 0));
    curMonthTextFont_.setFontSize(13);

    lastMonthTextFont_.setFontColor(_RGB(136, 136, 136));
    lastMonthTextFont_.setFontSize(13);

    chineseFont_.setFontColor(_RGB(136, 136, 136));
    chineseFont_.setFontSize(11);
}

CalendarWidget::~CalendarWidget()
{
}

void CalendarWidget::setDate(int32_t year, int32_t month)
{
    curShowDate_.setYear(year);
    curShowDate_.setMonth(month);
    curShowDate_.setDay(1);
}

bool CalendarWidget::onResizeEvent(TpResizeEvent *event)
{
    TpWidget::onResizeEvent(event);
    return true;
}

bool CalendarWidget::onPaintEvent(TpPaintEvent *event)
{
    TpWidget::onPaintEvent(event);

    TpPainter *painter = event->painter();

    // 绘制底部网格线，表格共 六行 七列；第一行为 周几 标题
    painter->setPen(_RGB(190, 196, 202));

    // 绘制外边框
    painter->drawHLine(0, width(), 0);
    painter->drawVLine(0, 0, height());
    painter->drawHLine(0, width(), height());
    painter->drawVLine(width(), 0, height());

    // 计算每一行平均行高; 第一行是其余行行高的40%，所以第一行算4份，其余每行占10份
    float singleRowHeight = 1.0 * height() / (3.5 + (globalTableRowCount - 1) * 10);
    int32_t singleColumnWidth = 1.0 * width() / globalTableColumnCount;

    int32_t firstRowHeight = singleRowHeight * 3.5;
    int32_t otherRowHeight = singleRowHeight * 10;

    // 浮点数计算像素有误差，将误差叠加到第一行
    if ((firstRowHeight + otherRowHeight * (globalTableRowCount - 1)) != height())
    {
        firstRowHeight = height() - otherRowHeight * (globalTableRowCount - 1);
    }

    // 绘制内部网格线
    int32_t curRowY = firstRowHeight;
    int32_t curColumnX = singleColumnWidth;
    for (int i = 0; i < globalTableColumnCount; ++i)
    {
        painter->drawVLine(curColumnX, 0, height());
        curColumnX += singleColumnWidth;

        if (i < (globalTableRowCount - 1))
        {
            painter->drawHLine(0, width(), curRowY);
            curRowY += otherRowHeight;
        }
    }

    // 绘制标题栏
    weekNameFont_.setText(weekNameList_.front());
    int32_t weekTextY = 1.0 * (firstRowHeight - weekNameFont_.pixelHeight()) / 2.0;

    for (int i = 0; i < weekNameList_.size(); ++i)
    {
        weekNameFont_.setText(weekNameList_.at(i));
        int32_t curWeekNameX = singleColumnWidth * i + globalTextLeftRightMargin * 0.7;
        painter->drawText(weekNameFont_, curWeekNameX, weekTextY);
    }

    // 获取今天的日期
    bool isCurrentMonth = false;
    TpDate currentDate = TpDate::currentDate();
    if ((currentDate.year() == curShowDate_.year()) && (currentDate.month() == curShowDate_.month()))
        isCurrentMonth = true;

    // 根据当前月份绘制填充内容
    // 获取本月第一天为周几
    int32_t monthFirstDayWeek = curShowDate_.dayOfWeek();
    int32_t dayInfoStartColumn = monthFirstDayWeek == globalTableColumnCount ? 0 : monthFirstDayWeek;

    // 获取本月共计多少天
    int32_t curMonthDays = curShowDate_.daysInMonth();
    int32_t curDayTextY = firstRowHeight;

    TpFont offsetFont;
    offsetFont.setFontSize(curMonthTextFont_.fontSize());
    offsetFont.setText("00");

    // 绘制上个月的末尾几天填充完整表格
    if (monthFirstDayWeek != 0)
    {
        TpDate lastMonth = curShowDate_.addMonths(-1);
        int32_t lastMonthDays = lastMonth.daysInMonth();

        int32_t curTextY = curDayTextY + offsetFont.pixelWidth();

        for (int i = 0; i < monthFirstDayWeek; ++i)
        {
            // 填充阴影背景
            painter->setPen(_RGB(236, 236, 236));
            painter->setBrush(TpBrush(_RGB(236, 236, 236)));
            painter->drawRect(singleColumnWidth * i + 1, curDayTextY + 1, singleColumnWidth - 2, otherRowHeight - 2);
            painter->setBrush(TpBrush(Tp::NoBrush));

            lastMonthTextFont_.setText(TpString::number(lastMonthDays - monthFirstDayWeek + i + 1));
            int32_t curDayNumX = singleColumnWidth * i + globalTextLeftRightMargin;
            painter->drawText(lastMonthTextFont_, curDayNumX, curTextY);

            // 绘制农历日期
            TpChineseCalendar chineseCalendarText(lastMonth.year(), lastMonth.month(), lastMonthDays - monthFirstDayWeek + i + 1);
            chineseFont_.setText(chineseCalendarText.dayName());

            int32_t curChineseTextX = singleColumnWidth * (i + 1) - globalTextLeftRightMargin - chineseFont_.pixelWidth();
            painter->drawText(chineseFont_, curChineseTextX, curTextY);
        }
    }

    // 绘制当前月的信息
    for (int i = 0; i < curMonthDays; ++i)
    {
        curMonthTextFont_.setText(TpString::number(i + 1));
        int32_t curDayNumX = singleColumnWidth * dayInfoStartColumn + globalTextLeftRightMargin;
        int32_t curTextY = curDayTextY + offsetFont.pixelWidth();

        // 当前日期绘制一个背景
        if (isCurrentMonth && (currentDate.day() == (i + 1)))
        {
            painter->setPen(_RGB(140, 29, 235));
            painter->setBrush(TpBrush(_RGB(140, 29, 235)));
            painter->drawEllipse(curDayNumX + (curMonthTextFont_.pixelWidth() / 2.0), curTextY + (curMonthTextFont_.pixelHeight() / 2.0),
                                 curMonthTextFont_.pixelWidth(), curMonthTextFont_.pixelWidth());
        }

        painter->drawText(curMonthTextFont_, curDayNumX, curTextY);

        // 绘制农历日期
        TpChineseCalendar chineseCalendarText(curShowDate_.year(), curShowDate_.month(), i);
        chineseFont_.setText(chineseCalendarText.dayName());

        int32_t curChineseTextX = singleColumnWidth * (dayInfoStartColumn + 1) - globalTextLeftRightMargin - chineseFont_.pixelWidth();
        painter->drawText(chineseFont_, curChineseTextX, curTextY);

        ++dayInfoStartColumn;

        if (dayInfoStartColumn == 7)
        {
            dayInfoStartColumn = 0;
            curDayTextY += otherRowHeight;
        }
    }

    // 计算当前月信息是否占满了表格，没有占满需要将下个月的日期补全表格
    int32_t drawNextMonthDays = globalTableColumnCount * (globalTableRowCount - 1) - curMonthDays - monthFirstDayWeek;
    if (drawNextMonthDays > 0)
    {
        TpDate nextMonthDate = curShowDate_.addMonths(1);
        int32_t curTextY = curDayTextY + offsetFont.pixelWidth();

        int32_t nextDayStartIndex = globalTableColumnCount - (drawNextMonthDays % globalTableColumnCount);
        for (int i = 0; i < drawNextMonthDays; ++i)
        {
            // 填充阴影背景
            painter->setPen(_RGB(236, 236, 236));
            painter->setBrush(TpBrush(_RGB(236, 236, 236)));
            painter->drawRect(singleColumnWidth * nextDayStartIndex + 1, curDayTextY + 1, singleColumnWidth - 2, otherRowHeight - 2);
            painter->setBrush(TpBrush(Tp::NoBrush));

            lastMonthTextFont_.setText(TpString::number(i + 1));
            int32_t curDayNumX = singleColumnWidth * nextDayStartIndex + globalTextLeftRightMargin;
            painter->drawText(lastMonthTextFont_, curDayNumX, curTextY);

            // 绘制农历日期
            TpChineseCalendar chineseCalendarText(nextMonthDate.year(), nextMonthDate.month(), i + 1);
            chineseFont_.setText(chineseCalendarText.dayName());

            int32_t curChineseTextX = singleColumnWidth * (nextDayStartIndex + 1) - globalTextLeftRightMargin - chineseFont_.pixelWidth();
            painter->drawText(chineseFont_, curChineseTextX, curTextY);

            ++nextDayStartIndex;

            if (nextDayStartIndex == globalTableColumnCount)
            {
                nextDayStartIndex = 0;
                curDayTextY += otherRowHeight;
            }
        }
    }

    return true;
}
