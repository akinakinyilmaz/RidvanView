#include "addupdatepersondialog.h"
#include "ui_addupdatepersondialog.h"
#include<QMessageBox>
using namespace RidvanView;
AddUpdatePersonDialog::AddUpdatePersonDialog(QWidget *parent) :
    mDatabaseManager(NULL),
    QDialog(parent),
    ui(new Ui::AddUpdatePersonDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
}

AddUpdatePersonDialog::~AddUpdatePersonDialog()
{
    delete ui;
}

void AddUpdatePersonDialog::setDatabaseManager(DatabaseManager *manager)
{
    mDatabaseManager = manager;
}

void AddUpdatePersonDialog::setOwnerPerson(Person person)
{
    mOwnerPerson = person;
    ui->lineEditName->setText(person.mName);
    ui->lineEditRegisterNo->setText(person.mRegisterNo);
    ui->lineEditPhoneNumber->setText(person.mPhoneNumber);
    ui->lineEditSurname->setText(person.mSurname);
}

void AddUpdatePersonDialog::on_pushButtonAddPersonOk_clicked()
{
    if(ui->lineEditName->text().size() == 0 || ui->lineEditSurname->text().size() == 0 ||
            ui->lineEditPhoneNumber->text().size() == 0 || ui->lineEditRegisterNo->text().size() == 0)
    {
        QMessageBox::critical(this,tr("Hata"),tr("Lütfen Bütün Alanları Doldurunuz"));
        return;
    }
    mOwnerPerson.mName = ui->lineEditName->text();
    mOwnerPerson.mSurname = ui->lineEditSurname->text();
    mOwnerPerson.mPhoneNumber = ui->lineEditPhoneNumber->text();
    mOwnerPerson.mRegisterNo = ui->lineEditRegisterNo->text();
    Person existedPerson = mDatabaseManager->getPersonWithRegisterNo(mOwnerPerson.mRegisterNo);
    if(existedPerson.mId != mOwnerPerson.mId && existedPerson.mId > 0)
    {
        QMessageBox::critical(this,tr("Hata"),tr("Aynı kayıt numarasına sahip başka bir kullanıcı var!"));
        return;
    }
    mDatabaseManager->addUpdatePerson(mOwnerPerson);
    this->hide();
    emit personListUpdated();
}
