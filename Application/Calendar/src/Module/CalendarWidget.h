#ifndef __CALENDAR_WIDGET_H
#define __CALENDAR_WIDGET_H

#include "TpWidget.h"
#include "TpVector.h"
#include "TpString.h"
#include "TpFont.h"
#include "TpDate.h"

/// @brief 日历显示信息
class CalendarWidget : public TpWidget
{
public:
    CalendarWidget(TpWidget *parent = nullptr);
    virtual ~CalendarWidget();

    /// @brief 设置要显示的年月
    /// @param year 年份
    /// @param month 月份
    void setDate(int32_t year, int32_t month);

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;
    virtual bool onPaintEvent(TpPaintEvent *event) override;

private:
    TpDate curShowDate_;

    TpVector<TpString> weekNameList_;
    TpFont weekNameFont_;

    TpFont chineseFont_;
    TpFont lastMonthTextFont_;
    TpFont curMonthTextFont_;
};

#endif