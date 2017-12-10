#include "VenuePicturesWidget.h"
#include <QtCore/QTimer>
#include <QsLog/QsLog.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

VenuePicturesWidget::VenuePicturesWidget(QWidget *parent) :
    QWidget(parent),
    m_venuePictures(nullptr),
    m_picturesCurrentIndex(-1)
{
    m_pictureDisplay = new QLabel(this);
    m_pictureDisplay->setMinimumSize(100, 100);
    m_pictureDisplay->setBackgroundRole(QPalette::Base);
    m_pictureDisplay->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_pictureDisplay->setScaledContents(true);

    auto *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(m_pictureDisplay);
    setLayout(layout);

    m_delayAdvancePicture = new QTimer(this);
    m_delayAdvancePicture->setSingleShot(false);
    m_delayAdvancePicture->setInterval(1000);
    connect(m_delayAdvancePicture, &QTimer::timeout,
            this, &VenuePicturesWidget::advancePicture);

    m_delayAdvancePicture->start();
}

void VenuePicturesWidget::setVenuePictures(VenuePictures *venuePictures)
{
    Q_ASSERT(nullptr == m_venuePictures);

    m_venuePictures = venuePictures;
    connect(m_venuePictures, &VenuePictures::pictureReady,
            this, &VenuePicturesWidget::onPictureReady);
}

void VenuePicturesWidget::onPictureReady(const VenuePicture &pictures)
{
    m_pictures << pictures;
}

void VenuePicturesWidget::advancePicture()
{
    if (m_pictures.isEmpty()) {
        return;
    }

    if (m_picturesCurrentIndex < 0) {
        m_picturesCurrentIndex = 0;
    } else {
        m_picturesCurrentIndex++;
    }

    if (m_picturesCurrentIndex >= m_pictures.count()) {
        m_picturesCurrentIndex = 0;
    }

    QString currentPicturePath = m_pictures.at(m_picturesCurrentIndex).localPath();
    QLOG_INFO() << "VenuePicturesWidget::advancePicture(),"
                << "index:" << m_picturesCurrentIndex
                << "picture:" << currentPicturePath;

    m_currentPicture.load(currentPicturePath);
    m_pictureDisplay->setPixmap(m_currentPicture);
}
