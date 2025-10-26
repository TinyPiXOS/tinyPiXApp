#include "MainWindowService.h"
#include "TpComBox.h"

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));

    TpComBox *listCombox = new TpComBox(this);
    listCombox->setTitle("视图模式");
    listCombox->addItem("测试1");
    listCombox->addItem("测试2");
    listCombox->addItem("测试3");

    listCombox->move(20, 20);
}

MainWindowService::~MainWindowService()
{
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    return true;
}

bool MainWindowService::onResizeEvent(TpResizeEvent *event)
{
    std::cout << "systemSetting::onResizeEvent" << std::endl;

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
