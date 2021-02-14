#ifndef PERSONDETAILS_H
#define PERSONDETAILS_H

#include <QDialog>
#include<DatabaseManager.h>
#include<Definitions.h>
#include<addupdatevehicledialog.h>
#include<addupdatetrafficpolicydialog.h>
#include<addupdatehealthpolicydialog.h>
namespace Ui {
class PersonDetails;
}
namespace RidvanView
{
    class PersonDetails : public QDialog
    {
        Q_OBJECT

    public:
        explicit PersonDetails(QWidget *parent = nullptr);
        ~PersonDetails();
        void setDatabaseManager(DatabaseManager* dbManager);
        void setOwnerPerson(Person person);
        void updateVehiclesSegment();
        void updateTrafficPoliciesSegment();
        void updateHealthPoliciesSegment();
    signals:
        void personUpdated();
    protected slots:
        void on_pushButtonUpdate_clicked();
        void on_pushButtonAddVehicle_clicked();
        void on_pushButtonEditVehicle_clicked();
        void on_pushButtonDeleteVehicle_clicked();
        void onVehicleListUpdated();
        void on_pushButtonAddTrafficPolicy_clicked();
        void on_pushButtonEditTrafficPolicy_clicked();
        void on_pushButtonDeleteTrafficPolicy_clicked();
        void onTrafficPolicyListUpdated();
        void on_pushButtonAddHealthPolicies_clicked();
        void on_pushButtonEditHealthPolicy_clicked();
        void on_pushButtonDeleteHealthPolicy_clicked();
        void onHealthPolicyListUpdated();
    private:
        Ui::PersonDetails *ui;
        DatabaseManager* mDatabaseManager;
        Person mOwnerPerson;
        AddUpdateVehicleDialog* mAddUpdateVehicleDialog;
        AddUpdateTrafficPolicyDialog* mAddUpdateTrafficPolicyDialog;
        AddUpdateHealthPolicyDialog* mAddUpdateHealthPolicyDialog;
    };
}

#endif // PERSONDETAILS_H
