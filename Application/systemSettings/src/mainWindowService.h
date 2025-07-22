#ifndef __MAIN_WINDOW_SERVICE_H
#define __MAIN_WINDOW_SERVICE_H

#include "tpFixScreen.h"
#include "tpScrollPanel.h"
#include "tpLineEdit.h"
#include "tpMenuPanelWidget.h"
#include "tpVBoxLayout.h"
#include "systemSettingsGlobal.h"
#include "tpButton.h"
#include "tpLabel.h"
#include "settingWindow/settingBase.h"

class mainWindowService
    : public tpFixScreen
{
public:
    mainWindowService();
    virtual ~mainWindowService();

public:
    virtual bool appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require) override;

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

    virtual bool onActiveEvent(tpObjectActiveEvent *event) override;

private:
    void slotClickMenuItem(tpMenuPanelItem *deviceBtn);

    // 点击右侧窗体后退按钮
    void slotClickBackBtn(bool);

private:
    void initUi();

    // 创建设置的所有一级菜单并添加至布局
    void createAllSettingTopMenu(tpVBoxLayout *menuLayout);

    // 根据设置类型枚举获取设置对应的中文名
    tpString generalSettingNames(const SettingType &type);

    // 根据设置类型生成item的自定义widget；如果类型对应的不需要widget返回空指针
    tpChildWidget *generalCustomWidget(const SettingType &type);

    // 创建所有功能子窗体；放入allMenuWidgetMapper_容器
    void createAllSettingWindow();

    // 刷新顶级菜单的状态；热点开关状态；wifi连接状态等
    void refreshTopMenuStatus();

private:
    // 菜单滚动窗体
    tpScrollPanel *mainScrollPanel_;

    // 右侧设置具体菜单画布；根据选择菜单不同显示不同设置窗体
    tpScrollPanel *settingWindow_;

    // 菜单搜索框
    tpLineEdit *searchEdit_;

    // 右侧功能窗口标题
    tpLabel *subTitleLabel_;
    tpButton* subBackBtn_;

    // 当前选中的顶级菜单
    tpMenuPanelItem* curSelectItem_;

    // 所有菜单panel列表
    tpVector<tpMenuPanelWidget *> allMenuPanelList_;

    // 所有设置菜单对应的Item
    tpHash<SettingType, tpMenuPanelItem *> allMenuItemMapper_;

    // 设置菜单对应的窗体指针
    tpHash<SettingType, settingBase *> allMenuWidgetMapper_;

};

#endif