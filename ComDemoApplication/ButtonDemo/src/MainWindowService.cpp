#include "MainWindowService.h"
#include "TpButton.h"

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    
    TpButton *button1 = new TpButton("北京市", this);
	button1->setProperty("type", "ControlPanelPowerButton");
	button1->setSize(300, 64);
	button1->move(150, 150);

	TpButton *buttonEnable = new TpButton("河北省", this);
	buttonEnable->setProperty("type", "VirtualKeyboardButton");
	buttonEnable->setSize(305, 64);
	buttonEnable->move(460, 150);

	TpButton *button2 = new TpButton(this);
	button2->setButtonStyle(TpButton::TextBesideIcon);
	button2->setProperty("type", "FunctionButton");
	button2->setIcon(applicationDirPath() + "/../res/btnIcon.png");
	button2->setText("吉林省2");
	button2->setRect(50, 250, 250, 50);

	TpButton *button3 = new TpButton(this);
	button3->setButtonStyle(TpButton::IconOnly);
	button3->setIcon(applicationDirPath() + "/../res/btnIcon2.png");
	button3->setRoundCorners(13);
	button3->setRect(200, 350, 200, 200);
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
