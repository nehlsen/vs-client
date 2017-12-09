#ifndef VSC_VENUEPICTURESWIDGET_H
#define VSC_VENUEPICTURESWIDGET_H

#include <QtWidgets/QWidget>
#include <Client/Model/VenuePictures.h>

class QLabel;

class VenuePicturesWidget : public QWidget
{
Q_OBJECT

public:
    explicit VenuePicturesWidget(QWidget *parent = nullptr);

    void setVenuePictures(VenuePictures *venuePictures);

protected slots:
    void onPictureReady(const VenuePicture &pictures);

    void advancePicture();

protected:
    QPixmap m_currentPicture;
    QLabel *m_pictureDisplay;

    VenuePictures *m_venuePictures;

    int m_picturesCurrentIndex;
    QList<VenuePicture> m_pictures;

    QTimer *m_delayAdvancePicture;
};

#endif // VSC_VENUEPICTURESWIDGET_H
