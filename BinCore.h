#ifndef VENUESHOTCLIENT_BINCORE_H
#define VENUESHOTCLIENT_BINCORE_H

#include <QtWidgets/QApplication>
#include <Client/Model/SlideShow.h>
#include <Client/Client.h>

class BinCore
{
public:
    BinCore(int argc, char **argv);

    enum InitOptions {
        LogCli = 0x1,
        LogFile = 0x2,
        LogCliAndFile = LogCli | LogFile,
    };
    void init(InitOptions options = LogFile);
    int run();

    Client* createClient();
    SlideShow *createSlideShow(Client *client);

protected:
    QApplication m_app;
};

#endif //VENUESHOTCLIENT_BINCORE_H
