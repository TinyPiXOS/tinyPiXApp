#include "TpApp.h"
#include "mainWindowService.h"
#include <iostream>
#include "browserToolWindow/fileListWindow.h"
#include "TpDialog.h"

int32_t main(int32_t argc, char *argv[])
{
	TpApp app(argc, argv);

	mainWindowService *fileManage = new mainWindowService();
	app.bindVScreen(fileManage);
	fileManage->update();

	return app.run();
}
