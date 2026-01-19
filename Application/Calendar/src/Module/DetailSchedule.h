#ifndef __DETAIL_SCHEDULE_H
#define __DETAIL_SCHEDULE_H

#include "TpWidget.h"

/// @brief 详细日程信息
class DetailSchedule : public TpWidget
{
public:
    DetailSchedule(TpWidget* parent = nullptr);
    virtual ~DetailSchedule();

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;
};

#endif