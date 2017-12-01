#ifndef VSC_LOGINWIDGET_H
#define VSC_LOGINWIDGET_H

#include <QtWidgets/QWidget>

class QLineEdit;

class LoginWidget : public QWidget
{
Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

protected:
    QLineEdit *m_editUsername;
    QLineEdit *m_editPassword;
};

#endif // VSC_LOGINWIDGET_H
