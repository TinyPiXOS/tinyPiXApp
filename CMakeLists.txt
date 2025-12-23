cmake_minimum_required(VERSION 3.20.0)
project(tinyPiXApp VERSION 1.0.0 LANGUAGES C CXX)

# 添加SDK路径选项，支持命令行传入
set(DEFAULT_SDK_PATH "/home/hawk/Public/arm_64")
set(SDK_PATH ${DEFAULT_SDK_PATH})
set(ARM_SDK_PATH "${SDK_PATH}" CACHE PATH "交叉编译SDK路径")

include(cmake/LoadCrossSDK.cmake)

# 加载SDK
LoadCrossSDK(${ARM_SDK_PATH})

# 显示使用的SDK路径
message(STATUS "使用 SDK 路径: ${ARM_SDK_PATH}")

# 应用配置文件；源码安装可手动配置并安装该json；使用安装包安装会自动生成Json
set(INSTALL_RUN_APP_CONFIG_DIR "/System/conf/app")

# 添加子目录
add_subdirectory(Application/DeskTop)
add_subdirectory(Application/FileManagement)
add_subdirectory(Application/SystemSettings)
add_subdirectory(Application/MediaPlayer)
add_subdirectory(Application/Calendar)
add_subdirectory(Application/PhotoAlbum)

add_subdirectory(ComDemoApplication/AnimationDemo)
add_subdirectory(ComDemoApplication/ComboxDemo)
add_subdirectory(ComDemoApplication/BatteryDemo)
add_subdirectory(ComDemoApplication/SliderDemo)
add_subdirectory(ComDemoApplication/CheckBoxDemo)
add_subdirectory(ComDemoApplication/ProgressBarDemo)
add_subdirectory(ComDemoApplication/ButtonDemo)
add_subdirectory(ComDemoApplication/FlexLayoutDemo)
add_subdirectory(ComDemoApplication/LabelDemo)
