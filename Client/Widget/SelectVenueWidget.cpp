#include "SelectVenueWidget.h"

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

SelectVenueWidget::SelectVenueWidget(QWidget *parent) :
    QWidget(parent)
{
    editVenueToken = new QLineEdit(this);

    btnNext = new QPushButton(this);
    btnNext->setText(tr("Next"));

    auto *layout = new QFormLayout(this);
    layout->addRow(new QLabel(tr("Venue token")), editVenueToken);
    layout->addRow(btnNext);

    setLayout(layout);
}
