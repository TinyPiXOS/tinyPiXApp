cmake_minimum_required(VERSION 3.20.0)
project(fileManagement VERSION 1.0.0 LANGUAGES C CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include(CTest)
# enable_testing()

# 设置构建生成的根目录
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

# 设置不同类型的目标文件的输出目录
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

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
    ${SOURCE_PATH}/mainWindowService.cpp
    ${SOURCE_PATH}/sonWindow/recentlyWindow.cpp
    ${SOURCE_PATH}/sonWindow/browseWindow.cpp
    ${SOURCE_PATH}/toolWindow/diskDeviceCheckBox.cpp
    ${SOURCE_PATH}/toolWindow/usbPopButton.cpp
    ${SOURCE_PATH}/browserToolWindow/fileListWindow.cpp
    ${SOURCE_PATH}/browserToolWindow/settingWindow.cpp
    ${SOURCE_PATH}/browserToolWindow/listFileItem.cpp
    ${SOURCE_PATH}/recentlyToolWindow/clearSpaceWindow.cpp
    ${SOURCE_PATH}/recentlyToolWindow/pressPopWindow.cpp
)

include_directories(${SOURCE_PATH})
include_directories(${SOURCE_PATH}/sonWindow)
include_directories(/usr/include)
include_directories(/usr/include/PiXWM)
include_directories(/usr/include/tinyPiX)
include_directories(/usr/include/tinyPiX/TpUtils)
include_directories(/usr/include/tinyPiX/TpExtUtils)
include_directories(/usr/include/tinyPiX/TpGui)
include_directories(/usr/include/tinyPiX/TpGui/Core)
include_directories(/usr/include/tinyPiX/TpGui/Screen)
include_directories(/usr/include/tinyPiX/TpGui/Widgets)

add_executable(fileManagement ${SOURCE_FILES})

# 将pthread库链接到可执行目标
target_link_libraries(fileManagement    PiXApi TpGui TpUtils TpExtUtils 
                )

# 包含共享模块
include(../../cmake/SetupAppInstallation.cmake)

# 初始化应用安装信息
setup_app_installation("6e3aaf4e-7a62-46d7-172d-e244038c4ae7")