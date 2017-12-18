#include "SlideShowWidget.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <Client/Model/SlideShow.h>

SlideShowWidget::SlideShowWidget(SlideShow *slideShow, QWidget *parent):
    QWidget(parent)
{
    initLayout();
    setupSlideShow(slideShow);
}

SlideShowWidget::SlideShowWidget(Client *client, QWidget *parent):
    QWidget(parent)
{
    initLayout();
    setupSlideShow(new SlideShow(client));
}

void SlideShowWidget::reset()
{
    m_pictureDisplay->setText(tr("No Pictures available"));
}

void SlideShowWidget::onShowPicture(const VenuePicture &picture)
{
    QString currentPicturePath = picture.localPath();
    m_currentPicture.load(currentPicturePath);
    m_pictureDisplay->setPixmap(m_currentPicture);
}

void SlideShowWidget::initLayout()
{
    m_pictureDisplay = new QLabel(this);
    m_pictureDisplay->setMinimumSize(100, 100);
    m_pictureDisplay->setBackgroundRole(QPalette::Base);
    m_pictureDisplay->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_pictureDisplay->setScaledContents(true);
    reset();

    auto *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(m_pictureDisplay);
    setLayout(layout);
}

void SlideShowWidget::setupSlideShow(SlideShow *slideShow)
{
    connect(slideShow, &SlideShow::cleared,
            this, &SlideShowWidget::reset);
    connect(slideShow, &SlideShow::showPicture,
            this, &SlideShowWidget::onShowPicture);
}
