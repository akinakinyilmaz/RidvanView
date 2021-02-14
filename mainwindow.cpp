#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QProgressDialog>
#include<QMessageBox>
#include<QDateTime>
#define SETTING_UPCOMING_POLICIES_ALARM_DAY_COUNT "SETTING_UPCOMING_POLICIES_ALARM_DAY_COUNT"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mDatabaseManager = new DatabaseManager();
    mDatabaseManager->connectToDatabase("./Database/database.db");
    if(!mDatabaseManager->isConnected())
    {
        qDebug("Veritabanına bağlanılamadı");
        return;
    }
    mAddUpdatePersonDialog = new AddUpdatePersonDialog(this);
    QObject::connect(mAddUpdatePersonDialog,SIGNAL(personListUpdated()),this,SLOT(onPersonListUpdated()));
    mAddUpdatePersonDialog->setDatabaseManager(mDatabaseManager);
    mPersonDetailsDialog = new PersonDetails(this);
    QObject::connect(mPersonDetailsDialog,SIGNAL(personUpdated()),this,SLOT(onPersonListUpdated()));
    mPersonDetailsDialog->setDatabaseManager(mDatabaseManager);
    mProgramSettings =new QSettings("./programSettings.ini",QSettings::IniFormat);
    mUpcomingPoliciesAlarmDayCount = mProgramSettings->value(SETTING_UPCOMING_POLICIES_ALARM_DAY_COUNT,"15").toInt();
    ui->spinBoxAlarmDayCount->blockSignals(true);
    ui->spinBoxAlarmDayCount->setValue(mUpcomingPoliciesAlarmDayCount);
    ui->spinBoxAlarmDayCount->blockSignals(false);
    fillUpcomingPolicies();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillUpcomingPolicies()
{
    ui->tableWidgetUpcomingPolicies->clearContents();
    ui->tableWidgetUpcomingPolicies->setRowCount(0);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDateTime dueDateTime = currentDateTime.addDays(mUpcomingPoliciesAlarmDayCount);
    int currentRowCount = 0;
    QList<TrafficPolicy> upcomingTrafficPolicies = mDatabaseManager->getTrafficPoliciesBetweenDates(currentDateTime,dueDateTime);
    for(int i=0;i<upcomingTrafficPolicies.size();i++)
    {
        if(upcomingTrafficPolicies.at(i).mIsActive == false)
            continue;
        ui->tableWidgetUpcomingPolicies->setRowCount(currentRowCount+1);
        Vehicle ownerVehicle = mDatabaseManager->getVehicleWithId(upcomingTrafficPolicies.at(i).mOwnerVehicleId);
        Person ownerPerson = mDatabaseManager->getPersonWithId(ownerVehicle.mOwnerPersonId);
        QTableWidgetItem* nameSurnameItem = new QTableWidgetItem();
        nameSurnameItem->setText(ownerPerson.mName + " " + ownerPerson.mSurname);
        nameSurnameItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,0,nameSurnameItem);
        QTableWidgetItem* phoneNumberItem = new QTableWidgetItem();
        phoneNumberItem->setText(ownerPerson.mPhoneNumber);
        phoneNumberItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,1,phoneNumberItem);
        QTableWidgetItem* policyInfoItem = new QTableWidgetItem();
        QString policyInfoStr = "Türü: Trafik Poliçesi\nPlaka: " + ownerVehicle.mPlaka + "\nKasko mu?: " +
                (upcomingTrafficPolicies.at(i).mKaskomu ? "Evet" : "Hayır");
        policyInfoItem->setText(policyInfoStr);
        policyInfoItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,2,policyInfoItem);
        QTableWidgetItem* dueDateItem = new QTableWidgetItem();
        dueDateItem->setText(QDateTime::fromString(upcomingTrafficPolicies.at(i).mBitisTarihi,DATE_FORMAT).toString(VISUAL_DATE_FORMAT));
        dueDateItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,3,dueDateItem);
        currentRowCount++;
    }
    QList<HealthPolicy> upcomingHealthPolicies = mDatabaseManager->getHealthPoliciesBetweenDates(currentDateTime,dueDateTime);
    for(int i=0;i<upcomingHealthPolicies.size();i++)
    {
        if(upcomingHealthPolicies.at(i).mIsActive == false)
            continue;
        ui->tableWidgetUpcomingPolicies->setRowCount(currentRowCount+1);
        Person ownerPerson = mDatabaseManager->getPersonWithId(upcomingHealthPolicies.at(i).mOwnerPersonId);
        QTableWidgetItem* nameSurnameItem = new QTableWidgetItem();
        nameSurnameItem->setText(ownerPerson.mName + " " + ownerPerson.mSurname);
        nameSurnameItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,0,nameSurnameItem);
        QTableWidgetItem* phoneNumberItem = new QTableWidgetItem();
        phoneNumberItem->setText(ownerPerson.mPhoneNumber);
        phoneNumberItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,1,phoneNumberItem);
        QTableWidgetItem* policyInfoItem = new QTableWidgetItem();
        QString policyInfoStr = "Türü: Sağlık Poliçesi";
        policyInfoItem->setText(policyInfoStr);
        policyInfoItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,2,policyInfoItem);
        QTableWidgetItem* dueDateItem = new QTableWidgetItem();
        dueDateItem->setText(QDateTime::fromString(upcomingHealthPolicies.at(i).mBitisTarihi,DATE_FORMAT).toString(VISUAL_DATE_FORMAT));
        dueDateItem->setData(Qt::UserRole,ownerPerson.mId);
        ui->tableWidgetUpcomingPolicies->setItem(currentRowCount,3,dueDateItem);
        currentRowCount++;
    }
    ui->tableWidgetUpcomingPolicies->resizeColumnsToContents();
}

