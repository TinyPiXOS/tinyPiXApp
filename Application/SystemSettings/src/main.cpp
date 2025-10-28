#include "TpApp.h"
#include "MainWindowService.h"

int32_t main(int32_t argc, char *argv[])
{
	TpApp app(argc, argv);

	MainWindowService *fileManage = new MainWindowService();
	fileManage->update();

	return app.run();
}
