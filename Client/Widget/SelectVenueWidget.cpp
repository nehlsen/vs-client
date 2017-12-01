#include "SelectVenueWidget.h"

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

SelectVenueWidget::SelectVenueWidget(QWidget *parent) :
    QWidget(parent)
{
    m_editVenueToken = new QLineEdit(this);

    auto *layout = new QFormLayout(this);
    layout->addRow(new QLabel(tr("Venue token")), m_editVenueToken);

    setLayout(layout);
}