cmake_minimum_required(VERSION 3.20.0)
project(PiXDesktop VERSION 1.0.0 LANGUAGES C CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置构建生成的根目录
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

# 设置不同类型的目标文件的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# 添加源码目录
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

# 定义源码路径宏
set(SOURCE_FILES
    ${SOURCE_PATH}/DeskEntry.cpp
    ${SOURCE_PATH}/DeskScreen.cpp
    ${SOURCE_PATH}/ToolBar/TopBar.cpp
    ${SOURCE_PATH}/ToolBar/BottomBar.cpp
    ${SOURCE_PATH}/ToolBar/AppSettingBar.cpp
    ${SOURCE_PATH}/ToolBar/PowerManage.cpp
    ${SOURCE_PATH}/ToolBar/NavigationBar.cpp
    ${SOURCE_PATH}/ToolWindow/SysLockWindow.cpp
    ${SOURCE_PATH}/ToolWindow/AppTaskManageWindow.cpp
    ${SOURCE_PATH}/ToolWindow/AppOperateMaskWindow.cpp
    ${SOURCE_PATH}/ToolWindow/AppPreviewWidget.cpp
    ${SOURCE_PATH}/ToolModule/MainAppScrollPanel.cpp
    ${SOURCE_PATH}/ToolModule/CaculateAppPos.cpp
    ${SOURCE_PATH}/ToolModule/DesktopAppButton.cpp
)

include_directories(${SOURCE_PATH})
include_directories(${SOURCE_PATH}/ToolBar)
include_directories(${SOURCE_PATH}/ToolModule)
include_directories(${SOURCE_PATH}/ToolWindow)
include_directories(/usr/include)
include_directories(/usr/include/TpWM)
include_directories(/usr/include/TinyPiX)
include_directories(/usr/include/TinyPiX/TpUtils)
include_directories(/usr/include/TinyPiX/TpExtUtils)
include_directories(/usr/include/TinyPiX/TpGUI)
include_directories(/usr/include/TinyPiX/TpGUI/Core)
include_directories(/usr/include/TinyPiX/TpGUI/Screen)
include_directories(/usr/include/TinyPiX/TpGUI/Widgets)

# 添加动态库目标
# add_library(PiXDesktop SHARED ${SOURCE_FILES})
add_executable(PiXDesktop ${SOURCE_FILES})

# 设置动态库版本 VERSION 是库的当前版本号，而 SOVERSION 是API的兼容性版本号。
# set_target_properties(PiXDesktop PROPERTIES VERSION 1.0 SOVERSION 1)

# 指定依赖动态库目录
link_directories("/usr/lib")

# 将pthread库链接到可执行目标
target_link_libraries(PiXDesktop TpWMEngine TpGUI TpUtils TpExtUtils)

# 包含共享模块
include(../../cmake/SetupAppInstallation.cmake)

# 初始化应用安装信息
setup_app_installation("desk0000-0000-0000-0000-0000000000000")
