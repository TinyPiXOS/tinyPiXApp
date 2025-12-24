cmake_minimum_required(VERSION 3.20.0)
project(BatteryDemo VERSION 1.0.0 LANGUAGES C CXX)

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
link_directories("/usr/lib/TinyPiX")


# 定义源码路径宏
set(SOURCE_FILES
    ${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/MainWindowService.cpp
)

include_directories(${SOURCE_PATH})
include_directories(/usr/include)
include_directories(/usr/include/TinyPiX/TpWM)
include_directories(/usr/include/TinyPiX)
include_directories(/usr/include/TinyPiX/TpService)
include_directories(/usr/include/TinyPiX/TpCore)
include_directories(/usr/include/TinyPiX/TpUtils)

include_directories(/usr/include/TinyPiX/TpGUI)
include_directories(/usr/include/TinyPiX/TpGUI/GUI)
include_directories(/usr/include/TinyPiX/TpGUI/Widgets)

add_executable(BatteryDemo ${SOURCE_FILES})

# 指定依赖动态库目录
if(CMAKE_CROSSCOMPILING)
    target_link_directories(BatteryDemo PRIVATE
    ${LOAD_SDK_LIB_PATH}
)
else()
    target_link_directories(BatteryDemo PRIVATE
    /usr/lib/TinyPiX
)
endif()

# 在find_library之前设置
set(CMAKE_PREFIX_PATH
    "/usr/lib"
    "/usr/lib/TinyPiX"
    ${CMAKE_PREFIX_PATH}
)

LoadSDKLibrary(TPCORE_LIBRARY TpCore)
LoadSDKLibrary(TPGUI_LIBRARY TpGUI)
LoadSDKLibrary(TPUTILS_LIBRARY TpUtils)
LoadSDKLibrary(TPSERVICE_LIBRARY TpService)
LoadSDKLibrary(TPWMENGINE_LIBRARY TpWMEngine)
LoadSDKLibrary(WEBP_LIBRARY webp)
LoadSDKLibrary(PNG_LIBRARY png)
LoadSDKLibrary(GOMP_LIBRARY gomp)

# 将pthread库链接到可执行目标
target_link_libraries(BatteryDemo 
    ${TPCORE_LIBRARY} ${TPGUI_LIBRARY} ${TPUTILS_LIBRARY} ${TPSERVICE_LIBRARY} ${TPWMENGINE_LIBRARY}
    ${WEBP_LIBRARY}
    ${PNG_LIBRARY}
    ${GOMP_LIBRARY}    
)

# 包含共享模块
include(../../cmake/SetupAppInstallation.cmake)

# 初始化应用安装信息
setup_app_installation("33406742-ceb3-3833-c0a5-ae7e3a82811f")
