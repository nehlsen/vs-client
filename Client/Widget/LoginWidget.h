#ifndef VSC_LOGINWIDGET_H
#define VSC_LOGINWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

class LoginWidget : public QWidget
{
Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

    QLineEdit *editUsername;
    QLineEdit *editPassword;

    QPushButton *btnLogin;
};

#endif // VSC_LOGINWIDGET_H
