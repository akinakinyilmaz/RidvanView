#ifndef ADDUPDATEVEHICLEDIALOG_H
#define ADDUPDATEVEHICLEDIALOG_H

#include <QDialog>
#include<DatabaseManager.h>
#include<Definitions.h>
namespace Ui {
class AddUpdateVehicleDialog;
}
namespace RidvanView
{
    class AddUpdateVehicleDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit AddUpdateVehicleDialog(QWidget *parent = nullptr);
        ~AddUpdateVehicleDialog();
        void setDatabaseManager(DatabaseManager* dbManager);
        void setOwnerVehicle(Vehicle vehicle);
    signals:
        void vehicleListUpdated();
    protected slots:
        void on_pushButtonSave_clicked();
    private:
        Ui::AddUpdateVehicleDialog *ui;
        DatabaseManager* mDatabaseManager;
        Vehicle mOwnerVehicle;
    };
}

#endif // ADDUPDATEVEHICLEDIALOG_H
