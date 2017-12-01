#include "LoginWidget.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent)
{
    editUsername = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPassword->setEchoMode(QLineEdit::Password);

    btnLogin = new QPushButton(this);
    btnLogin->setText(tr("Login"));

    auto *layout = new QFormLayout(this);
    layout->addRow(new QLabel(tr("Username")), editUsername);
    layout->addRow(new QLabel(tr("Password")), editPassword);
    layout->addRow(btnLogin);

    setLayout(layout);
}
