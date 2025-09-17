cmake_minimum_required(VERSION 3.20.0)
project(flexLayoutDemo VERSION 1.0.0 LANGUAGES C CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置构建生成的根目录
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# 添加源码目录
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

# 指定动态库目录
link_directories("/usr/lib")

# 定义源码路径宏
set(SOURCE_FILES
    ${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/mainWindowService.cpp
)

include_directories(${SOURCE_PATH})
include_directories(/usr/include)
include_directories(/usr/include/PiXWM)
include_directories(/usr/include/tinyPiX)
include_directories(/usr/include/tinyPiX/TpUtils)
include_directories(/usr/include/tinyPiX/TpExtUtils)
include_directories(/usr/include/tinyPiX/TpGui)
include_directories(/usr/include/tinyPiX/TpGui/Core)
include_directories(/usr/include/tinyPiX/TpGui/Screen)
include_directories(/usr/include/tinyPiX/TpGui/Widgets)

add_executable(flexLayoutDemo ${SOURCE_FILES})

# 将pthread库链接到可执行目标
target_link_libraries(flexLayoutDemo pthread dl pthread PiXApi TpGui TpUtils TpExtUtils 
                 )

# 包含共享模块
include(../../cmake/SetupAppInstallation.cmake)

# 初始化应用安装信息
setup_app_installation("ec71147b-b8c7-8db7-b46b-916a5c30b2b9")