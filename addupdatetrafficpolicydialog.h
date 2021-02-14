#ifndef ADDUPDATETRAFFICPOLICYDIALOG_H
#define ADDUPDATETRAFFICPOLICYDIALOG_H

#include <QDialog>
#include<DatabaseManager.h>
#include<Definitions.h>
namespace Ui {
class AddUpdateTrafficPolicyDialog;
}
namespace RidvanView
{
    class AddUpdateTrafficPolicyDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit AddUpdateTrafficPolicyDialog(QWidget *parent = nullptr);
        ~AddUpdateTrafficPolicyDialog();
        void setDatabaseManager(DatabaseManager* dbManager);
        void setOwnerTrafficPolicyAndPerson(TrafficPolicy trafficPolicy,Person person);
    signals:
        void trafficPolicyUpdated();
    protected slots:
        void on_pushButtonSave_clicked();
    private:
        Ui::AddUpdateTrafficPolicyDialog *ui;
        DatabaseManager* mDatabaseManager;
        TrafficPolicy mOwnerTrafficPolicy;
        Person mOwnerPerson;
    };
}

#endif // ADDUPDATETRAFFICPOLICYDIALOG_H
