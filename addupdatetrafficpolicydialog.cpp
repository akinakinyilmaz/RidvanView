#include "addupdatetrafficpolicydialog.h"
#include "ui_addupdatetrafficpolicydialog.h"
using namespace RidvanView;
AddUpdateTrafficPolicyDialog::AddUpdateTrafficPolicyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUpdateTrafficPolicyDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
}

AddUpdateTrafficPolicyDialog::~AddUpdateTrafficPolicyDialog()
{
    delete ui;
}

void AddUpdateTrafficPolicyDialog::setDatabaseManager(DatabaseManager *dbManager)
{
    mDatabaseManager= dbManager;
}

void AddUpdateTrafficPolicyDialog::setOwnerTrafficPolicyAndPerson(TrafficPolicy trafficPolicy,Person person)
{
    mOwnerTrafficPolicy = trafficPolicy;
    mOwnerPerson = person;
    QDateTime policyStartDate = QDateTime::currentDateTime();
    QDateTime policyEndDate = policyStartDate.addYears(1);
    bool kaskomu = false;
    bool isActive = true;
    QList<Vehicle> vehicleList = mDatabaseManager->getVehiclesWithOwnerId(mOwnerPerson.mId);
    ui->comboBoxVehicles->clear();
    for (int i=0;i<vehicleList.size() ;i++ )
    {
        ui->comboBoxVehicles->addItem(vehicleList.at(i).mPlaka,vehicleList.at(i).mId);
        if(trafficPolicy.mId > 0 && trafficPolicy.mOwnerVehicleId == vehicleList.at(i).mId)
        {
            ui->comboBoxVehicles->setCurrentIndex(i);
        }
    }
    if(trafficPolicy.mId > 0)
    {
        policyStartDate = QDateTime::fromString(mOwnerTrafficPolicy.mBaslangicTarihi,DATE_FORMAT);
        policyEndDate = QDateTime::fromString(mOwnerTrafficPolicy.mBitisTarihi,DATE_FORMAT);
        kaskomu = mOwnerTrafficPolicy.mKaskomu;
        isActive = mOwnerTrafficPolicy.mIsActive;
    }
    ui->dateEditStartDate->setDateTime(policyStartDate);
    ui->dateEditEndDate->setDateTime(policyEndDate);
    ui->lineEditPolicyCompany->setText(mOwnerTrafficPolicy.mSigortaSirketi);
    ui->checkBoxKaskomu->setChecked(kaskomu);
    ui->checkBoxIsActive->setChecked(isActive);
}


void AddUpdateTrafficPolicyDialog::on_pushButtonSave_clicked()
{
    mOwnerTrafficPolicy.mBaslangicTarihi = ui->dateEditStartDate->dateTime().toString(DATE_FORMAT);
    mOwnerTrafficPolicy.mBitisTarihi = ui->dateEditEndDate->dateTime().toString(DATE_FORMAT);
    mOwnerTrafficPolicy.mSigortaSirketi = ui->lineEditPolicyCompany->text();
    mOwnerTrafficPolicy.mKaskomu = ui->checkBoxKaskomu->isChecked();
    mOwnerTrafficPolicy.mOwnerVehicleId = ui->comboBoxVehicles->currentData().toInt();
    mOwnerTrafficPolicy.mIsActive = ui->checkBoxIsActive->isChecked();
    mDatabaseManager->addUpdateTrafficPolicy(mOwnerTrafficPolicy);
    emit trafficPolicyUpdated();
    this->hide();
}
