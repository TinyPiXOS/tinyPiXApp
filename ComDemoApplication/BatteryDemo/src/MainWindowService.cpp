#include "MainWindowService.h"
#include "TpBattery.h"
#include "TpSignalSlot.h"
#include "TpSlider.h"
#include <Service/TpSystemApi.h>
#include <TpEvent.h>

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    TpSystemApi::Instance()->setStatusBarStyle(_RGB(128, 128, 128));

    TpBattery *battertWidget = new TpBattery(this);
    battertWidget->setValue(50);
    battertWidget->setWidth(600);
    battertWidget->setHeight(400);
    battertWidget->move(20, 200);

    TpSlider *slider = new TpSlider(this);
    slider->setValue(50);
    slider->setSize(500, 10);
    slider->move(20, 20);

    TpSlider *vSlider = new TpSlider(this);
    vSlider->setDirection(TpSlider::Vertical);
    vSlider->setValue(50);
    vSlider->setSize(10, 500);
    vSlider->move(650, 20);

    connect(slider, valueChanged, [=](int32_t value)
            { battertWidget->setValueAnimated(value);
				vSlider->setValue(value); });

    connect(vSlider, valueChanged, [=](int32_t value)
            { battertWidget->setValueAnimated(value);
				slider->setValue(value); });
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
        TpSystemApi::Instance()->setStatusBarStyle(_RGB(128, 128, 128));
    }

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
