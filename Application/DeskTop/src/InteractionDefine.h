#ifndef DESKTOP_INTERACTION_DEFINE_HEADER
#define DESKTOP_INTERACTION_DEFINE_HEADER

/*
 *  桌面与应用交互协议；慎重修改； 修改需与TinyCore中，应用接收桌面部分统一修改！
 */

#include <cstdlib>
#include "TpString.h"

// 应用上线标识;应用启动时发送该主题；桌面会通知应用状态栏信息
const static TpString DeskApplicationRunTopic = "DeskApplicationRunTopicConfig";
/// @brief 桌面发布状态栏信息主题
const static TpString DeskStatusBarInfoTopic = "DeskStatusBarConfig";
/// @brief 读取桌面状态栏信息;慎重修改，需和桌面保持协议一致
struct DeskStatusBarInfo
{
    /// @brief 状态栏位置；0=上，1=右，2=下，3=左，其它值=上
    int32_t statusBarLocation;
    /// @brief 状态栏宽度值
    int32_t statusBarWidth;
    /// @brief 顶状态栏高度值
    int32_t statusBarHeight;
    /// @brief 状态栏是否显示；true显示，false隐藏
    bool statusBarVislble;

    DeskStatusBarInfo() : statusBarLocation(0), statusBarWidth(0), statusBarHeight(0), statusBarVislble(false)
    {
    }

    virtual ~DeskStatusBarInfo() {}

    bool operator==(const DeskStatusBarInfo &others)
    {
        return (statusBarLocation == others.statusBarLocation) &&
               (statusBarWidth == others.statusBarWidth) &&
               (statusBarHeight == others.statusBarHeight) &&
               (statusBarVislble == others.statusBarVislble);
    }
};

#endif
