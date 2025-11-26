#include "DeskEntry.h"
#include "TpApp.h"
#include "DeskScreen.h"

// int deskEntry_Start(int argc, char *argv[])
int main(int argc, char *argv[])
{
    TpApp app(argc, argv, "tinyPiX_DeskTop_0x43ef3dc14");
    DeskScreen *deskTopMainWindow = new DeskScreen();
    deskTopMainWindow->update();

    return app.run();
}
