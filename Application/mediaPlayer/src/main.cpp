#include "TpApp.h"
#include "mainWindowService.h"

int32_t main(int32_t argc, char *argv[])
{
	TpApp app(argc, argv);

	mainWindowService *mainWindow = new mainWindowService();
	app.bindVScreen(mainWindow);
	mainWindow->update();

	mainWindow->setBackBtnVisible(false);

	if (argc > 1)
	{
		TpString inputFile = argv[1];
		mainWindow->setBackBtnVisible(true);
		mainWindow->setPlayFile(inputFile);
		// mainWindow->setPlayFile("/System/data/千千阙歌.mp3");
	}

	return app.run();
}
