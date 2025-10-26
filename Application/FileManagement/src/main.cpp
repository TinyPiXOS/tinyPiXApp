#include "TpApp.h"
#include "MainWindowService.h"
#include <iostream>
#include "BrowserToolWindow/FileListWindow.h"
#include "TpDialog.h"

int32_t main(int32_t argc, char *argv[])
{
	TpApp app(argc, argv);

	MainWindowService *fileManage = new MainWindowService();
	app.bindVScreen(fileManage);
	fileManage->update();

	return app.run();
}
