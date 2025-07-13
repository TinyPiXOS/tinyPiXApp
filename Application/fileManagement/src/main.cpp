#include "tpApp.h"
#include "mainWindowService.h"
#include <iostream>
#include "browserToolWindow/fileListWindow.h"
#include "tpDialog.h"

int32_t main(int32_t argc, char *argv[])
{
	tpApp app(argc, argv);

	mainWindowService *fileManage = new mainWindowService();
	app.bindVScreen(fileManage);
	fileManage->update();

	return app.run();
}
