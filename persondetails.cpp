#include "persondetails.h"
#include "ui_persondetails.h"
#include<QMessageBox>
#include<QDateTime>
using namespace RidvanView;
PersonDetails::PersonDetails(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonDetails)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    mAddUpdateVehicleDialog = new AddUpdateVehicleDialog(this);
    QObject::connect(mAddUpdateVehicleDialog,SIGNAL(vehicleListUpdated()),this,SLOT(onVehicleListUpdated()));
    mAddUpdateTrafficPolicyDialog = new AddUpdateTrafficPolicyDialog(this);
    QObject::connect(mAddUpdateTrafficPolicyDialog,SIGNAL(trafficPolicyUpdated()),this,SLOT(onTrafficPolicyListUpdated()));
    mAddUpdateHealthPolicyDialog = new AddUpdateHealthPolicyDialog(this);
    QObject::connect(mAddUpdateHealthPolicyDialog,SIGNAL(healthPolicyListUpdated()),this,SLOT(onHealthPolicyListUpdated()));
    ui->toolBoxPolicies->setCurrentIndex(0);
}

PersonDetails::~PersonDetails()
{
    delete ui;
}

void PersonDetails::setDatabaseManager(DatabaseManager *dbManager)
{
    mDatabaseManager = dbManager;
    mAddUpdateVehicleDialog->setDatabaseManager(dbManager);
    mAddUpdateTrafficPolicyDialog->setDatabaseManager(dbManager);
    mAddUpdateHealthPolicyDialog->setDatabaseManager(dbManager);
}

void PersonDetails::setOwnerPerson(Person person)
{
    mOwnerPerson = person;
    ui->labelPersonDetails->setText("İsim-Soyisim: " + mOwnerPerson.mName + " " + mOwnerPerson.mSurname + "\n"
+ "TC Kimlik Numarası: " + mOwnerPerson.mRegisterNo + "\n"
+ "Telefon Numarası: " + mOwnerPerson.mPhoneNumber);
    updateVehiclesSegment();
    updateTrafficPoliciesSegment();
    updateHealthPoliciesSegment();
}

void PersonDetails::updateVehiclesSegment()
{
    ui->tableWidgetVehicles->clearContents();
    ui->tableWidgetVehicles->setRowCount(0);
    QList<Vehicle> vehicleList = mDatabaseManager->getVehiclesWithOwnerId(mOwnerPerson.mId);
    for (int i=0;i<vehicleList.size() ;i++ )
    {
        Vehicle currentVehicle = vehicleList.at(i);
        ui->tableWidgetVehicles->setRowCount(i+1);
        QTableWidgetItem* markaItem = new QTableWidgetItem();
        markaItem->setText(currentVehicle.mMarka);
        markaItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,0,markaItem);
        QTableWidgetItem* plakaItem = new QTableWidgetItem();
        plakaItem->setText(currentVehicle.mPlaka);
        plakaItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,1,plakaItem);
        QTableWidgetItem* tipItem = new QTableWidgetItem();
        tipItem->setText(currentVehicle.mTipi);
        tipItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,2,tipItem);
        QTableWidgetItem* aracKoduItem = new QTableWidgetItem();
        aracKoduItem->setText(currentVehicle.mAracKodu);
        aracKoduItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,3,aracKoduItem);
        QTableWidgetItem* kullanimTarziItem = new QTableWidgetItem();
        kullanimTarziItem->setText(currentVehicle.mKullanimTarzi);
        kullanimTarziItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,4,kullanimTarziItem);
        QTableWidgetItem* kullanimSinifiItem = new QTableWidgetItem();
        kullanimSinifiItem->setText(currentVehicle.mKullanimSinifi);
        kullanimSinifiItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,5,kullanimSinifiItem);
        QTableWidgetItem* ruhsatSeriNoItem = new QTableWidgetItem();
        ruhsatSeriNoItem->setText(currentVehicle.mRuhsatSeriNo);
        ruhsatSeriNoItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,6,ruhsatSeriNoItem);
        QTableWidgetItem* engelliAracimiItem = new QTableWidgetItem();
        engelliAracimiItem->setText(currentVehicle.mEngelliAracimi? "Evet" : "Hayır");
        engelliAracimiItem->setData(Qt::UserRole,currentVehicle.mId);
        ui->tableWidgetVehicles->setItem(i,7,engelliAracimiItem);
    }
    ui->tableWidgetVehicles->resizeColumnsToContents();
}

