
/*
    浏览
*/

#ifndef __BROWSE_WINDOW_H
#define __BROWSE_WINDOW_H

#include "tpChildWidget.h"
#include "tpScrollPanel.h"
#include "tpLineEdit.h"
#include "tpVector.h"
#include "tpMediaTileButton.h"
#include "tpMenuPanelWidget.h"
#include "tpEvent.h"
#include "tpDiskManage.h"

#include "toolWindow/diskDeviceCheckBox.h"
#include "browserToolWindow/fileListWindow.h"

class browseWindow
    : public tpChildWidget
{
public:
    browseWindow(tpChildWidget *parent = nullptr);
    ~browseWindow();

	virtual void setVisible(bool visible = true) override;

protected:
    virtual bool onPaintEvent(tpObjectPaintEvent *event) override;

private:
    void init();

    // 快捷访问按钮点击事件
    void fastPathBtnClicked(tpMediaTileButton* clickBtn);

    // 设备节点点击
    void devicePathBtnClicked(diskDeviceCheckBox* clickBtn);

    // 数据来源路径切换
    void sourceMenuChanged(tpMenuPanelItem* sourceItem);

private:
    // 主滚动显示区域
    tpScrollPanel *mainScrollPanel_;

    // 搜索框
    tpLineEdit *searchEdit_;

    // 快捷访问图标滚动区域
    tpScrollPanel *fastPathScroll_;

    // 快捷访问按钮
    tpMediaTileButton *pictureTileBtn_;
    tpMediaTileButton *videoTileBtn_;
    tpMediaTileButton *textTileBtn_;
    tpMediaTileButton *musicTileBtn_;
    tpMediaTileButton *applicationTileBtn_;
    
    tpVector<tpMediaTileButton*> mediaBtnList_;

    // 数据来源选择菜单
    tpMenuPanelWidget *menuPanelWidget_;

    // 文件列表
    fileListWindow *fileListWindow_;

    // 所有USB设备列表
    tpDiskManage* diskManager_;
    tpVector<diskDeviceCheckBox *> deviceList_;
};

#endif