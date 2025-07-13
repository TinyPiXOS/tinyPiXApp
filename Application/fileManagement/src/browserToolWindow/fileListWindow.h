
/*
    浏览-文件列表窗口
*/

#ifndef __FILE_LIST_WINDOW_H
#define __FILE_LIST_WINDOW_H

#include "tpChildWidget.h"
#include "tpScrollPanel.h"
#include "tpVector.h"
#include "tpCollapsibleFileItem.h"
#include "tpFlexLayout.h"
#include "tpButton.h"
#include "tpFilePathWidget.h"
#include "browserToolWindow/settingWindow.h"
#include "browserToolWindow/listFileItem.h"
#include "toolWindow/usbPopButton.h"
#include "tpVBoxLayout.h"
#include "tpImageWidget.h"

class fileListWindow
    : public tpScrollPanel
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
    fileListWindow(tpChildWidget *parent = nullptr);
    ~fileListWindow();

    // 设置显示模式
    void setShowMode(const FileListMode &mode);

    // 设置显示目录，根目录
    void setRootPath(const tpString &path);

    // 设置显示的路径的设备类型
    void setDeviceType(const DeviceType &deviceType);

    // 指定路径刷新UI；必须在根目录下
    void refreshPath(const tpString &path);

private:
    // Item点击槽函数
    void SlotClickFileItem(tpCollapsibleFileItem *item);

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
    void openFile(const tpString &filePath);

private:
    // 主布局
    tpVBoxLayout *mainLayout_;

    FileListMode mode_;
    DeviceType deviceType_;

    // USB设备弹出按钮
    usbPopButton *popBtn_;

    tpString rootPath_;
    tpString curShowPath_;

    // 网格模式的布局
    tpFlexLayout *gridFileLayout_;

    // 列表模式文件布局
    tpVBoxLayout *listFileLayout_;

    tpFilePathWidget *filePathWidget_;

    // 路径回退按钮
    tpButton *pathBackBtn_;
    tpButton *settingBtn_;

    // 功能设置窗口
    settingWindow *settingWindow_;

    // 图片预览窗口
    tpImageWidget *imagePreviewWidget_;
};

#endif