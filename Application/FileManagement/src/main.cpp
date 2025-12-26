#include "TpApp.h"
#include "MainWindowService.h"
#include <iostream>
#include "BrowserToolWindow/FileListWindow.h"
#include "TpDesktopDialog.h"

int32_t main(int32_t argc, char *argv[])
{
	TpApp app(argc, argv);

	MainWindowService *fileManage = new MainWindowService();
	fileManage->update();

	return app.run();
}
