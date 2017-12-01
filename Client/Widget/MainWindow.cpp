#include "MainWindow.h"

#include <Client/Client.h>
#include <Client/Widget/LoginWidget.h>
#include <Client/Widget/SelectVenueWidget.h>

MainWindow::MainWindow(QWidget *parent) :
    QStackedWidget(parent)
{
    m_client = new Client(this);

    m_loginWidget = new LoginWidget(this);
    addWidget(m_loginWidget);
    m_selectVenueWidget = new SelectVenueWidget(this);
    addWidget(m_selectVenueWidget);
}
