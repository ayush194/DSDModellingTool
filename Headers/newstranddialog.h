//
//  newstranddialog.h
//  DSDModellingTool
//
//  Created by Ayush Kumar on 2/6/19.
//

#ifndef NEWSTRANDDIALOG_H
#define NEWSTRANDDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "Headers/abstractnewdialog.h"

class NewStrandDialog : public AbstractNewDialog {
    Q_OBJECT
public:
    explicit NewStrandDialog(QWidget *parent = nullptr);
    ~NewStrandDialog() override;

protected slots:
    void acceptInput() override;

protected:
    QLabel *domainseqlabel;
    QLabel *lengthslabel;
    //QLabel *iderror;
    //QLabel *idlabel;
    QLabel *domainseqerror;
    QLabel *lengthserror;
    QLineEdit *domainseqlineedit;
    QLineEdit *lengthslineedit;
    //QTextEdit *idtextedit;
    //QPushButton *cancelbutton;
    //QPushButton *okbutton;
};

#endif // NEWSTRANDDIALOG_H
