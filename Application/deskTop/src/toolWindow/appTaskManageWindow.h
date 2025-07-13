#ifndef __APP_TASK_MANAGE_WINDOW_H
#define __APP_TASK_MANAGE_WINDOW_H

/*
    应用任务管理器
*/

#include "tpDialog.h"
#include "tpButton.h"
#include "tpEvent.h"
#include "tpScrollPanel.h"
#include "tpVector.h"
#include "toolWindow/appPreviewWidget.h"
#include "tpHash.h"

class appTaskManageWindow
    : public tpDialog
{
public:
    appTaskManageWindow();
    virtual ~appTaskManageWindow();

    virtual void setVisible(bool visible = true) override;

    int32_t getWinIdByPid(const int32_t &pid);

protected:
    virtual bool eventFilter(tpObject *watched, tpEvent *event) override;

    virtual bool onMousePressEvent(tpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;

    virtual bool onMouseMoveEvent(tpMouseEvent *event) override;

    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

    virtual bool onLeaveEvent(tpObjectLeaveEvent *event) override;

private:
    // 清理所有应用
    void slotClearAllApp(bool);

    // 单个应用点击关闭事件
    void slotKillApp(int32_t pid);

    // 打开指定应用
    void slotOpenApp(int32_t pid);

private:
    tpScrollPanel *taskScrollPanel_;

    ItpPoint mousePressPoint_;
    
    uint32_t taskHeight_;
    uint32_t taskWidth_;

    tpButton *clearAllBtn_;

    // <pid, 应用预览窗>
    tpHash<int32_t, appPreviewWidget *> allTaskWidgetMap_;
};

#endif