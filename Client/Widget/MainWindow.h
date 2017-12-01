#ifndef VSC_MAINWINDOW_H
#define VSC_MAINWINDOW_H

#include <QtWidgets/QStackedWidget>

class Client;
class LoginWidget;
class SelectVenueWidget;

class MainWindow : public QStackedWidget
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    Client *m_client;

    // TODO add settings widget to setup server etc
    LoginWidget *m_loginWidget;
    SelectVenueWidget *m_selectVenueWidget;
};

#endif // VSC_MAINWINDOW_H
