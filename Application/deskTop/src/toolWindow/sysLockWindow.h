#ifndef __SYS_LOCK_WINDOW_H
#define __SYS_LOCK_WINDOW_H

#include "tpDialog.h"
#include "tpButton.h"
#include "tpEvent.h"
#include "tpLabel.h"
#include "tpTimer.h"
#include "tpLineEdit.h"

class sysLockWindow
    : public tpDialog
{
public:
    sysLockWindow();
    virtual ~sysLockWindow();

    virtual void setVisible(bool visible = true) override;

public:
    // 系统显示大小变换，锁屏窗口大小也要变化
    void resizeLockWindow(const ItpRect &mainWindowRect);

protected:
    virtual bool onMousePressEvent(tpMouseEvent *event) override;
	virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;

    virtual bool onMouseMoveEvent(tpMouseEvent *event) override;

    virtual bool onLeaveEvent(tpObjectLeaveEvent *event) override;

private:
    void initUi();
    
    // 刷新锁屏界面显示，true为壁纸界面，false为输入密码界面
    void refreshLockWindow(const bool& windowStatus);

    // 系统登录
    void slotLogin();

    // 更新时间
    void slotUpdateSystemTime();
    
private:
    bool mouseLeftPress_;
    ItpPoint pressPoint_;

    // 更新时间定时器
	tpTimer *updateTimetimer_;

    // 锁屏壁纸界面提示信息；时间、解锁提示、解锁方向提示
    tpLabel *systemDateTimeLabel_;
    tpLabel *unlockPromptLabel_;
    tpLabel *unlockDirectLabel_;

    // 输密码解锁界面
    tpLabel *systemNameLabel_;
    tpLabel *welcomeLabel_;
    tpLabel *userIconLabel_;

    tpLineEdit* pwdEdit_;
    tpButton* loginBtn_;
};

#endif