void PersonDetails::updateTrafficPoliciesSegment()
{
    ui->tableWidgetTrafficPolicies->clearContents();
    ui->tableWidgetTrafficPolicies->setRowCount(0);
    QList<Vehicle> personVehicles = mDatabaseManager->getVehiclesWithOwnerId(mOwnerPerson.mId);
    int currentRowCount = 0;
    for(int i=0;i<personVehicles.size();i++)
    {
        QList<TrafficPolicy> vehicleTrafficPolicies = mDatabaseManager->getTrafficPoliciesWithOwnerVehicleId(personVehicles.at(i).mId);
        QString aracPlakasi = personVehicles.at(i).mPlaka;
        for(int j=0;j<vehicleTrafficPolicies.size();j++)
        {
            TrafficPolicy currentTrafficPolicy = vehicleTrafficPolicies.at(j);
            ui->tableWidgetTrafficPolicies->setRowCount(currentRowCount+1);
            QTableWidgetItem* plakaItem = new QTableWidgetItem();
            plakaItem->setText(aracPlakasi);
            plakaItem->setData(Qt::UserRole,currentTrafficPolicy.mId);
            ui->tableWidgetTrafficPolicies->setItem(currentRowCount,0,plakaItem);
            QDateTime baslangicTarihi = QDateTime::fromString(currentTrafficPolicy.mBaslangicTarihi,DATE_FORMAT);
            QDateTime bitisTarihi =  QDateTime::fromString(currentTrafficPolicy.mBitisTarihi,DATE_FORMAT);
            QTableWidgetItem* baslangicTarihiItem = new QTableWidgetItem();
            baslangicTarihiItem->setText(baslangicTarihi.toString(VISUAL_DATE_FORMAT));
            baslangicTarihiItem->setData(Qt::UserRole,currentTrafficPolicy.mId);
            ui->tableWidgetTrafficPolicies->setItem(currentRowCount,1,baslangicTarihiItem);
            QTableWidgetItem* bitisTarihiItem = new QTableWidgetItem();
            bitisTarihiItem->setText(bitisTarihi.toString(VISUAL_DATE_FORMAT));
            bitisTarihiItem->setData(Qt::UserRole,currentTrafficPolicy.mId);
            ui->tableWidgetTrafficPolicies->setItem(currentRowCount,2,bitisTarihiItem);
            QTableWidgetItem* sigortaSirketiItem = new QTableWidgetItem();
            sigortaSirketiItem->setText(currentTrafficPolicy.mSigortaSirketi);
            sigortaSirketiItem->setData(Qt::UserRole,currentTrafficPolicy.mId);
            ui->tableWidgetTrafficPolicies->setItem(currentRowCount,3,sigortaSirketiItem);
            QTableWidgetItem* kaskomuItem = new QTableWidgetItem();
            kaskomuItem->setText(currentTrafficPolicy.mKaskomu ? "Evet" : "Hayır");
            kaskomuItem->setData(Qt::UserRole,currentTrafficPolicy.mId);
            ui->tableWidgetTrafficPolicies->setItem(currentRowCount,4,kaskomuItem);
            QTableWidgetItem* isActiveItem = new QTableWidgetItem();
            isActiveItem->setText(currentTrafficPolicy.mIsActive ? "Evet" : "Hayır");
            isActiveItem->setData(Qt::UserRole,currentTrafficPolicy.mId);
            ui->tableWidgetTrafficPolicies->setItem(currentRowCount,5,isActiveItem);
            currentRowCount++;
        }
    }
    ui->tableWidgetTrafficPolicies->resizeColumnsToContents();
}

