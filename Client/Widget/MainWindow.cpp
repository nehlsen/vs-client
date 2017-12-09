#include "MainWindow.h"

#include <Client/Widget/LoginWidget.h>
#include <Client/Widget/SelectVenueWidget.h>
#include <Client/Widget/VenueWidget.h>
#include <QtCore/QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QStackedWidget(parent)
{
    m_client = new Client(this);
    m_client->setAutoFetchPicturesEnabled(true);
    connect(m_client, &Client::statusChanged,
            this, &MainWindow::onClientStatusChanged);

    m_loginWidget = new LoginWidget(this);
    connect(m_loginWidget->btnLogin, &QPushButton::clicked,
            this, &MainWindow::onBtnLoginClicked);
    addWidget(m_loginWidget);

    m_selectVenueWidget = new SelectVenueWidget(this);
    connect(m_selectVenueWidget->btnNext, &QPushButton::clicked,
            this, &MainWindow::onBtnSelectVenueClicked);
    addWidget(m_selectVenueWidget);

    m_venueWidget = new VenueWidget(this);
    connect(m_venueWidget->capturePublishWidget, &CapturePublishWidget::publishImage,
            m_client, &Client::postPicture);
    addWidget(m_venueWidget);

    loadSettings();
    onClientStatusChanged(Client::Offline);
}

void MainWindow::onClientStatusChanged(Client::Status status)
{
    switch (status) {
        case Client::Offline:
            setCurrentIndex(0);
            break;
        case Client::Online:
            setCurrentIndex(1);
            break;
        case Client::Ready:
            setCurrentIndex(2);
            break;
        case Client::Error:
        default:
            break;
    }
}

void MainWindow::onBtnLoginClicked()
{
    m_client->acquireToken(m_loginWidget->editUsername->text(), m_loginWidget->editPassword->text());
}

void MainWindow::onBtnSelectVenueClicked()
{
    m_client->setVenue(m_selectVenueWidget->editVenueToken->text());
}

void MainWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("Server");
    if (!settings.contains("Url")) {
        settings.setValue("Url", "http://vesh.my-server.com");
    }
    if (!settings.contains("Username")) {
        settings.setValue("Username", "my-username");
    }
    if (!settings.contains("Password")) {
        settings.setValue("Password", "my-password");
    }
    settings.endGroup();
    settings.sync();

    m_client->setServer(settings.value("Server/Url").toString());
    m_loginWidget->editUsername->setText(settings.value("Server/Username").toString());
    m_loginWidget->editPassword->setText(settings.value("Server/Password").toString());
}
