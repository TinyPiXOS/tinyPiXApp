
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

#include "ToolWindow/DiskDeviceCheckBox.h"
#include "RecentlyToolWindow/ClearSpaceWindow.h"
#include "RecentlyToolWindow/PressPopWindow.h"
#include "TpCollapsibleFileWidget.h"

class RecentlyWindow
    : public TpChildWidget
{
public:
    RecentlyWindow(TpChildWidget *parent = nullptr);
    ~RecentlyWindow();

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
    PressPopWindow *pressPopMenu_;

    // 搜索框
    TpLineEdit *searchEdit_;

    TpButton *clearButton_;
    ClearSpaceWindow *clearWindow_;

    TpVector<DiskDeviceCheckBox *> deviceList_;

    TpVector<TpCollapsibleFileWidget*> fileWidgetList_;
};

#endif