void PersonDetails::updateHealthPoliciesSegment()
{
    ui->tableWidgetHealthPolicies->clearContents();
    ui->tableWidgetHealthPolicies->setRowCount(0);
    QList<HealthPolicy> personHealthPolicyList = mDatabaseManager->getHealthPoliciesWithOwnerId(mOwnerPerson.mId);
    for(int i=0;i<personHealthPolicyList.size();i++)
    {
        ui->tableWidgetHealthPolicies->setRowCount(i+1);
        QTableWidgetItem* digerSigortaliTcItem = new QTableWidgetItem();
        digerSigortaliTcItem->setText(personHealthPolicyList.at(i).mDigerSigortaliTc);
        digerSigortaliTcItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,0,digerSigortaliTcItem);
        QTableWidgetItem* digerSigortali2TcItem = new QTableWidgetItem();
        digerSigortali2TcItem->setText(personHealthPolicyList.at(i).mDigerSigortaliTc2);
        digerSigortali2TcItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,1,digerSigortali2TcItem);
        QTableWidgetItem* digerSigortali3TcItem = new QTableWidgetItem();
        digerSigortali3TcItem->setText(personHealthPolicyList.at(i).mDigerSigortaliTc3);
        digerSigortali3TcItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,2,digerSigortali3TcItem);
        QTableWidgetItem* hastalikBeyaniItem = new QTableWidgetItem();
        hastalikBeyaniItem->setText(personHealthPolicyList.at(i).mHastalikBeyani);
        hastalikBeyaniItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,3,hastalikBeyaniItem);
        QTableWidgetItem* sigortaSirketiItem = new QTableWidgetItem();
        sigortaSirketiItem->setText(personHealthPolicyList.at(i).mSigortaSirketi);
        sigortaSirketiItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,4,sigortaSirketiItem);
        QTableWidgetItem* teminatlarItem = new QTableWidgetItem();
        teminatlarItem->setText(personHealthPolicyList.at(i).mTeminatlar);
        teminatlarItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,5,teminatlarItem);
        QTableWidgetItem* baslangicTarihiItem = new QTableWidgetItem();
        baslangicTarihiItem->setText(QDateTime::fromString(personHealthPolicyList.at(i).mBaslangicTarihi,DATE_FORMAT).toString(VISUAL_DATE_FORMAT));
        baslangicTarihiItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,6,baslangicTarihiItem);
        QTableWidgetItem* bitisTarihiItem = new QTableWidgetItem();
        bitisTarihiItem->setText(QDateTime::fromString(personHealthPolicyList.at(i).mBitisTarihi,DATE_FORMAT).toString(VISUAL_DATE_FORMAT));
        bitisTarihiItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,7,bitisTarihiItem);
        QTableWidgetItem* isActiveItem = new QTableWidgetItem();
        isActiveItem->setText(personHealthPolicyList.at(i).mIsActive ? "Evet" : "Hayır");
        isActiveItem->setData(Qt::UserRole,personHealthPolicyList.at(i).mId);
        ui->tableWidgetHealthPolicies->setItem(i,8,isActiveItem);
    }
    ui->tableWidgetHealthPolicies->resizeColumnsToContents();
}

void PersonDetails::on_pushButtonUpdate_clicked()
{
    emit personUpdated();
    this->hide();
}

void PersonDetails::on_pushButtonAddVehicle_clicked()
{
    Vehicle vehicleToAdd;
    vehicleToAdd.mOwnerPersonId = mOwnerPerson.mId;
    mAddUpdateVehicleDialog->setOwnerVehicle(vehicleToAdd);
    mAddUpdateVehicleDialog->show();
}

void PersonDetails::on_pushButtonEditVehicle_clicked()
{
    if(ui->tableWidgetVehicles->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidgetVehicles->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            Vehicle selectedVehicle = mDatabaseManager->getVehicleWithId(selectedItemData);
            if(selectedVehicle.mId>0)
            {
                mAddUpdateVehicleDialog->setOwnerVehicle(selectedVehicle);
                mAddUpdateVehicleDialog->show();
            }
        }
    }
}

