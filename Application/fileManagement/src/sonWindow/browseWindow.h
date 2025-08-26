
/*
    浏览
*/

#ifndef __BROWSE_WINDOW_H
#define __BROWSE_WINDOW_H

#include "TpChildWidget.h"
#include "TpScrollPanel.h"
#include "TpLineEdit.h"
#include "TpVector.h"
#include "TpMediaTileButton.h"
#include "TpMenuPanelWidget.h"
#include "TpEvent.h"
#include "TpDiskManage.h"

#include "toolWindow/diskDeviceCheckBox.h"
#include "browserToolWindow/fileListWindow.h"

class browseWindow
    : public TpChildWidget
{
public:
    browseWindow(TpChildWidget *parent = nullptr);
    ~browseWindow();

	virtual void setVisible(bool visible = true) override;

protected:
    virtual bool onPaintEvent(TpObjectPaintEvent *event) override;

private:
    void init();

    // 快捷访问按钮点击事件
    void fastPathBtnClicked(TpMediaTileButton* clickBtn);

    // 设备节点点击
    void devicePathBtnClicked(diskDeviceCheckBox* clickBtn);

    // 数据来源路径切换
    void sourceMenuChanged(TpMenuPanelItem* sourceItem);

private:
    // 主滚动显示区域
    TpScrollPanel *mainScrollPanel_;

    // 搜索框
    TpLineEdit *searchEdit_;

    // 快捷访问图标滚动区域
    TpScrollPanel *fastPathScroll_;

    // 快捷访问按钮
    TpMediaTileButton *pictureTileBtn_;
    TpMediaTileButton *videoTileBtn_;
    TpMediaTileButton *textTileBtn_;
    TpMediaTileButton *musicTileBtn_;
    TpMediaTileButton *applicationTileBtn_;
    
    TpVector<TpMediaTileButton*> mediaBtnList_;

    // 数据来源选择菜单
    TpMenuPanelWidget *menuPanelWidget_;

    // 文件列表
    fileListWindow *fileListWindow_;

    // 所有USB设备列表
    TpDiskManage* diskManager_;
    TpVector<diskDeviceCheckBox *> deviceList_;
};

#endif