#include "Headers/abstractnewdialog.h"

AbstractNewDialog::AbstractNewDialog(QWidget *parent) : QDialog(parent) {
    cancelbutton = new QPushButton(tr("Cancel"), this);
    okbutton = new QPushButton(tr("OK"), this);

    QObject::connect(cancelbutton, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(okbutton, SIGNAL(clicked()), this, SLOT(acceptInput()));
    QObject::connect(this, SIGNAL(inputValid(int)), this, SLOT(close()));
}

int AbstractNewDialog::exec() {
    this->activateWindow();
    this->show();
    return 1;
}
