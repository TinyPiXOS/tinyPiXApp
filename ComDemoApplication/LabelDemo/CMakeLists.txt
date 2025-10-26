cmake_minimum_required(VERSION 3.20.0)
project(LabelDemo VERSION 1.0.0 LANGUAGES C CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置构建生成的根目录
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# 为 Debug 和 Release 设置不同的输出目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")

# 添加源码目录
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

# 指定动态库目录
link_directories("/usr/lib")

# 定义源码路径宏
set(SOURCE_FILES
    ${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/MainWindowService.cpp
)

include_directories(${SOURCE_PATH})
include_directories(/usr/include)
include_directories(/usr/include/TinyPiX/TpWM)
include_directories(/usr/include/TinyPiX)
include_directories(/usr/include/TinyPiX/TpUtils)
include_directories(/usr/include/TinyPiX/TpExtUtils)
include_directories(/usr/include/TinyPiX/TpGUI)
include_directories(/usr/include/TinyPiX/TpGUI/Core)
include_directories(/usr/include/TinyPiX/TpGUI/Screen)
include_directories(/usr/include/TinyPiX/TpGUI/Widgets)

add_executable(LabelDemo ${SOURCE_FILES})

# 将pthread库链接到可执行目标
target_link_libraries(LabelDemo    TpWMEngine TpGUI TpUtils TpExtUtils 
                 )

# 包含共享模块
include(../../cmake/SetupAppInstallation.cmake)

# 初始化应用安装信息
setup_app_installation("fb1412cf-84de-4138-b401-215b6e9b1c11")