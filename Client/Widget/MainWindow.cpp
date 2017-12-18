#include "MainWindow.h"
#include <Client/Widget/LoginWidget.h>
#include <Client/Widget/SelectVenueWidget.h>
#include <Client/Widget/VenueWidget.h>
#include <Client/Model/VenuePictures.h>
#include <QtCore/QSettings>
#include <Client/Settings.h>

MainWindow::MainWindow(QWidget *parent) :
    QStackedWidget(parent)
{
    m_client = new Client(this);
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

    m_venueWidget = new VenueWidget(m_client, this);
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
    Settings::setDefaults();

    QSettings settings;
    m_client->setServer(settings.value("Server/Url").toString());
    m_client->venuePictures()->setAutoFetchPicturesEnabled(settings.value("Config/AutoFetchEnabled").toBool());
    m_client->venuePictures()->setAutoUpdateInterval(settings.value("Config/AutoUpdateInterval").toInt());
    m_client->venuePictures()->setCacheFolder(settings.value("Local/CacheFolder").toString());
    m_loginWidget->editUsername->setText(settings.value("Server/Username").toString());
    m_loginWidget->editPassword->setText(settings.value("Server/Password").toString());
}
