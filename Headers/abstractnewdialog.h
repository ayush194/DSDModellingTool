#ifndef ABSTRACTNEWDIALOG_H
#define ABSTRACTNEWDIALOG_H

#include <QDialog>
#include <QPushButton>

class AbstractNewDialog : public QDialog
{
    Q_OBJECT
public:
    AbstractNewDialog(QWidget *parent = nullptr);

signals:
    void inputValid(int);
protected slots:
    int exec();
    virtual void acceptInput() = 0;

protected:
    QPushButton *cancelbutton;
    QPushButton *okbutton;
};

#endif // ABSTRACTNEWDIALOG_H
