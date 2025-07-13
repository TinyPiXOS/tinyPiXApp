#ifndef __MAIN_WINDOW_SERVICE_H
#define __MAIN_WINDOW_SERVICE_H

#include "tpFixScreen.h"
#include "tpTabWidget.h"
#include "sonWindow/recentlyWindow.h"
#include "sonWindow/browseWindow.h"

class mainWindowService
    : public tpFixScreen
{
public:
    mainWindowService();
    virtual ~mainWindowService();

public:
    virtual bool appChange(int32_t id, int32_t pid, int32_t rotate, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require) override;

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

    virtual bool onRotateEvent(tpObjectRotateEvent *event) override;

    virtual bool onActiveEvent(tpObjectActiveEvent *event) override;

private:
    // 路径切换事件，根据路径切换树节点的选中
    void slotPathChanged(const tpString &path);

private:
    void initUi();

    // 计算所有控件的大小位置
    void caculateRect();

private:
    // titleBar *titleBar_;

    tpTabWidget* mainTabWidget_; 

    // 最近窗口
    recentlyWindow* recentlyWindow_;

    // 浏览窗口
    browseWindow* browseWindow_;
};

#endif