void PersonDetails::on_pushButtonDeleteVehicle_clicked()
{
    if(ui->tableWidgetVehicles->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidgetVehicles->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            Vehicle selectedVehicle = mDatabaseManager->getVehicleWithId(selectedItemData);
            if(selectedVehicle.mId>0)
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Silme İşlemi", "Silmek İstediğinize Emin misiniz?",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    mDatabaseManager->deleteVehicle(selectedVehicle.mId);
                    onVehicleListUpdated();
                    onTrafficPolicyListUpdated();
                }
            }
        }
    }
}

void PersonDetails::onVehicleListUpdated()
{
    updateVehiclesSegment();
}

void PersonDetails::on_pushButtonAddTrafficPolicy_clicked()
{
    TrafficPolicy trafficPolicyToAdd;
    mAddUpdateTrafficPolicyDialog->setOwnerTrafficPolicyAndPerson(trafficPolicyToAdd,mOwnerPerson);
    mAddUpdateTrafficPolicyDialog->show();
}

void PersonDetails::on_pushButtonEditTrafficPolicy_clicked()
{
    if(ui->tableWidgetTrafficPolicies->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidgetTrafficPolicies->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            TrafficPolicy selectedTrafficPolicy = mDatabaseManager->getTrafficPolicyWithId(selectedItemData);
            if(selectedTrafficPolicy.mId>0)
            {
                mAddUpdateTrafficPolicyDialog->setOwnerTrafficPolicyAndPerson(selectedTrafficPolicy,mOwnerPerson);
                mAddUpdateTrafficPolicyDialog->show();
            }
        }
    }
}

void PersonDetails::on_pushButtonDeleteTrafficPolicy_clicked()
{
    if(ui->tableWidgetTrafficPolicies->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidgetTrafficPolicies->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            TrafficPolicy selectedTrafficPolicy = mDatabaseManager->getTrafficPolicyWithId(selectedItemData);
            if(selectedTrafficPolicy.mId>0)
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Silme İşlemi", "Silmek İstediğinize Emin misiniz?",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    mDatabaseManager->deleteTrafficPolicy(selectedItemData);
                    onTrafficPolicyListUpdated();
                }
            }
        }
    }
}

void PersonDetails::onTrafficPolicyListUpdated()
{
    updateTrafficPoliciesSegment();
}

void PersonDetails::on_pushButtonAddHealthPolicies_clicked()
{
    mAddUpdateHealthPolicyDialog->setOwnerHealthPolicyAndPerson(HealthPolicy(),mOwnerPerson);
    mAddUpdateHealthPolicyDialog->show();
}

void PersonDetails::on_pushButtonEditHealthPolicy_clicked()
{
    if(ui->tableWidgetHealthPolicies->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidgetHealthPolicies->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            HealthPolicy selectedHealthPolicy = mDatabaseManager->getHealthPolicyWithId(selectedItemData);
            if(selectedHealthPolicy.mId>0)
            {
                mAddUpdateHealthPolicyDialog->setOwnerHealthPolicyAndPerson(selectedHealthPolicy,mOwnerPerson);
                mAddUpdateHealthPolicyDialog->show();
            }
        }
    }
}

void PersonDetails::on_pushButtonDeleteHealthPolicy_clicked()
{
    if(ui->tableWidgetHealthPolicies->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidgetHealthPolicies->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            HealthPolicy selectedHealthPolicy = mDatabaseManager->getHealthPolicyWithId(selectedItemData);
            if(selectedHealthPolicy.mId>0)
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Silme İşlemi", "Silmek İstediğinize Emin misiniz?",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    mDatabaseManager->deleteHealthPolicy(selectedItemData);
                    updateHealthPoliciesSegment();
                }
            }
        }
    }
}

void PersonDetails::onHealthPolicyListUpdated()
{
    updateHealthPoliciesSegment();
}
