#ifndef __SETTING_BASE_H
#define __SETTING_BASE_H

#include "tpChildWidget.h"
#include "systemSettingsGlobal.h"
#include "tpLabel.h"
#include "tpButton.h"
#include "tpVBoxLayout.h"

class settingBase
    : public tpChildWidget
{
public:
    settingBase(tpChildWidget *parent = nullptr);
    virtual ~settingBase();

    // 顶层窗口设置功能窗口标题和后退指针
    void setTitleBackInfo(tpLabel *titleLabel, tpButton *backBtn);

    // 功能窗口进入自己的子菜单；切换标题
    void setSubTile(const tpString &subTitle);

    // 设置后退按钮是否显隐
    void setBackBtnVisible(const bool &visible);

    // 窗口显示时刷新数据;子类重写该函数
    virtual void refreshData() {}

public:
    // 后退按钮点击事件；子类重写该事件捕获后退事件
    virtual void slotBack();

protected:
    // 所有子类使用此布局进行界面构建；确保使用统一的Margin
    tpVBoxLayout *mainLayout_;

private:
    // 使用函数设置；不要直接访问这两个指针
    tpLabel *titleLabel_;
    tpButton *backBtn_;
};

#endif