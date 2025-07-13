cmake_minimum_required(VERSION 3.5.0)
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
    ${SOURCE_PATH}/deskEntry.cpp
    ${SOURCE_PATH}/deskScreen.cpp
    ${SOURCE_PATH}/mainAppScrollPanel.cpp
    ${SOURCE_PATH}/toolBar/topBar.cpp
    ${SOURCE_PATH}/toolBar/bottomBar.cpp
    ${SOURCE_PATH}/toolBar/appSettingBar.cpp
    ${SOURCE_PATH}/toolBar/powerManage.cpp
    ${SOURCE_PATH}/toolBar/navigationBar.cpp
    ${SOURCE_PATH}/toolWindow/sysLockWindow.cpp
    ${SOURCE_PATH}/toolWindow/appTaskManageWindow.cpp
    ${SOURCE_PATH}/toolWindow/appOperateMaskWindow.cpp
    ${SOURCE_PATH}/toolWindow/appPreviewWidget.cpp
    ${SOURCE_PATH}/toolModule/caculateAppPos.cpp
    ${SOURCE_PATH}/toolModule/desktopAppButton.cpp
)

include_directories(${SOURCE_PATH})
include_directories(/usr/include)
include_directories(/usr/include/PiXWM)
include_directories(/usr/include/tinyPiX)
include_directories(/usr/include/tinyPiX/Api)
include_directories(/usr/include/tinyPiX/Utils)
include_directories(/usr/include/tinyPiX/ExternUtils)
include_directories(/usr/include/tinyPiX/SingleGUI)
include_directories(/usr/include/tinyPiX/SingleGUI/core)
include_directories(/usr/include/tinyPiX/SingleGUI/screen)
include_directories(/usr/include/tinyPiX/SingleGUI/widgets)
include_directories(/usr/include/PiXWM)
include_directories(/usr/include/SDL2)
include_directories(/usr/include/freetype2)
include_directories(/usr/include/cairo)
include_directories(/usr/include/pango-1.0)
include_directories(/usr/include/harfbuzz)
include_directories(/usr/include/glib-2.0)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include)

# 添加动态库目标
# add_library(PiXDesktop SHARED ${SOURCE_FILES})
add_executable(PiXDesktop ${SOURCE_FILES})

# 设置动态库版本 VERSION 是库的当前版本号，而 SOVERSION 是API的兼容性版本号。
# set_target_properties(PiXDesktop PROPERTIES VERSION 1.0 SOVERSION 1)

# 指定依赖动态库目录
link_directories("/usr/lib")

# 将pthread库链接到可执行目标
target_link_libraries(PiXDesktop dl pthread PiXApi PiXSingleGUI PiXUtils PiXExternUtils SDL2 SDL2_image SDL2_gfx cairo pango-1.0 gobject-2.0
    glib-2.0
    pangocairo-1.0
    fontconfig
    freetype
    gbm
    GLESv2
    EGL
)
# 包含共享模块
include(../../cmake/SetupAppInstallation.cmake)

# 初始化应用安装信息
setup_app_installation("desk0000-0000-0000-0000-0000000000000")
