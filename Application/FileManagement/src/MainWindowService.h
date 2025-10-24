#ifndef __MAIN_WINDOW_SERVICE_H
#define __MAIN_WINDOW_SERVICE_H

#include "TpMainWindow.h"
#include "TpTabWidget.h"
#include "SonWindow/RecentlyWindow.h"
#include "SonWindow/BrowseWindow.h"

class MainWindowService
    : public TpMainWindow
{
public:
    MainWindowService();
    virtual ~MainWindowService();

public:
    virtual bool appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require) override;

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;

    virtual bool onActiveEvent(TpActiveEvent *event) override;

private:
    // 路径切换事件，根据路径切换树节点的选中
    void slotPathChanged(const TpString &path);

private:
    void initUi();

    // 计算所有控件的大小位置
    void caculateRect();

private:
    // titleBar *titleBar_;

    TpTabWidget* mainTabWidget_; 

    // 最近窗口
    RecentlyWindow* recentlyWindow_;

    // 浏览窗口
    BrowseWindow* browseWindow_;
};

#endif