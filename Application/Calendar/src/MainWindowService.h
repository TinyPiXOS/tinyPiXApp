#ifndef __MAIN_WINDOW_SERVICE_H
#define __MAIN_WINDOW_SERVICE_H

#include "TpDesktopMainWindow.h"
#include "TpButton.h"
#include "TpLabel.h"
#include "Module/CalendarWidget.h"
#include "Module/DetailSchedule.h"
#include "TpDate.h"

class MainWindowService
    : public TpDesktopMainWindow
{
public:
    MainWindowService();
    virtual ~MainWindowService();

public:
    virtual bool appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require) override;

protected:
    virtual bool onActiveEvent(TpActiveEvent *event) override;

private:
    void initUi();

    void initSlots();

    // 获取当前年月字符串用于界面显示
    TpString parseShowMonthText(const TpDate &showDate);

    // 将月份数字转换为中文
    TpString parseMonthChinese(int32_t month);

private:
    // 当前显示的年月份
    TpDate currentShowDate_;

    // 跳转到今天按钮
    TpButton *todayBtn_;

    // 上个月。下个月按钮和当前月显示标签
    TpButton *previousMonthBtn_;
    TpButton *nextMonthBtn_;
    TpLabel *curYearMonthLabel_;

    // 日历显示窗口和详细日程信息窗口
    CalendarWidget *calendarWin_;
    DetailSchedule *detailScheduleWin_;
};

#endif