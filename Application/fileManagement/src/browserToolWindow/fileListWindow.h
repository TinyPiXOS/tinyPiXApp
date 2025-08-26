
/*
    浏览-文件列表窗口
*/

#ifndef __FILE_LIST_WINDOW_H
#define __FILE_LIST_WINDOW_H

#include "TpChildWidget.h"
#include "TpScrollPanel.h"
#include "TpVector.h"
#include "TpCollapsibleFileItem.h"
#include "TpFlexLayout.h"
#include "TpButton.h"
#include "TpFilePathWidget.h"
#include "browserToolWindow/settingWindow.h"
#include "browserToolWindow/listFileItem.h"
#include "toolWindow/usbPopButton.h"
#include "TpVBoxLayout.h"
#include "TpImageWidget.h"

class fileListWindow
    : public TpScrollPanel
{
public:
    enum FileListMode
    {
        Grid, // 网格视图模式
        List  // 列表模式
    };

    enum DeviceType
    {
        USBDevice,  // USB设备
        LocalDevice // 本地设备
    };

public:
    fileListWindow(TpChildWidget *parent = nullptr);
    ~fileListWindow();

    // 设置显示模式
    void setShowMode(const FileListMode &mode);

    // 设置显示目录，根目录
    void setRootPath(const TpString &path);

    // 设置显示的路径的设备类型
    void setDeviceType(const DeviceType &deviceType);

    // 指定路径刷新UI；必须在根目录下
    void refreshPath(const TpString &path);

private:
    // Item点击槽函数
    void SlotClickFileItem(TpCollapsibleFileItem *item);

    // 点击列表Item
    void SlotClickListFileItem(listFileItem *item);

    // 路径后退
    void SlotBackPath();

    // 点击功能设置按钮
    void SlotShowSetting(bool);

    // 弹出USB设备
    void SlotPopUsb(bool);

private:
    void init();

    // 指定文件路径打开文件
    void openFile(const TpString &filePath);

private:
    // 主布局
    TpVBoxLayout *mainLayout_;

    FileListMode mode_;
    DeviceType deviceType_;

    // USB设备弹出按钮
    usbPopButton *popBtn_;

    TpString rootPath_;
    TpString curShowPath_;

    // 网格模式的布局
    TpFlexLayout *gridFileLayout_;

    // 列表模式文件布局
    TpVBoxLayout *listFileLayout_;

    TpFilePathWidget *filePathWidget_;

    // 路径回退按钮
    TpButton *pathBackBtn_;
    TpButton *settingBtn_;

    // 功能设置窗口
    settingWindow *settingWindow_;

    // 图片预览窗口
    TpImageWidget *imagePreviewWidget_;
};

#endif