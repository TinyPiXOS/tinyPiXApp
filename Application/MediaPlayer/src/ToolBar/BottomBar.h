#ifndef __BOTTOM_BAR_H
#define __BOTTOM_BAR_H

#include "TpDesktopDialog.h"
#include "TpButton.h"
#include "TpLabel.h"
#include "TpSlider.h"
#include "TpMenu.h"
#include "MediaPlayerGlobal.h"

class BottomBar : public TpDesktopDialog
{
public:
    BottomBar();
    virtual ~BottomBar();

    // 设置当前进度时长
    void setCurTimeS(const double &timeS);

    // 设置文件总时长
    void setDurationTimeS(const double &allTime);

public
signals:
    /// @brief 媒体文件操作
    /// @param PlayerOperate 当前值
    declare_signal(medioOperate, PlayerOperate);

    // 手动调整播放进度；当前时间
    declare_signal(alterPostion, int32_t);

    // 切换倍速；倍速索引
    /*
        "3X"        0
        "2X"        1
        "1.5X"      2
        "1X"        3
    */
    declare_signal(switchSpeed, int32_t);

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;

private:
    // 切换倍速
    void slotSwitchSpeed(uint32_t speedIndex);

    // 滚动条拖拽修改进度
    void slotChangeProgress(int32_t curValue);

private:
    void init();

    TpButton *generalIconBtn(const TpString &iconPath);

    // 计算播放进度字符串并更新UI
    void caculatePlayProgressStr();

private:
    // 上一个、下一个
    TpButton *previousFileBtn_;
    TpButton *nextFileBtn_;

    // 快进快退
    TpButton *forwardBtn_;
    TpButton *backwardBtn_;

    // 暂停
    TpButton *pauseBtn_;
    bool isPause_;
    
    // 倍速
    TpButton *speedBtn_;
    TpMenu *speedMenu_;

    // 播放进度
    TpLabel *progressLabel_;
    TpSlider *progressSlider_;

    // 当前进度和总时长 S
    double curTimeS_;
    double allTimeS_;
};

#endif
