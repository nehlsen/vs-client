#ifndef VSC_CAPTUREPUBLISHWIDGET_H
#define VSC_CAPTUREPUBLISHWIDGET_H

#include <QtWidgets/QStackedWidget>

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QPushButton;
class QLabel;

class CapturePublishWidget : public QStackedWidget
{
Q_OBJECT

public:
    explicit CapturePublishWidget(QWidget *parent = nullptr);

signals:
    void publishImage(const QImage &image);

protected slots:
    void onBtnCaptureClicked();
    void onImageSaved(int id, const QString &fileName);
    void onImageCaptured(int id, const QImage &preview);

    void onBtnPublishClicked();
    void onBtnDiscardClicked();

protected:
    void initPageCapture();
    QCamera *m_camera;
    QCameraViewfinder *m_cameraViewfinder;
    QCameraImageCapture *m_cameraImageCapture;
    QPushButton *m_btnCapture;

    void initPagePreviewPublish();
    QImage m_preview;
    QLabel *m_previewDisplay;
    QPushButton *m_btnPublish;
    QPushButton *m_btnDiscard;
};

#endif // VSC_CAPTUREPUBLISHWIDGET_H
