#include "MainWindowService.h"
#include "TpPercentProgressBar.h"
#include <Service/TpAppManager.h>
#include <TpEvent.h>

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    TpAppManager::Instance()->setStatusBarStyle(_RGB(128, 128, 128));

    TpPercentProgressBar *progress = new TpPercentProgressBar(this);
    progress->setWidth(700);
    progress->move(0, 100);

    progress->setTotalValue(100);
    progress->addItem("图片", 8);
    progress->addItem("音频", 15);
    progress->addItem("视频", 30);
    progress->addItem("文本", 23);
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
        TpAppManager::Instance()->setStatusBarStyle(_RGB(128, 128, 128));
    }

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
