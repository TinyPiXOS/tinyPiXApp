#include "mainWindowService.h"
#include "tpBattery.h"
#include "tpSignalSlot.h"
#include "tpSlider.h"

mainWindowService::mainWindowService()
    : tpFixScreen()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    setVisible(true);

    tpBattery *battertWidget = new tpBattery(this);
    battertWidget->setValue(50);
    battertWidget->setWidth(600);
    battertWidget->setHeight(400);
    battertWidget->move(20, 200);

    tpSlider *slider = new tpSlider(this);
    slider->setValue(50);
    slider->setSize(500, 10);
    slider->move(20, 20);

    tpSlider *vSlider = new tpSlider(this);
    vSlider->setDirection(tpSlider::Vertical);
    vSlider->setValue(50);
    vSlider->setSize(10, 500);
    vSlider->move(650, 20);

    connect(slider, valueChanged, [=](int32_t value)
            { battertWidget->setValue(value);
				vSlider->setValue(value); });

    connect(vSlider, valueChanged, [=](int32_t value)
            { battertWidget->setValue(value);
				slider->setValue(value); });
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
