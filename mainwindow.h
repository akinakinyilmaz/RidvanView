#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<addupdatepersondialog.h>
#include<DatabaseManager.h>
#include<persondetails.h>
#include<QSettings>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
using namespace RidvanView;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fillUpcomingPolicies();
protected slots:
    void on_pushButtonAddPerson_clicked();
    void on_pushButtonSearch_clicked();
    void on_pushButtonEditPerson_clicked();
    void on_pushButtonDeletePerson_clicked();
    void onPersonListUpdated();
    void on_pushButtonDetails_clicked();
    void on_pushButtonUpcomingPoliciesSeeDetails_clicked();
    void on_pushButtonSaveSettings_clicked();
private:
    Ui::MainWindow *ui;
    AddUpdatePersonDialog *mAddUpdatePersonDialog;
    PersonDetails* mPersonDetailsDialog;
    DatabaseManager* mDatabaseManager;
    int mUpcomingPoliciesAlarmDayCount = 15;
    QSettings* mProgramSettings;
};
#endif // MAINWINDOW_H
