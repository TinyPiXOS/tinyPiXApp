#include "mainWindowService.h"
#include "tpPercentProgressBar.h"

mainWindowService::mainWindowService()
    : tpFixScreen()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));

    tpPercentProgressBar *progress = new tpPercentProgressBar(this);
    progress->setWidth(700);
    progress->move(0, 100);

    progress->setTotalValue(100);
    progress->addItem("图片", 8);
    progress->addItem("音频", 15);
    progress->addItem("视频", 30);
    progress->addItem("文本", 23);
}

mainWindowService::~mainWindowService()
{
}

bool mainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
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
