#include "LoginWidget.h"

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent)
{
    m_editUsername = new QLineEdit(this);
    m_editPassword = new QLineEdit(this);
    m_editPassword->setEchoMode(QLineEdit::Password);

    auto *layout = new QFormLayout(this);
    layout->addRow(new QLabel(tr("Username")), m_editUsername);
    layout->addRow(new QLabel(tr("Password")), m_editPassword);

    setLayout(layout);
}
