
/*
    浏览-功能设置窗口
*/

#ifndef __SETTING_WINDOW_H
#define __SETTING_WINDOW_H

#include "tpDialog.h"
#include "tpScrollPanel.h"
#include "tpEvent.h"
#include "tpButton.h"
#include "tpListCheckBoxGroup.h"
#include "tpComBox.h"
#include "tpLabel.h"

class settingWindow
    : public tpDialog
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
    virtual bool onPaintEvent(tpObjectPaintEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;

private:
    void init();

    tpLabel *createContentLabel(const tpString &text);

private:
    // 主滚动显示区域
    tpScrollPanel *mainScrollPanel_;

    tpButton *closeBtn_;

    // 视图选择下拉框
    tpComBox *viewCbx_;

    // 筛选条件单选组
    tpListCheckBoxGroup *filterCbxGroup_;

    // 排序条件单选组
    tpListCheckBoxGroup *sortCbxGroup_;
};

#endif