#include "MainWindowService.h"
#include "TpListWidget.h"
#include <TpAppManager.h>
#include <TpEvent.h>

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    TpAppManager::Instance()->setStatusBarStyle(_RGB(128, 128, 128));

    TpListWidget *checkBoxGroup = new TpListWidget(this);

    checkBoxGroup->addItem("测试1");
    checkBoxGroup->addItem("测试2");
    checkBoxGroup->addItem("测试3");

    checkBoxGroup->addItem(new TpListWidgetItem("测试4"));
    checkBoxGroup->addItem(new TpListWidgetItem("测试5"));

    checkBoxGroup->move(15, 20);
    checkBoxGroup->update();
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
