cmake_minimum_required(VERSION 3.5.0)
project(tinyPiXApp VERSION 1.0.0 LANGUAGES C CXX)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i686|i386|AMD64")
    message(STATUS "============X86 平台============")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
    message(STATUS "============ARM 平台============")
endif()

# 应用配置文件；源码安装可手动配置并安装该json；使用安装包安装会自动生成Json
set(INSTALL_RUN_APP_CONFIG_DIR "/System/conf/app")

# 添加子目录
add_subdirectory(Application/deskTop)
add_subdirectory(Application/fileManagement)
add_subdirectory(Application/systemSettings)
add_subdirectory(Application/mediaPlayer)
add_subdirectory(Application/calendar)
add_subdirectory(Application/photoAlbum)

add_subdirectory(ComDemoApplication/animationDemo)
add_subdirectory(ComDemoApplication/comboxDemo)
add_subdirectory(ComDemoApplication/batteryDemo)
add_subdirectory(ComDemoApplication/sliderDemo)
add_subdirectory(ComDemoApplication/checkBoxDemo)
add_subdirectory(ComDemoApplication/progressBarDemo)
add_subdirectory(ComDemoApplication/buttonDemo)
add_subdirectory(ComDemoApplication/flexLayoutDemo)
add_subdirectory(ComDemoApplication/labelDemo)


