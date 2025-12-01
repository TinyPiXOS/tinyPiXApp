#include "MainWindowService.h"
#include "CalendarGlobal.h"
#include <Service/TpSystemApi.h>
#include "TpEvent.h"

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(248, 248, 248));
    TpSystemApi::Instance()->setStatusBarStyle(_RGB(248, 248, 248));
}

MainWindowService::~MainWindowService()
{
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    return true;
}

bool MainWindowService::onVisibleEvent(TpVisibleEvent *event)
{
    if (event->visible())
    {
        TpSystemApi::Instance()->setStatusBarStyle(_RGB(248, 248, 248));
    }

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
