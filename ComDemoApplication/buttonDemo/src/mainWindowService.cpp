#include "mainWindowService.h"
#include "tpButton.h"

mainWindowService::mainWindowService()
    : tpFixScreen()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    
    tpButton *button1 = new tpButton("北京市", this);
	button1->setProperty("type", "ControlPanelPowerButton");
	button1->setSize(300, 64);
	button1->move(150, 150);

	tpButton *buttonEnable = new tpButton("河北省", this);
	buttonEnable->setProperty("type", "VirtualKeyboardButton");
	buttonEnable->setSize(305, 64);
	buttonEnable->move(460, 150);

	tpButton *button2 = new tpButton(this);
	button2->setButtonStyle(tpButton::TextBesideIcon);
	button2->setProperty("type", "FunctionButton");
	button2->setIcon(applicationDirPath() + "/../res/btnIcon.png");
	button2->setText("吉林省2");
	button2->setRect(50, 250, 250, 50);

	tpButton *button3 = new tpButton(this);
	button3->setButtonStyle(tpButton::IconOnly);
	button3->setIcon(applicationDirPath() + "/../res/btnIcon2.png");
	button3->setRoundCorners(13);
	button3->setRect(200, 350, 200, 200);
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
