#include "CapturePublishWidget.h"
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraImageCapture>
#include <QtMultimediaWidgets/QCameraViewfinder>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QsLog/QsLog.h>

CapturePublishWidget::CapturePublishWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    initPageCapture();
    initPagePreviewPublish();
}

void CapturePublishWidget::onBtnCaptureClicked()
{
    QLOG_TRACE() << "CapturePublishWidget::onBtnCaptureClicked()";

//on half pressed shutter button
    m_camera->searchAndLock();

//on shutter button pressed
    m_cameraImageCapture->capture();

//on shutter button released
    m_camera->unlock();
}

void CapturePublishWidget::onImageSaved(int id, const QString &fileName)
{
    QLOG_TRACE() << "CapturePublishWidget::onImageSaved(" << id << "," << fileName << ")";
    // TODO remove me? maybe required for HTTP-Form-Post
}

void CapturePublishWidget::onImageCaptured(int id, const QImage &preview)
{
    QLOG_TRACE() << "CapturePublishWidget::onImageCaptured(" << id << ", preview)";

    m_preview->setPixmap(QPixmap::fromImage(preview));
    setCurrentIndex(1);
}

void CapturePublishWidget::initPageCapture()
{
    m_camera = new QCamera;
    m_cameraViewfinder = new QCameraViewfinder();
    m_camera->setViewfinder(m_cameraViewfinder);
    m_cameraImageCapture = new QCameraImageCapture(m_camera);
    m_cameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    connect(m_cameraImageCapture, &QCameraImageCapture::imageSaved,
            this, &CapturePublishWidget::onImageSaved);
    connect(m_cameraImageCapture, &QCameraImageCapture::imageCaptured,
            this, &CapturePublishWidget::onImageCaptured);
    m_camera->setCaptureMode(QCamera::CaptureStillImage);
    m_camera->start();

    m_btnCapture = new QPushButton(this);
    m_btnCapture->setText(tr("Capture!"));
    connect(m_btnCapture, &QPushButton::clicked, this, &CapturePublishWidget::onBtnCaptureClicked);

    auto *layout = new QVBoxLayout();
    layout->addWidget(m_cameraViewfinder);
    layout->addWidget(m_btnCapture);
    auto *pageCapture = new QWidget(this);
    pageCapture->setLayout(layout);
    addWidget(pageCapture);
}

void CapturePublishWidget::initPagePreviewPublish()
{
    m_preview = new QLabel();
    m_preview->setBackgroundRole(QPalette::Base);
    m_preview->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_preview->setScaledContents(true);

    m_btnPublish = new QPushButton();
    m_btnPublish->setText(tr("Publish"));
    connect(m_btnPublish, &QPushButton::clicked, this, &CapturePublishWidget::onBtnPublishClicked);

    m_btnDiscard = new QPushButton();
    m_btnDiscard->setText(tr("Discard"));
    connect(m_btnDiscard, &QPushButton::clicked, this, &CapturePublishWidget::onBtnDiscardClicked);

    auto *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_btnDiscard);
    btnLayout->addWidget(m_btnPublish);

    auto *layout = new QVBoxLayout();
    layout->addWidget(m_preview);
    layout->addLayout(btnLayout);

    auto *pagePreviewPublish = new QWidget(this);
    pagePreviewPublish->setLayout(layout);
    addWidget(pagePreviewPublish);
}

void CapturePublishWidget::onBtnPublishClicked()
{
    setCurrentIndex(0);
}

void CapturePublishWidget::onBtnDiscardClicked()
{
    setCurrentIndex(0);
}
