#ifndef ADDUPDATEHEALTHPOLICYDIALOG_H
#define ADDUPDATEHEALTHPOLICYDIALOG_H

#include <QDialog>
#include<DatabaseManager.h>
#include<Definitions.h>
namespace Ui {
class AddUpdateHealthPolicyDialog;
}
namespace RidvanView
{
    class AddUpdateHealthPolicyDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit AddUpdateHealthPolicyDialog(QWidget *parent = nullptr);
        ~AddUpdateHealthPolicyDialog();
        void setOwnerHealthPolicyAndPerson(HealthPolicy healthPolicy,Person person);
        void setDatabaseManager(DatabaseManager* dbManager);
    signals:
        void healthPolicyListUpdated();
    protected slots:
        void on_pushButtonSave_clicked();
    private:
        Ui::AddUpdateHealthPolicyDialog *ui;
        HealthPolicy mOwnerHealthPolicy;
        Person mOwnerPerson;
        DatabaseManager* mDatabaseManager;
    };
}


#endif // ADDUPDATEHEALTHPOLICYDIALOG_H
