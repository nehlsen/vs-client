#include <BinCore.h>
#include <Client/Widget/MainWindow.h>

int main(int argc, char **argv)
{
    BinCore bc(argc, argv);
    bc.init();

    MainWindow mainWindow;
    mainWindow.show();

    return bc.run();
}
