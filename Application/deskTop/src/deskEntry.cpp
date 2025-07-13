#include "deskEntry.h"
#include "tpApp.h"
#include "deskScreen.h"
#include "toolBar/topBar.h"
#include "toolBar/bottomBar.h"

tpDeskScreen::tpDeskScreen() : tpFixScreen("tinyPiX_DeskTop_0x43ef3dc14"), pressAppBtn_(nullptr)
	, isMoveMode_(false), installingApp_(nullptr)
{
	this->construct();
	this->setVisible(true);
}

tpDeskScreen::~tpDeskScreen()
{
	this->destruction();
}

// int deskEntry_Start(int argc, char *argv[])
int main(int argc, char *argv[])
{
	tpApp app(argc, argv);
	tpDeskScreen *deskTop = new tpDeskScreen();

	if (deskTop)
	{
		app.bindVScreen(deskTop);
		deskTop->update();
		return app.run();
	}

	return false;
}
