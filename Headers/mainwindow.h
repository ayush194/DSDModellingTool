//
//  mainwindow.h
//  DSDModellingTool
//
//  Created by Ayush Kumar on 30/5/19.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QGraphicsScene>
#include "Headers/newstranddialog.h"
#include "Headers/newspeciesdialog.h"
#include "Headers/newduplexdialog.h"
#include "Headers/strandscene.h"
#include "Headers/dashboardscene.h"
#include "Headers/springscene.h"

class QPushButton;
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
//signals:
private slots:
    //void createNewStrand();
    //void openNewStrandWindow();
    void dropdownActiveItemChanged(const QString&);
    void addItemInDropdown(int);
    void refreshMainDropdown();
    void joinDomains();
    void unjoinDomains();
    void foo();
public slots:
    void updateCost(double);

private:
    QComboBox *maindropdown;
    StrandScene *strandscene;
    DashboardScene *dashboardscene;
    SpringScene *springscene;
    //QGraphicsScene *graphicsscene;
    QGraphicsView *graphicsview;
    QPushButton *newspeciesbutton;
    QPushButton *newstrandbutton;
    QPushButton *newduplexbutton;
    QPushButton *joinbutton;
    QPushButton *unjoinbutton;
    QPushButton *foobutton;
    NewStrandDialog *newstranddialog;
    NewSpeciesDialog *newspeciesdialog;
    NewDuplexDialog *newduplexdialog;
    QLabel *costlabel;
};

#endif // MAINWINDOW_H
