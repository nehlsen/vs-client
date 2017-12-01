#ifndef VSC_MAINWINDOW_H
#define VSC_MAINWINDOW_H

#include <QtWidgets/QStackedWidget>
#include <Client/Client.h>

class LoginWidget;
class SelectVenueWidget;

class MainWindow : public QStackedWidget
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected slots:
    void onClientStatusChanged(Client::Status status);
    void onBtnLoginClicked();
    void onBtnSelectVenueClicked();

protected:
    Client *m_client;

    // TODO add settings widget to setup server etc
    LoginWidget *m_loginWidget;
    SelectVenueWidget *m_selectVenueWidget;

    void loadSettings();
};

#endif // VSC_MAINWINDOW_H
