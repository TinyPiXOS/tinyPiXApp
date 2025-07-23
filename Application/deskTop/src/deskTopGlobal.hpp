#ifndef DESKTOP_GLOBAL_HEADER
#define DESKTOP_GLOBAL_HEADER

#include <cstdint>
#include "toolWindow/sysLockWindow.h"
#include "toolWindow/appTaskManageWindow.h"
#include "toolBar/appSettingBar.h"
#include "tpDisplay.h"
#include "tinyPiXUtils.h"
#include "tinyPiXSys.h"

// 桌面最大页数
const uint32_t globalDesktopMaxPageNum = 10;

// 系统锁屏状态
extern bool globalSystemLockStatus;

// app任务管理窗
extern appTaskManageWindow *globalAppTaskWindow;

// 锁屏窗口
extern sysLockWindow *globalSysLockWindow;

// 下拉菜单
extern appSettingBar *globalTopSettingBar_;

// 主窗体
extern tpScreen *globalMainScreen_;

extern IPiSysApiAgent *globalAgent;

// 正在运行的应用信息
struct RunAppInfo
{
    tpString appName;
    tpString appUuid;
    tpString appIconPath;
    int32_t pid;

    RunAppInfo() : appName(""), appUuid(""), appIconPath(""), pid(0)
    {
    }
};
extern std::mutex readRunAppMutex_;
// <pid, 应用信息>
extern tpHash<int32_t, RunAppInfo> globalRunAppMap_;
// 应用uuid对应的pid
extern tpHash<tpString, int32_t> globalUuidPidMap_;

// APP页码信息
struct ApplicationInfo
{
    tpString appUuid;
    uint32_t page;
    uint32_t index;

    ApplicationInfo() : appUuid(""), page(0), index(0)
    {
    }
};
typedef std::shared_ptr<ApplicationInfo> ApplicationInfoSPtr;
#endif

#ifndef BOTTOM_BAR_WIDTH
#define BOTTOM_BAR_WIDTH globalMainScreen_->screenWidth() * 0.7777
#endif

#ifndef BOTTOM_BAR_HEIGHT
#define BOTTOM_BAR_HEIGHT globalMainScreen_->screenHeight() * 0.16
#endif

// 顶部工具栏高度
#ifndef TOP_BAR_HEIGHT
// #define TOP_BAR_HEIGHT tpScreen::screenHeight() * 0.04166
#define TOP_BAR_HEIGHT globalMainScreen_->screenHeight() * 0.05
#endif

// APP显示主窗口距离topbar距离
#ifndef MAIN_PANEL_TOP_DISTANCE
#define MAIN_PANEL_TOP_DISTANCE globalMainScreen_->screenHeight() * 0.027777
#endif

// APP之间水平间隔
// #ifndef APP_H_INTERVAL
// #define APP_H_INTERVAL tpScreen::screenWidth() * 0.06666
// #endif
extern uint32_t globalAppHInterval;

// APP之间垂直间隔
#ifndef APP_V_INTERVAL
#define APP_V_INTERVAL globalMainScreen_->screenHeight() * 0.0527777
#endif

// APP一页最大行列数
extern uint32_t globalAppMaxRow;
extern uint32_t globalAppMaxColumn;

// #ifndef APP_MAX_ROW
// #define APP_MAX_ROW 4
// #endif

// APP一页最大列数
// #ifndef APP_MAX_COLUMN
// #define APP_MAX_COLUMN 6
// #endif

#ifndef APP_FONT_SIZE
#define APP_FONT_SIZE tpDisplay::sp2Px(11)
#endif

// APPIcon的宽高
#ifndef APP_WIDTH_HEIGHT
#define APP_WIDTH_HEIGHT globalMainScreen_->screenHeight() * 0.1275
#endif