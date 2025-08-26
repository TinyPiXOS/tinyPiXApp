
/*
    最近
*/

#ifndef __RECENTLY_WINDOW_H
#define __RECENTLY_WINDOW_H

#include "TpChildWidget.h"
#include "TpScrollPanel.h"
#include "TpLineEdit.h"
#include "TpVector.h"
#include "TpVBoxLayout.h"
#include "TpButton.h"
#include "TpDialog.h"

#include "toolWindow/diskDeviceCheckBox.h"
#include "recentlyToolWindow/clearSpaceWindow.h"
#include "recentlyToolWindow/pressPopWindow.h"
#include "TpCollapsibleFileWidget.h"

class recentlyWindow
    : public TpChildWidget
{
public:
    recentlyWindow(TpChildWidget *parent = nullptr);
    ~recentlyWindow();

protected:
    virtual bool onMousePressEvent(TpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;
    virtual bool eventFilter(TpObject *watched, TpEvent *event) override;

private:
    // 长按文件列表
    void SlotLongPressItem(TpCollapsibleFileWidget* fileWidget);

private:
    void init();

    // 创建最近访问文件列表信息
    void createRecentlyFileList(TpVBoxLayout *scrollLayout);

private:
    // 滚动显示区域
    TpScrollPanel *mainScrollPanel_;

    // 长按item弹出菜单
    pressPopWindow *pressPopMenu_;

    // 搜索框
    TpLineEdit *searchEdit_;

    TpButton *clearButton_;
    clearSpaceWindow *clearWindow_;

    TpVector<diskDeviceCheckBox *> deviceList_;

    TpVector<TpCollapsibleFileWidget*> fileWidgetList_;
};

#endif