void MainWindow::on_pushButtonAddPerson_clicked()
{
    mAddUpdatePersonDialog->setOwnerPerson(Person());
    mAddUpdatePersonDialog->show();
}

void MainWindow::on_pushButtonSearch_clicked()
{
    QProgressDialog progressDialog;
    progressDialog.setLabelText(tr("Aranıyor"));
    progressDialog.setValue(10);
    progressDialog.show();
    QString searchText = ui->lineEditSearchPersons->text();
    QList<Person> searchPersons = mDatabaseManager->getPersonsWithSearchText(searchText);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    for(int i=0;i<searchPersons.size();i++)
    {
        Person person = searchPersons.at(i);
        ui->tableWidget->setRowCount(i+1);
        QTableWidgetItem* tableWidgetNameItem = new QTableWidgetItem();
        tableWidgetNameItem->setText(person.mName + " " + person.mSurname);
        tableWidgetNameItem->setData(Qt::UserRole,person.mId);
        ui->tableWidget->setItem(i,0,tableWidgetNameItem);
        QList<Vehicle> personVehicles = mDatabaseManager->getVehiclesWithOwnerId(person.mId);
        QList<TrafficPolicy> trafficPolicies;
        for (int j=0; j < personVehicles.size() ; j++ ) {
                QList<TrafficPolicy> trafficPoliciesOfVehicle = mDatabaseManager->getTrafficPoliciesWithOwnerVehicleId(personVehicles.at(j).mId);
                trafficPolicies.append(trafficPoliciesOfVehicle);
        }
        QList<HealthPolicy> healthPolicies = mDatabaseManager->getHealthPoliciesWithOwnerId(person.mId);

        QTableWidgetItem* tableWidgetTrafficPolicyCount = new QTableWidgetItem();
        tableWidgetTrafficPolicyCount->setText(QString::number(trafficPolicies.size()));
        tableWidgetTrafficPolicyCount->setData(Qt::UserRole,person.mId);
        ui->tableWidget->setItem(i,1,tableWidgetTrafficPolicyCount);

        QTableWidgetItem* tableWidgetHealthPolicyCount = new QTableWidgetItem();
        tableWidgetHealthPolicyCount->setText(QString::number(healthPolicies.size()));
        tableWidgetHealthPolicyCount->setData(Qt::UserRole,person.mId);
        ui->tableWidget->setItem(i,2,tableWidgetHealthPolicyCount);

        bool atLeastOnePolicyFound = false;
        TrafficPolicy earliestTrafficPolicy;     
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QDateTime earliestTrafficPolicyEndDate = currentDateTime.addYears(10);
        for (int j=0; j<trafficPolicies.size();j++ )
        {
            if(trafficPolicies.at(j).mIsActive == false)
                continue;
            QDateTime trafficPolicyStartDate = QDateTime::fromString(trafficPolicies.at(j).mBaslangicTarihi,DATE_FORMAT);
            QDateTime trafficPolicyEndDate = QDateTime::fromString(trafficPolicies.at(j).mBitisTarihi,DATE_FORMAT);
            if(trafficPolicyEndDate >= currentDateTime )
            {
                if(earliestTrafficPolicy.mId < 0 || trafficPolicyEndDate < earliestTrafficPolicyEndDate)
                {
                    earliestTrafficPolicy = trafficPolicies.at(j);
                    earliestTrafficPolicyEndDate = trafficPolicyEndDate;
                    atLeastOnePolicyFound = true;
                }
            }
        }
        HealthPolicy earliestHealthPolicy;
        QDateTime earliestHealthPolicyEndDate = currentDateTime.addYears(10);
        for (int j=0; j<healthPolicies.size();j++ )
        {
            if(healthPolicies.at(j).mIsActive == false)
                continue;
            QDateTime healthPolicyStartDate = QDateTime::fromString(healthPolicies.at(j).mBaslangicTarihi,DATE_FORMAT);
            QDateTime healthPolicyEndDate =  QDateTime::fromString(healthPolicies.at(j).mBitisTarihi,DATE_FORMAT);
            if(healthPolicyEndDate >= currentDateTime )
            {
                if(earliestHealthPolicy.mId < 0 || healthPolicyEndDate < earliestHealthPolicyEndDate)
                {
                    earliestHealthPolicy = healthPolicies.at(j);
                    earliestHealthPolicyEndDate = healthPolicyEndDate;
                    atLeastOnePolicyFound = true;
                }
            }
        }
        QString earliestPolicyStr = "-";
        if(atLeastOnePolicyFound)
        {
            QDateTime dateTimeToUse = earliestHealthPolicyEndDate < earliestTrafficPolicyEndDate ? earliestHealthPolicyEndDate : earliestTrafficPolicyEndDate;
            earliestPolicyStr = dateTimeToUse.toString(VISUAL_DATE_FORMAT);
        }
        QTableWidgetItem* earliestPolicyTableWidgetItem = new QTableWidgetItem();
        earliestPolicyTableWidgetItem->setText(earliestPolicyStr);
        earliestPolicyTableWidgetItem->setData(Qt::UserRole,person.mId);
        ui->tableWidget->setItem(i,3,earliestPolicyTableWidgetItem);
    }
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_pushButtonEditPerson_clicked()
{
    if(ui->tableWidget->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidget->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            Person selectedPerson = mDatabaseManager->getPersonWithId(selectedItemData);
            if(selectedPerson.mId>0)
            {
                mAddUpdatePersonDialog->setOwnerPerson(selectedPerson);
                mAddUpdatePersonDialog->show();
            }
        }
    }
}

