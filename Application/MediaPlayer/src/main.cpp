#include "TpApp.h"
#include "MainWindowService.h"

int32_t main(int32_t argc, char *argv[])
{
	TpApp app(argc, argv);

	MainWindowService *mainWindow = new MainWindowService();
	mainWindow->setBackBtnVisible(false);

	if (argc > 1)
	{
		TpString inputFile = argv[1];
		mainWindow->setBackBtnVisible(true);
		mainWindow->setPlayFile(inputFile);
	}
    // mainWindow->setPlayFile("/System/data/Music/千千阙歌.mp3");
    mainWindow->setPlayFile("/System/data/Videos/demo.mp4");

	return app.run();
}
