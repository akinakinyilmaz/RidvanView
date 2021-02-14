#include "addupdatevehicledialog.h"
#include "ui_addupdatevehicledialog.h"
using namespace RidvanView;
AddUpdateVehicleDialog::AddUpdateVehicleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUpdateVehicleDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
}

AddUpdateVehicleDialog::~AddUpdateVehicleDialog()
{
    delete ui;
}

void AddUpdateVehicleDialog::setDatabaseManager(DatabaseManager *dbManager)
{
    mDatabaseManager = dbManager;
}

void AddUpdateVehicleDialog::setOwnerVehicle(Vehicle vehicle)
{
    mOwnerVehicle = vehicle;
    ui->lineEditMarka->setText(mOwnerVehicle.mMarka);
    ui->lineEditTipi->setText(mOwnerVehicle.mTipi);
    ui->lineEditAracKodu->setText(mOwnerVehicle.mAracKodu);
    ui->lineEditKullanimTarzi->setText(mOwnerVehicle.mKullanimTarzi);
    ui->lineEditKullanimSinifi->setText(mOwnerVehicle.mKullanimSinifi);
    ui->lineEditPlaka->setText(mOwnerVehicle.mPlaka);
    ui->lineEditRuhsatSeriNo->setText(mOwnerVehicle.mRuhsatSeriNo);
    ui->comboBoxEngelliAracimi->setCurrentIndex(mOwnerVehicle.mEngelliAracimi);
}

void AddUpdateVehicleDialog::on_pushButtonSave_clicked()
{
    mOwnerVehicle.mMarka = ui->lineEditMarka->text();
    mOwnerVehicle.mTipi = ui->lineEditTipi->text();
    mOwnerVehicle.mAracKodu = ui->lineEditAracKodu->text();
    mOwnerVehicle.mKullanimTarzi = ui->lineEditKullanimTarzi->text();
    mOwnerVehicle.mKullanimSinifi = ui->lineEditKullanimSinifi->text();
    mOwnerVehicle.mPlaka = ui->lineEditPlaka->text();
    mOwnerVehicle.mRuhsatSeriNo = ui->lineEditRuhsatSeriNo->text();
    mOwnerVehicle.mEngelliAracimi = ui->comboBoxEngelliAracimi->currentIndex();
    mDatabaseManager->addUpdateVehicle(mOwnerVehicle);
    emit vehicleListUpdated();
    this->hide();
}
