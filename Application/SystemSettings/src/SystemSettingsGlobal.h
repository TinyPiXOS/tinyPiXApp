#ifndef __SYSTEM_SETTINGS_GLOBAL_H
#define __SYSTEM_SETTINGS_GLOBAL_H

#include "TpLabel.h"
#include "TpFont.h"

enum SettingType
{
    WLANSetting,        // WLAN
    BlueToothSetting,   // 蓝牙
    InternetSetting,    // 以太网
    HotspotSetting,     // 热点
    WallPaperSetting,   // 壁纸设置
    DisplaySetting,     // 显示设置
    DesktopSetting,     // 桌面设置
    VoiceSetting,       // 声音设置
    DeviceSetting,      // 设备信息
    MoreSetting,        // 更多设置
    ApplicationSetting  // 应用设置
};

// 创建一个分组标题label 
static TpLabel* createGroupNameLabel(const TpString& text)
{
    TpLabel *sourceLabel = new TpLabel(text);
    sourceLabel->font()->setFontSize(11);
    sourceLabel->setFixedHeight(sourceLabel->font()->pixelHeight());
    sourceLabel->font()->setFontColor(_RGB(89, 89, 89));
    return sourceLabel;
}
#endif
