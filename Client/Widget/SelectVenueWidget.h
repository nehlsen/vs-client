#ifndef VSC_SELECTVENUEWIDGET_H
#define VSC_SELECTVENUEWIDGET_H

#include <QtWidgets/QWidget>

class QLineEdit;

class SelectVenueWidget : public QWidget
{
Q_OBJECT

public:
    explicit SelectVenueWidget(QWidget *parent = nullptr);

protected:
    QLineEdit *m_editVenueToken;
};

#endif // VSC_SELECTVENUEWIDGET_H
