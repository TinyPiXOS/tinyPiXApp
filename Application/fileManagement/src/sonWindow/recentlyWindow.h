
/*
    最近
*/

#ifndef __RECENTLY_WINDOW_H
#define __RECENTLY_WINDOW_H

#include "tpChildWidget.h"
#include "tpScrollPanel.h"
#include "tpLineEdit.h"
#include "tpVector.h"
#include "tpVBoxLayout.h"
#include "tpButton.h"
#include "tpDialog.h"

#include "toolWindow/diskDeviceCheckBox.h"
#include "recentlyToolWindow/clearSpaceWindow.h"
#include "recentlyToolWindow/pressPopWindow.h"
#include "tpCollapsibleFileWidget.h"

class recentlyWindow
    : public tpChildWidget
{
public:
    recentlyWindow(tpChildWidget *parent = nullptr);
    ~recentlyWindow();

protected:
    virtual bool onMousePressEvent(tpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;
    virtual bool eventFilter(tpObject *watched, tpEvent *event) override;

private:
    // 长按文件列表
    void SlotLongPressItem(tpCollapsibleFileWidget* fileWidget);

private:
    void init();

    // 创建最近访问文件列表信息
    void createRecentlyFileList(tpVBoxLayout *scrollLayout);

private:
    // 滚动显示区域
    tpScrollPanel *mainScrollPanel_;

    // 长按item弹出菜单
    pressPopWindow *pressPopMenu_;

    // 搜索框
    tpLineEdit *searchEdit_;

    tpButton *clearButton_;
    clearSpaceWindow *clearWindow_;

    tpVector<diskDeviceCheckBox *> deviceList_;

    tpVector<tpCollapsibleFileWidget*> fileWidgetList_;
};

#endif