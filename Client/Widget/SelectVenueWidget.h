#ifndef VSC_SELECTVENUEWIDGET_H
#define VSC_SELECTVENUEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

class QLineEdit;

class SelectVenueWidget : public QWidget
{
Q_OBJECT

public:
    explicit SelectVenueWidget(QWidget *parent = nullptr);

    QLineEdit *editVenueToken;

    QPushButton *btnNext;
};

#endif // VSC_SELECTVENUEWIDGET_H