void MainWindow::on_pushButtonDeletePerson_clicked()
{
    if(ui->tableWidget->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidget->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            Person selectedPerson = mDatabaseManager->getPersonWithId(selectedItemData);
            if(selectedPerson.mId>0)
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Silme İşlemi", "Silmek İstediğinize Emin misiniz?",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    mDatabaseManager->deletePerson(selectedPerson.mId);
                    on_pushButtonSearch_clicked();
                    fillUpcomingPolicies();
                }
            }
        }
    }
}

void MainWindow::onPersonListUpdated()
{
    on_pushButtonSearch_clicked();
    fillUpcomingPolicies();
}

void MainWindow::on_pushButtonDetails_clicked()
{
    if(ui->tableWidget->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidget->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            Person selectedPerson = mDatabaseManager->getPersonWithId(selectedItemData);
            if(selectedPerson.mId>0)
            {
                mPersonDetailsDialog->setOwnerPerson(selectedPerson);
                mPersonDetailsDialog->show();
            }
        }
    }
}

void MainWindow::on_pushButtonUpcomingPoliciesSeeDetails_clicked()
{
    if(ui->tableWidgetUpcomingPolicies->selectedItems().size()>0)
    {
        QTableWidgetItem* selectedItem = ui->tableWidgetUpcomingPolicies->selectedItems().at(0);
        int selectedItemData = selectedItem->data(Qt::UserRole).toInt();
        if(selectedItemData > 0)
        {
            Person selectedPerson = mDatabaseManager->getPersonWithId(selectedItemData);
            if(selectedPerson.mId>0)
            {
                mPersonDetailsDialog->setOwnerPerson(selectedPerson);
                mPersonDetailsDialog->show();
            }
        }
    }
}

void MainWindow::on_pushButtonSaveSettings_clicked()
{
    mUpcomingPoliciesAlarmDayCount = ui->spinBoxAlarmDayCount->value();
    mProgramSettings->setValue(SETTING_UPCOMING_POLICIES_ALARM_DAY_COUNT,mUpcomingPoliciesAlarmDayCount);
    fillUpcomingPolicies();
}


