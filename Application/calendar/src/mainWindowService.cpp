#include "mainWindowService.h"
#include "calendarGlobal.h"

mainWindowService::mainWindowService()
    : tpFixScreen()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(248, 248, 248));
    setVisible(true);
}

mainWindowService::~mainWindowService()
{
}

bool mainWindowService::appChange(int32_t id, int32_t pid, int32_t rotate, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    return true;
}

bool mainWindowService::onResizeEvent(tpObjectResizeEvent *event)
{
    std::cout << "systemSetting::onResizeEvent" << std::endl;

    return true;
}

bool mainWindowService::onActiveEvent(tpObjectActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
