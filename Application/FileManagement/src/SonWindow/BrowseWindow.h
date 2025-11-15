
/*
    浏览
*/

#ifndef __BROWSE_WINDOW_H
#define __BROWSE_WINDOW_H

#include "TpWidget.h"
#include "TpScrollPanel.h"
#include "TpLineEdit.h"
#include "TpVector.h"
#include "TpMediaTileButton.h"
#include "TpMenuPanelWidget.h"
#include "TpEvent.h"
#include "SystemInfo/TpDiskManage.h"

#include "ToolWindow/DiskDeviceCheckBox.h"
#include "BrowserToolWindow/FileListWindow.h"

class BrowseWindow
    : public TpWidget
{
public:
    BrowseWindow(TpWidget *parent = nullptr);
    ~BrowseWindow();

	virtual void setVisible(bool visible = true) override;

protected:
    virtual bool onPaintEvent(TpPaintEvent *event) override;

private:
    void init();

    // 快捷访问按钮点击事件
    void fastPathBtnClicked(TpMediaTileButton* clickBtn);

    // 设备节点点击
    void devicePathBtnClicked(DiskDeviceCheckBox* clickBtn);

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
    FileListWindow *fileListWindow_;

    // 所有USB设备列表
    TpDiskManage* diskManager_;
    TpVector<DiskDeviceCheckBox *> deviceList_;
};

#endif