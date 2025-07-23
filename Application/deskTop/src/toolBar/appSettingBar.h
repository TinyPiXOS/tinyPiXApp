#ifndef __APP_SETTING_BAR_H
#define __APP_SETTING_BAR_H

#include "tpDialog.h"
#include "tpButton.h"
#include "tpEvent.h"
#include "tpIconTopButton.h"
#include "tpVector.h"
#include "tpSlideProgressBar.h"
#include "tpLabel.h"
#include "tpPanelSwitchButton.h"
#include "powerManage.h"

class appSettingBar
    : public tpDialog
{
public:
    appSettingBar();
    virtual ~appSettingBar();

    virtual void setVisible(bool visible) override;

    // 更新系统时间
    void updateTime(const int32_t &year, const int32_t &month, const int32_t &day, const tpString &weekDay);

    // 设置蓝牙开关状态
    void setBluetoothStatus(const bool &status);

    // void set
protected:
    virtual bool onMousePressEvent(tpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;

    virtual bool onMouseMoveEvent(tpMouseEvent *event) override;

    // virtual bool onLeaveEvent(tpObjectLeaveEvent *event) override;

    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

private:
    // 点击切换蓝牙状态按钮
    void slotSwitchBluetooth(bool checked);

    // 切换WIFI状态
    void slotSwitchWifi(bool checked);

    // 切换音量
    void slotChangeVoice(int32_t value);

    // 切换亮度
    void slotChangelight(int32_t value);

private:
    // 外部窗口大小变换，调整功能区按钮显示
    void resizeOperatorBtn();

    void slotPowerOff(bool checked);

private:
    bool mouseLeftPress_;
    ItpPoint pressPoint_;

    // 日期lable
    tpLabel *dateTimeLabel_;

    tpIconTopButton *powerOffBtn_;

    // // 声音进度条
    tpSlideProgressBar *voiceProgessBar_;

    // // 亮度进度条
    tpSlideProgressBar *lightProgessBar_;

    tpPanelSwitchButton *wifiBtn_;
    tpPanelSwitchButton *bluetoothBtn_;
    // 锁定旋转
    tpPanelSwitchButton *sysLockBtn_;

    // 电源管理窗口
    powerManage *powerManageWindow_;
    // tpVector<tpButton *> operatorBtnList_;
};

#endif