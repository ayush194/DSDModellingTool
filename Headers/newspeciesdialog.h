#ifndef NEWSPECIESDIALOG_H
#define NEWSPECIESDIALOG_H

#include "Headers/abstractnewdialog.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

//TODO : Define an abstract class NewDialog from which NewSpeciesDialog and NewStrandDialog will be derived
//the abstract class will have virtual methods okbutton, cancelbutton, slots for exec(), acceptInput() and connections for these

class NewSpeciesDialog : public AbstractNewDialog
{
    Q_OBJECT
public:
    explicit NewSpeciesDialog(QWidget *parent = nullptr);
    ~NewSpeciesDialog() override;

private:
    QLabel *strandidslabel;
    QLabel *strandidserror;
    QLabel *speciesidslabel;
    QLabel *speciesidserror;
    QLineEdit *speciesidslineedit;
    QLineEdit *strandidslineedit;
    //QPushButton *cancelbutton;
    //QPushButton *okbutton;

protected slots:
    //int exec();
    void acceptInput() override;
};

#endif // NEWSPECIESDIALOG_H
