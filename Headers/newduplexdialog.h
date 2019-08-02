#ifndef NEWDUPLEXDIALOG_H
#define NEWDUPLEXDIALOG_H

#include "Headers/newstranddialog.h"

class NewDuplexDialog : public NewStrandDialog
{
    Q_OBJECT
public:
    explicit NewDuplexDialog(QWidget *parent = nullptr);
    ~NewDuplexDialog() override;

protected slots:
    void acceptInput() override;
};

#endif // NEWDUPLEXDIALOG_H
