#ifndef ADDUPDATEPERSONDIALOG_H
#define ADDUPDATEPERSONDIALOG_H

#include <QDialog>
#include<DatabaseManager.h>
namespace Ui {
class AddUpdatePersonDialog;
}
namespace RidvanView
{
    class AddUpdatePersonDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit AddUpdatePersonDialog(QWidget *parent = nullptr);
        ~AddUpdatePersonDialog();
        void setDatabaseManager(DatabaseManager* manager);
        void setOwnerPerson(Person person);
    signals:
        void personListUpdated();
    protected slots:
        void on_pushButtonAddPersonOk_clicked();
    private:
        Ui::AddUpdatePersonDialog *ui;
        DatabaseManager* mDatabaseManager;
        Person mOwnerPerson;
    };
}

#endif // ADDUPDATEPERSONDIALOG_H
