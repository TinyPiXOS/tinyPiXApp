#include "mainWindowService.h"
#include "tpListCheckBoxGroup.h"

mainWindowService::mainWindowService()
    : tpFixScreen()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));

    tpListCheckBoxGroup *checkBoxGroup = new tpListCheckBoxGroup(this);

    checkBoxGroup->addItem("测试1");
    checkBoxGroup->addItem("测试2");
    checkBoxGroup->addItem("测试3");

    checkBoxGroup->addItem(new tpListCheckBoxGroupItem("测试4"));
    checkBoxGroup->addItem(new tpListCheckBoxGroupItem("测试5"));

    checkBoxGroup->move(15, 20);
    checkBoxGroup->update();
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
