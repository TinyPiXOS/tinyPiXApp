#include "DeskEntry.h"
#include "TpApp.h"
#include "DeskScreen.h"
#include "ToolBar/TopBar.h"
#include "ToolBar/BottomBar.h"

DeskScreen::DeskScreen() : TpMainWindow("tinyPiX_DeskTop_0x43ef3dc14"), pressAppBtn_(nullptr)
	, isMoveMode_(false), installingApp_(nullptr)
{
	this->construct();
	this->setVisible(true);
}

DeskScreen::~DeskScreen()
{
	this->destruction();
}

// int deskEntry_Start(int argc, char *argv[])
int main(int argc, char *argv[])
{
	TpApp app(argc, argv);
	DeskScreen *deskTopMainWindow = new DeskScreen();

	if (deskTopMainWindow)
	{
		app.bindVScreen(deskTopMainWindow);
		deskTopMainWindow->update();
		return app.run();
	}

	return false;
}
