#include "MainWindowService.h"
#include "TpLabel.h"
#include "TpSlider.h"
#include "TpFont.h"
#include "TpSignalSlot.h"

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    
    TpLabel *valueText = new TpLabel(this);
	valueText->setText(TpString::number(50));
	valueText->setAlign(Tp::AlignCenter);
	valueText->font()->setFontColor(_RGB(255, 255, 255),_RGB(255, 255, 255));
	valueText->font()->setFontSize(30);
	valueText->setWidth(600);
	valueText->setHeight(400);
	valueText->move(20, 200);

	TpSlider *slider = new TpSlider(this);
	slider->setValue(50);
	slider->setSize(500, 10);
	slider->move(20, 60);

	TpSlider *vSlider = new TpSlider(this);
	vSlider->setDirection(TpSlider::Vertical);
	vSlider->setValue(50);
	vSlider->setSize(10, 500);
	vSlider->move(650, 20);

	connect(slider, valueChanged, [=](int32_t value)
			{ valueText->setText(TpString::number(value));
				vSlider->setValue(value); });

	connect(vSlider, valueChanged, [=](int32_t value)
			{ valueText->setText(TpString::number(value));
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
