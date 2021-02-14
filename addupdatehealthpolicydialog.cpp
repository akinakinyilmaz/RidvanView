#include "addupdatehealthpolicydialog.h"
#include "ui_addupdatehealthpolicydialog.h"
using namespace RidvanView;
AddUpdateHealthPolicyDialog::AddUpdateHealthPolicyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUpdateHealthPolicyDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
}

AddUpdateHealthPolicyDialog::~AddUpdateHealthPolicyDialog()
{
    delete ui;
}

void AddUpdateHealthPolicyDialog::setOwnerHealthPolicyAndPerson(RidvanView::HealthPolicy healthPolicy,Person person)
{
    mOwnerHealthPolicy = healthPolicy;
    mOwnerPerson = person;
    ui->lineEditDigerSigortaliTc->setText(mOwnerHealthPolicy.mDigerSigortaliTc);
    ui->lineEditDigerSigortali2Tc->setText(mOwnerHealthPolicy.mDigerSigortaliTc2);
    ui->lineEditDigerSigortali3Tc->setText(mOwnerHealthPolicy.mDigerSigortaliTc3);
    ui->lineEditHastalikBeyani->setText(mOwnerHealthPolicy.mHastalikBeyani);
    ui->lineEditSigortaSirketi->setText(mOwnerHealthPolicy.mSigortaSirketi);
    ui->lineEditTeminatlar->setText(mOwnerHealthPolicy.mTeminatlar);
    QDateTime policyStartDate = QDateTime::currentDateTime();
    QDateTime policyEndDate = policyStartDate.addYears(1);
    bool isActive = true;
    if(mOwnerHealthPolicy.mId>0)
    {
        policyStartDate = QDateTime::fromString(mOwnerHealthPolicy.mBaslangicTarihi,DATE_FORMAT);
        policyEndDate = QDateTime::fromString(mOwnerHealthPolicy.mBitisTarihi,DATE_FORMAT);
        isActive = mOwnerHealthPolicy.mIsActive;
    }
    ui->dateEditBaslangicTarihi->setDateTime(policyStartDate);
    ui->dateEditBitisTarihi->setDateTime(policyEndDate);
    ui->checkBoxIsActive->setChecked(isActive);
}

void AddUpdateHealthPolicyDialog::setDatabaseManager(DatabaseManager *dbManager)
{
    mDatabaseManager = dbManager;
}

void AddUpdateHealthPolicyDialog::on_pushButtonSave_clicked()
{
    mOwnerHealthPolicy.mDigerSigortaliTc = ui->lineEditDigerSigortaliTc->text();
    mOwnerHealthPolicy.mDigerSigortaliTc2 = ui->lineEditDigerSigortali2Tc->text();
    mOwnerHealthPolicy.mDigerSigortaliTc3 = ui->lineEditDigerSigortali3Tc->text();
    mOwnerHealthPolicy.mHastalikBeyani = ui->lineEditHastalikBeyani->text();
    mOwnerHealthPolicy.mSigortaSirketi = ui->lineEditSigortaSirketi->text();
    mOwnerHealthPolicy.mTeminatlar = ui->lineEditTeminatlar->text();
    mOwnerHealthPolicy.mBaslangicTarihi = ui->dateEditBaslangicTarihi->dateTime().toString(DATE_FORMAT);
    mOwnerHealthPolicy.mBitisTarihi =ui->dateEditBitisTarihi->dateTime().toString(DATE_FORMAT);
    mOwnerHealthPolicy.mOwnerPersonId = mOwnerPerson.mId;
    mOwnerHealthPolicy.mIsActive = ui->checkBoxIsActive->isChecked();
    mDatabaseManager->addUpdateHealthPolicy(mOwnerHealthPolicy);
    emit healthPolicyListUpdated();
    this->hide();
}
