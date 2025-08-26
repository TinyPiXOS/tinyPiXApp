#include "deskEntry.h"
#include "TpApp.h"
#include "DeskScreen.h"
#include "ToolBar/topBar.h"
#include "ToolBar/bottomBar.h"

DeskScreen::DeskScreen() : TpFixScreen("tinyPiX_DeskTop_0x43ef3dc14"), pressAppBtn_(nullptr)
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
	DeskScreen *deskTop = new DeskScreen();

	if (deskTop)
	{
		app.bindVScreen(deskTop);
		deskTop->update();
		return app.run();
	}

	return false;
}
