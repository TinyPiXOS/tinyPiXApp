#ifndef DESKTOP_INTERACTION_DEFINE_HEADER
#define DESKTOP_INTERACTION_DEFINE_HEADER

/*
 *  桌面与应用交互协议；慎重修改； 修改需与TinyCore中，应用接收桌面部分统一修改！
 */

#include <cstdlib>
#include "TpString.h"

// 应用上线标识
const static TpString ApplicationRunTopic = "ApplicationRunTopicConfig";
/// @brief 桌面工具栏信息
const static TpString DeskTopBarInfoTopic = "DeskTopBarConfig";
struct DeskTopBarInfo
{
    /// @brief 顶部工具栏宽度值
    int32_t topBarWidth;
    /// @brief 顶部工具栏高度值
    int32_t topBarHeight;
    /// @brief 顶部工具栏是否显示；true显示，false隐藏
    bool topBarisVislble;

    DeskTopBarInfo() : topBarWidth(0), topBarHeight(0), topBarisVislble(false)
    {
    }

    virtual ~DeskTopBarInfo() {}
};

#endif
