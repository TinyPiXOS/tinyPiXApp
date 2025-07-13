#ifndef __BOTTOM_BAR_H
#define __BOTTOM_BAR_H

#include "tpDialog.h"
#include "tpButton.h"
#include "tpLabel.h"
#include "tpSlider.h"
#include "tpMenu.h"
#include "mediaPlayerGlobal.h"

class bottomBar : public tpDialog
{
public:
    bottomBar();
    virtual ~bottomBar();

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
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

private:
    // 切换倍速
    void slotSwitchSpeed(uint32_t speedIndex);

    // 滚动条拖拽修改进度
    void slotChangeProgress(int32_t curValue);

private:
    void init();

    tpButton *generalIconBtn(const tpString &iconPath);

    // 计算播放进度字符串并更新UI
    void caculatePlayProgressStr();

private:
    // 上一个、下一个
    tpButton *previousFileBtn_;
    tpButton *nextFileBtn_;

    // 快进快退
    tpButton *forwardBtn_;
    tpButton *backwardBtn_;

    // 暂停
    tpButton *pauseBtn_;
    bool isPause_;
    
    // 倍速
    tpButton *speedBtn_;
    tpMenu *speedMenu_;

    // 播放进度
    tpLabel *progressLabel_;
    tpSlider *progressSlider_;

    // 当前进度和总时长 S
    double curTimeS_;
    double allTimeS_;
};

#endif
