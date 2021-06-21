#include <QLineEdit>
#include <QVBoxLayout>

#include "byepage.h"
#include "wigglywidget.h"

ByePage::ByePage(QWidget *parent)
    : QDialog(parent)
{
    WigglyWidget *wigglyWidget = new WigglyWidget;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(wigglyWidget);

    setWindowTitle(tr("Wiggly"));
    resize(360, 145);
}
