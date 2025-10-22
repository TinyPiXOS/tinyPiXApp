#ifndef __MAIN_WINDOW_SERVICE_H
#define __MAIN_WINDOW_SERVICE_H

#include "TpFixScreen.h"
#include "TpScrollPanel.h"
#include "TpLineEdit.h"
#include "TpMenuPanelWidget.h"
#include "TpVBoxLayout.h"
#include "SystemSettingsGlobal.h"
#include "TpButton.h"
#include "TpLabel.h"
#include "SettingWindow/SettingBase.h"

class MainWindowService
    : public TpFixScreen
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
    void slotClickMenuItem(TpMenuPanelItem *deviceBtn);

    // 点击右侧窗体后退按钮
    void slotClickBackBtn(bool);

private:
    void initUi();

    // 创建设置的所有一级菜单并添加至布局
    void createAllSettingTopMenu(TpVBoxLayout *menuLayout);

    // 根据设置类型枚举获取设置对应的中文名
    TpString generalSettingNames(const SettingType &type);

    // 根据设置类型生成item的自定义widget；如果类型对应的不需要widget返回空指针
    TpChildWidget *generalCustomWidget(const SettingType &type);

    // 创建所有功能子窗体；放入allMenuWidgetMapper_容器
    void createAllSettingWindow();

    // 刷新顶级菜单的状态；热点开关状态；wifi连接状态等
    void refreshTopMenuStatus();

private:
    // 菜单滚动窗体
    TpScrollPanel *mainScrollPanel_;

    // 右侧设置具体菜单画布；根据选择菜单不同显示不同设置窗体
    TpScrollPanel *settingWindow_;

    // 菜单搜索框
    TpLineEdit *searchEdit_;

    // 右侧功能窗口标题
    TpLabel *subTitleLabel_;
    TpButton* subBackBtn_;

    // 当前选中的顶级菜单
    TpMenuPanelItem* curSelectItem_;

    // 所有菜单panel列表
    TpVector<TpMenuPanelWidget *> allMenuPanelList_;

    // 所有设置菜单对应的Item
    TpHash<SettingType, TpMenuPanelItem *> allMenuItemMapper_;

    // 设置菜单对应的窗体指针
    TpHash<SettingType, SettingBase *> allMenuWidgetMapper_;

};

#endif