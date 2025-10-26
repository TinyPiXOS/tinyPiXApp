#ifndef __TOP_BAR_H
#define __TOP_BAR_H

#include "TpDialog.h"
#include "TpLabel.h"
#include "TpTimer.h"
#include "TpEvent.h"
#include "TpBattery.h"
#include "TpShareMemory.h"

class TopBar : public TpDialog
{
public:
    TopBar();
    virtual ~TopBar();

    /// @brief 设置顶部工具栏颜色
    /// @param appColor 应用底部颜色，根据应用颜色，选择设置工具栏是黑色/白色
    void setColor(const int32_t &appColor);

    virtual void setVisible(bool visible = true) override;

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;
    virtual bool onMousePressEvent(TpMouseEvent *event) override;
    virtual bool onLeaveEvent(TpLeaveEvent *event) override;

private:
    void initUI();

    void caculateTopAppPos();

    void slotUpdateSystemTime();

    // 将周几的数字转为汉字显示
    TpString transWeekData(const int32_t &dayOfWeek);

    void refreshSharedMomery();

private:
    TpShareMemory *shareMemory_;

    TpLabel *sysDateLabel_;
    TpLabel *sysTimeLabel_;

    // 蓝牙和网络连接状态
    TpLabel *wifiLabel_;
    TpLabel *blueToothLabel_;

    // 电量显示窗
    TpBattery *elecBattery_;

    TpTimer *updateTimetimer_;
};

#endif
