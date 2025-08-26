
/*
    浏览-功能设置窗口
*/

#ifndef __SETTING_WINDOW_H
#define __SETTING_WINDOW_H

#include "TpDialog.h"
#include "TpScrollPanel.h"
#include "TpEvent.h"
#include "TpButton.h"
#include "TpListCheckBoxGroup.h"
#include "TpComBox.h"
#include "TpLabel.h"

class settingWindow
    : public TpDialog
{
public:
    settingWindow();
    ~settingWindow();

    void setVisible(bool visible = true) override;

public
signals:
    /// @brief 显示视图切换事件
    /// @param uint32_t 切换的视图索引，0视图模式，1列表模式
    declare_signal(onChangeViewMode, uint32_t);

protected:
    virtual bool onPaintEvent(TpObjectPaintEvent *event) override;
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;

private:
    void init();

    TpLabel *createContentLabel(const TpString &text);

private:
    // 主滚动显示区域
    TpScrollPanel *mainScrollPanel_;

    TpButton *closeBtn_;

    // 视图选择下拉框
    TpComBox *viewCbx_;

    // 筛选条件单选组
    TpListCheckBoxGroup *filterCbxGroup_;

    // 排序条件单选组
    TpListCheckBoxGroup *sortCbxGroup_;
};

#endif