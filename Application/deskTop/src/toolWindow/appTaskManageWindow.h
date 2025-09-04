#ifndef __APP_TASK_MANAGE_WINDOW_H
#define __APP_TASK_MANAGE_WINDOW_H

/*
    应用任务管理器
*/

#include "TpDialog.h"
#include "TpButton.h"
#include "TpEvent.h"
#include "TpScrollPanel.h"
#include "TpVector.h"
#include "toolWindow/appPreviewWidget.h"
#include "TpHash.h"

class appTaskManageWindow
    : public TpDialog
{
public:
    appTaskManageWindow();
    virtual ~appTaskManageWindow();

    virtual void setVisible(bool visible = true) override;

    int32_t getWinIdByPid(const int32_t &pid);

protected:
    virtual bool eventFilter(TpObject *watched, TpEvent *event) override;

    virtual bool onMousePressEvent(TpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;

    virtual bool onMouseMoveEvent(TpMouseEvent *event) override;

    virtual bool onResizeEvent(TpObjectResizeEvent *event) override;

    virtual bool onLeaveEvent(TpObjectLeaveEvent *event) override;

private:
    // 清理所有应用
    void slotClearAllApp(bool);

    // 单个应用点击关闭事件
    void slotKillApp(int32_t pid);

    // 打开指定应用
    void slotOpenApp(int32_t pid);

private:
    TpScrollPanel *taskScrollPanel_;

    TpPoint mousePressPoint_;
    
    uint32_t taskHeight_;
    uint32_t taskWidth_;

    TpButton *clearAllBtn_;

    // <pid, 应用预览窗>
    TpHash<int32_t, appPreviewWidget *> allTaskWidgetMap_;
};

#endif