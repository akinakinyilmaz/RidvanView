#include "DatabaseManager.h"
#include<QtSql\qsqlerror.h>
#include<QFile>
using namespace RidvanView;
DatabaseManager::DatabaseManager(QString dbFileName):
	mDatabaseOpenedStatus(false)
{
	if (dbFileName.size() > 0)
	{
		connectToDatabase(dbFileName);
	}
}
DatabaseManager::~DatabaseManager()
{
}
bool DatabaseManager::connectToDatabase(QString dbFileName)
{
	mDBFileName = dbFileName;
    mSqlDatabase = QSqlDatabase::addDatabase("QSQLITE","RidvanView");
	mSqlDatabase.setDatabaseName(dbFileName);
	mDatabaseOpenedStatus = mSqlDatabase.open();
	return mDatabaseOpenedStatus;
}

void DatabaseManager::closeDatabaseConnection()
{
	if (mDatabaseOpenedStatus)
	{
		mSqlDatabase.close();
		mDatabaseOpenedStatus = false;
	}
}

bool DatabaseManager::isConnected()
{
	return mDatabaseOpenedStatus;
}

QSqlDatabase DatabaseManager::getSqlDatabase()
{
	return mSqlDatabase;
}

void DatabaseManager::setDefaultDBName(QString defaultDbName)
{
	mDefaultDBFileName = defaultDbName;
}

QString DatabaseManager::getDefaultDbName()
{
	return mDefaultDBFileName;
}

QString DatabaseManager::getDBName()
{
	return mDBFileName;
}

bool DatabaseManager::backUpCurrentDatabase(QString databaseBackUpPath)
{
	this->closeDatabaseConnection();
	QFile dbFileInstance(mDBFileName);
	bool copyStatus = false;
	copyStatus =  dbFileInstance.copy(databaseBackUpPath);
	this->connectToDatabase(mDBFileName);
	return copyStatus;
}

bool DatabaseManager::resetCurrentDatabase()
{
	this->closeDatabaseConnection();
	QFile dbFileInstance(mDBFileName);
	bool resetStatus = false;
	resetStatus = dbFileInstance.remove();
	if (resetStatus)
	{
		QFile defaultDbFileInstance(mDefaultDBFileName);
		resetStatus = defaultDbFileInstance.copy(mDBFileName);
		if (resetStatus)
		{
			this->connectToDatabase(mDBFileName);
		}
	}
	return resetStatus;
}

bool DatabaseManager::restoreBackupDatabase(QString dbFile)
{
	this->closeDatabaseConnection();
	QFile dbFileInstance(mDBFileName);
	bool resetStatus = false;
	resetStatus = dbFileInstance.remove();
	if (resetStatus)
	{
		QFile defaultDbFileInstance(dbFile);
		resetStatus = defaultDbFileInstance.copy(mDBFileName);
		if (resetStatus)
		{
			this->connectToDatabase(mDBFileName);
		}
	}
	return resetStatus;
}


Person DatabaseManager::getPersonWithId(int personId)
{
	Person person;
	person.mId = -1;
	if (mSqlDatabase.isOpen())
	{
		QSqlQuery qry(mSqlDatabase);
		qry.prepare("SELECT * FROM Persons WHERE id=" + QString::number(personId));
		if (qry.exec() && qry.next())
		{
			getPersonFromSqlQuery(person, qry);
		}
	}
	return person;
}

Person DatabaseManager::getPersonWithRegisterNo(QString registerNo)
{
	Person person;
	person.mId = -1;
    if (mSqlDatabase.isOpen())
	{
		QSqlQuery qry(mSqlDatabase);
		qry.prepare("SELECT * FROM Persons WHERE RegisterNo='" + registerNo + "'");
		if (qry.exec() && qry.next())
		{
			getPersonFromSqlQuery(person, qry);
		}
    }
    return person;
}

QList<Person> DatabaseManager::getPersonsWithSearchText(QString searchText)
{
    QList<Person> personList;
    if(mSqlDatabase.isOpen())
    {
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM Persons WHERE RegisterNo LIKE '%"+ searchText + "%' OR Name LIKE '%"+ searchText + "%' OR Surname LIKE '%"+searchText+"%'");
        if(qry.exec())
        {
            while(qry.next())
            {
                Person person;
                getPersonFromSqlQuery(person,qry);
                personList.push_back(person);
            }
        }
    }
    return personList;
}

bool DatabaseManager::addUpdatePerson(Person person)
{
	if (mSqlDatabase.isOpen())
	{
		if (person.mId < 0)//add
		{
			QSqlQuery insertQuery(mSqlDatabase);
            insertQuery.prepare("INSERT INTO Persons (RegisterNo, Name, Surname, PhoneNumber) VALUES( ? , ? , ? , ?)");
			setPersonAddUpdateQueryBindValues(person, insertQuery);
			if (insertQuery.exec())
			{
				return true;
			}           
			return false;
		}
		else//update
		{
			QSqlQuery qry(mSqlDatabase);
            qry.prepare("UPDATE Persons SET RegisterNo = ? , Name= ? , Surname= ? , PhoneNumber= ?  WHERE id = ?");
			setPersonAddUpdateQueryBindValues(person, qry);
            qry.bindValue(4, person.mId);
			if (qry.exec())
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

bool DatabaseManager::deletePerson(int personId)
{
	QSqlQuery qry(mSqlDatabase);
	qry.prepare("DELETE FROM Persons WHERE id = ? ");
	qry.bindValue(0, personId);
	if (qry.exec())
	{
        QList<Vehicle> personVehicles = getVehiclesWithOwnerId(personId);
        for(int i=0;i<personVehicles.size();i++)
        {
            deleteVehicle(personVehicles.at(i).mId);
            QList<TrafficPolicy> vehiclesTrafficPolicies = getTrafficPoliciesWithOwnerVehicleId(personVehicles.at(i).mId);
            for(int j=0;j<vehiclesTrafficPolicies.size();j++)
            {
                deleteTrafficPolicy(vehiclesTrafficPolicies.at(j).mId);
            }
        }
        QList<HealthPolicy> personHealthPolicies = getHealthPoliciesWithOwnerId(personId);
        for(int i=0;i<personHealthPolicies.size();i++)
        {
            deleteHealthPolicy(personHealthPolicies.at(i).mId);
        }
		return true;
	}
	return false;
}

void DatabaseManager::getPersonFromSqlQuery(Person& personToFill, QSqlQuery& sqlQuery)
{
	personToFill.mId = sqlQuery.value(0).toInt();
	personToFill.mRegisterNo = sqlQuery.value(1).toString();
	personToFill.mName = sqlQuery.value(2).toString();
	personToFill.mSurname = sqlQuery.value(3).toString();
    personToFill.mPhoneNumber = sqlQuery.value(4).toString();
}

void DatabaseManager::setPersonAddUpdateQueryBindValues(Person person, QSqlQuery& query)
{
	query.bindValue(0, person.mRegisterNo);
	query.bindValue(1, person.mName);
	query.bindValue(2, person.mSurname);
    query.bindValue(3, person.mPhoneNumber);
}

Vehicle DatabaseManager::getVehicleWithId(int vehicleId)
{
    Vehicle vehicle;
    vehicle.mId = -1;
    if (mSqlDatabase.isOpen())
    {
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM Vehicles WHERE id=" + QString::number(vehicleId));
        if (qry.exec() && qry.next())
        {
            getVehicleFromSqlQuery(vehicle, qry);
        }
    }
    return vehicle;
}

QList<Vehicle> DatabaseManager::getVehiclesWithOwnerId(int ownerId)
{
    QList<Vehicle> returnVehicles;
    if(mSqlDatabase.isOpen())
    {
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM Vehicles WHERE OwnerPersonId=" + QString::number(ownerId));
        if(qry.exec())
        {
            while(qry.next())
            {
                Vehicle vehicle;
                getVehicleFromSqlQuery(vehicle, qry);
                returnVehicles.append(vehicle);
            }
        }
    }
    return returnVehicles;
}

bool DatabaseManager::addUpdateVehicle(Vehicle vehicle)
{
    if (mSqlDatabase.isOpen())
    {
        if (vehicle.mId < 0)//add
        {
            QSqlQuery insertQuery(mSqlDatabase);
            insertQuery.prepare("INSERT INTO Vehicles (Marka, Tipi, AracKodu, KullanimTarzi,KullanimSinifi,OwnerPersonId,Plaka,RuhsatSeriNo,EngelliAracimi) VALUES( ? , ? , ? , ?,?,?,?,?,?)");
            setVehicleAddUpdateQueryBindValues(vehicle, insertQuery);
            if (insertQuery.exec())
            {
                return true;
            }
            return false;
        }
        else//update
        {
            QSqlQuery qry(mSqlDatabase);
            qry.prepare("UPDATE Vehicles SET Marka = ? , Tipi= ? , AracKodu= ? , KullanimTarzi= ?,KullanimSinifi=?,OwnerPersonId=?,Plaka=?,RuhsatSeriNo=?,EngelliAracimi=?  WHERE id = ?");
            setVehicleAddUpdateQueryBindValues(vehicle, qry);
            qry.bindValue(9, vehicle.mId);
            if (qry.exec())
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

bool DatabaseManager::deleteVehicle(int vehicleId)
{
    QSqlQuery qry(mSqlDatabase);
    qry.prepare("DELETE FROM Vehicles WHERE id = ? ");
    qry.bindValue(0, vehicleId);
    if (qry.exec())
    {
        QList<TrafficPolicy> vehiclesTrafficPolicies = getTrafficPoliciesWithOwnerVehicleId(vehicleId);
        for(int j=0;j<vehiclesTrafficPolicies.size();j++)
        {
            deleteTrafficPolicy(vehiclesTrafficPolicies.at(j).mId);
        }
        return true;
    }
    return false;
}

void DatabaseManager::getVehicleFromSqlQuery(Vehicle &vehicleToFill, QSqlQuery &sqlQuery)
{
    vehicleToFill.mId = sqlQuery.value(0).toInt();
    vehicleToFill.mMarka = sqlQuery.value(1).toString();
    vehicleToFill.mTipi = sqlQuery.value(2).toString();
    vehicleToFill.mAracKodu = sqlQuery.value(3).toString();
    vehicleToFill.mKullanimTarzi = sqlQuery.value(4).toString();
    vehicleToFill.mKullanimSinifi = sqlQuery.value(5).toString();
    vehicleToFill.mOwnerPersonId = sqlQuery.value(6).toInt();
    vehicleToFill.mPlaka = sqlQuery.value(7).toString();
    vehicleToFill.mRuhsatSeriNo = sqlQuery.value(8).toString();
    vehicleToFill.mEngelliAracimi = sqlQuery.value(9).toBool();
}

void DatabaseManager::setVehicleAddUpdateQueryBindValues(Vehicle vehicle, QSqlQuery &query)
{
    query.bindValue(0, vehicle.mMarka);
    query.bindValue(1, vehicle.mTipi);
    query.bindValue(2, vehicle.mAracKodu);
    query.bindValue(3, vehicle.mKullanimTarzi);
    query.bindValue(4, vehicle.mKullanimSinifi);
    query.bindValue(5,vehicle.mOwnerPersonId);
    query.bindValue(6,vehicle.mPlaka);
    query.bindValue(7,vehicle.mRuhsatSeriNo);
    query.bindValue(8,vehicle.mEngelliAracimi);
}

TrafficPolicy DatabaseManager::getTrafficPolicyWithId(int trafficPolicyId)
{
    TrafficPolicy trafficPolicy;
    trafficPolicy.mId = -1;
    if (mSqlDatabase.isOpen())
    {
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM TrafficPolicies WHERE id=" + QString::number(trafficPolicyId));
        if (qry.exec() && qry.next())
        {
            getTrafficPolicyFromSqlQuery(trafficPolicy, qry);
        }
    }
    return trafficPolicy;
}

QList<TrafficPolicy> DatabaseManager::getTrafficPoliciesWithOwnerVehicleId(int ownerVehicleId)
{
    QList<TrafficPolicy> returnTrafficPolicies;
    if(mSqlDatabase.isOpen())
    {
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM TrafficPolicies WHERE OwnerVehicleId=" + QString::number(ownerVehicleId) + " ORDER BY id DESC");
        if(qry.exec())
        {
            while(qry.next())
            {
                TrafficPolicy trafficPolicy;
                getTrafficPolicyFromSqlQuery(trafficPolicy, qry);
                returnTrafficPolicies.append(trafficPolicy);
            }
        }
    }
    return returnTrafficPolicies;
}

QList<TrafficPolicy> DatabaseManager::getTrafficPoliciesBetweenDates(QDateTime startDate, QDateTime endDate)
{
    QList<TrafficPolicy> returnTrafficPolicies;
    if(mSqlDatabase.isOpen())
    {
        QString startDateStr = startDate.toString(DATE_FORMAT);
        QString endDateStr = endDate.toString(DATE_FORMAT);
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM TrafficPolicies WHERE BitisTarihi>'"+startDateStr+"' AND BitisTarihi<'"+endDateStr+"' ORDER BY id DESC");
        if(qry.exec())
        {
            while(qry.next())
            {
                TrafficPolicy trafficPolicy;
                getTrafficPolicyFromSqlQuery(trafficPolicy, qry);
                returnTrafficPolicies.append(trafficPolicy);
            }
        }
    }
    return returnTrafficPolicies;
}

bool DatabaseManager::addUpdateTrafficPolicy(TrafficPolicy trafficPolicy)
{
    if (mSqlDatabase.isOpen())
    {
        if (trafficPolicy.mId < 0)//add
        {
            QSqlQuery insertQuery(mSqlDatabase);
            insertQuery.prepare("INSERT INTO TrafficPolicies (BaslangicTarihi, BitisTarihi, SigortaSirketi, Kaskomu,OwnerVehicleId,IsActive) VALUES( ? , ? , ? , ?,?,?)");
            setTrafficPolicyAddUpdateQueryBindValues(trafficPolicy, insertQuery);
            if (insertQuery.exec())
            {
                return true;
            }
            return false;
        }
        else//update
        {
            QSqlQuery qry(mSqlDatabase);
            qry.prepare("UPDATE TrafficPolicies SET BaslangicTarihi = ? , BitisTarihi= ? , SigortaSirketi= ? , Kaskomu= ?,OwnerVehicleId=?,IsActive=? WHERE id = ?");
            setTrafficPolicyAddUpdateQueryBindValues(trafficPolicy, qry);
            qry.bindValue(6, trafficPolicy.mId);
            if (qry.exec())
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

bool DatabaseManager::deleteTrafficPolicy(int trafficPolicyId)
{
    QSqlQuery qry(mSqlDatabase);
    qry.prepare("DELETE FROM TrafficPolicies WHERE id = ? ");
    qry.bindValue(0, trafficPolicyId);
    if (qry.exec())
    {
        return true;
    }
    return false;
}

void DatabaseManager::getTrafficPolicyFromSqlQuery(TrafficPolicy &trafficPolicy, QSqlQuery &sqlQuery)
{
    trafficPolicy.mId = sqlQuery.value(0).toInt();
    trafficPolicy.mBaslangicTarihi = sqlQuery.value(1).toString();
    trafficPolicy.mBitisTarihi = sqlQuery.value(2).toString();
    trafficPolicy.mSigortaSirketi = sqlQuery.value(3).toString();
    trafficPolicy.mKaskomu = sqlQuery.value(4).toBool();
    trafficPolicy.mOwnerVehicleId = sqlQuery.value(5).toInt();
    trafficPolicy.mIsActive = sqlQuery.value(6).toInt();
}

void DatabaseManager::setTrafficPolicyAddUpdateQueryBindValues(TrafficPolicy trafficPolicy, QSqlQuery &query)
{
    query.bindValue(0, trafficPolicy.mBaslangicTarihi);
    query.bindValue(1, trafficPolicy.mBitisTarihi);
    query.bindValue(2, trafficPolicy.mSigortaSirketi);
    query.bindValue(3, trafficPolicy.mKaskomu);
    query.bindValue(4, trafficPolicy.mOwnerVehicleId);
    query.bindValue(5,trafficPolicy.mIsActive);
}

HealthPolicy DatabaseManager::getHealthPolicyWithId(int healthPolicyId)
{
    HealthPolicy healthPolicy;
    healthPolicy.mId = -1;
    if (mSqlDatabase.isOpen())
    {
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM HealthPolicies WHERE id=" + QString::number(healthPolicyId));
        if (qry.exec() && qry.next())
        {
            getHealthPolicyFromSqlQuery(healthPolicy, qry);
        }
    }
    return healthPolicy;
}

QList<HealthPolicy> DatabaseManager::getHealthPoliciesWithOwnerId(int ownerId)
{
    QList<HealthPolicy> returnHealthPolicy;
    if(mSqlDatabase.isOpen())
    {
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM HealthPolicies WHERE OwnerPersonId=" + QString::number(ownerId) + " ORDER BY id DESC");
        if(qry.exec())
        {
            while(qry.next())
            {
                HealthPolicy healthPolicy;
                getHealthPolicyFromSqlQuery(healthPolicy, qry);
                returnHealthPolicy.append(healthPolicy);
            }
        }
    }
    return returnHealthPolicy;
}

QList<HealthPolicy> DatabaseManager::getHealthPoliciesBetweenDates(QDateTime startDate, QDateTime endDate)
{
    QList<HealthPolicy> returnHealthPolicies;
    if(mSqlDatabase.isOpen())
    {
        QString startDateStr = startDate.toString(DATE_FORMAT);
        QString endDateStr = endDate.toString(DATE_FORMAT);
        QSqlQuery qry(mSqlDatabase);
        qry.prepare("SELECT * FROM HealthPolicies WHERE BitisTarihi>'"+startDateStr+"' AND BitisTarihi<'"+endDateStr+"' ORDER BY id DESC");
        if(qry.exec())
        {
            while(qry.next())
            {
                HealthPolicy healthPolicy;
                getHealthPolicyFromSqlQuery(healthPolicy, qry);
                returnHealthPolicies.append(healthPolicy);
            }
        }
    }
    return returnHealthPolicies;
}

bool DatabaseManager::addUpdateHealthPolicy(HealthPolicy healthPolicy)
{
    if (mSqlDatabase.isOpen())
    {
        if (healthPolicy.mId < 0)//add
        {
            QSqlQuery insertQuery(mSqlDatabase);
            insertQuery.prepare("INSERT INTO HealthPolicies (DigerSigortaliTc, DigerSigortaliTc2, DigerSigortaliTc3, HastalikBeyani,SigortaSirketi,Teminatlar,BaslangicTarihi,BitisTarihi,OwnerPersonId,IsActive) VALUES( ? , ? , ? , ?,?,?,?,?,?,?)");
            setHealthPolicyAddUpdateQueryBindValues(healthPolicy, insertQuery);
            if (insertQuery.exec())
            {
                return true;
            }
            return false;
        }
        else//update
        {
            QSqlQuery qry(mSqlDatabase);
            qry.prepare("UPDATE HealthPolicies SET DigerSigortaliTc = ? , DigerSigortaliTc2= ? , DigerSigortaliTc3= ? , HastalikBeyani= ?,SigortaSirketi=?,Teminatlar=?,BaslangicTarihi=?,BitisTarihi=?,OwnerPersonId=?,IsActive = ? WHERE id = ?");
            setHealthPolicyAddUpdateQueryBindValues(healthPolicy, qry);
            qry.bindValue(10, healthPolicy.mId);
            if (qry.exec())
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

bool DatabaseManager::deleteHealthPolicy(int healthPolicyId)
{
    QSqlQuery qry(mSqlDatabase);
    qry.prepare("DELETE FROM HealthPolicies WHERE id = ? ");
    qry.bindValue(0, healthPolicyId);
    if (qry.exec())
    {
        return true;
    }
    return false;
}

void DatabaseManager::getHealthPolicyFromSqlQuery(HealthPolicy &healthPolicy, QSqlQuery &sqlQuery)
{
    healthPolicy.mId = sqlQuery.value(0).toInt();
    healthPolicy.mDigerSigortaliTc = sqlQuery.value(1).toString();
    healthPolicy.mDigerSigortaliTc2 = sqlQuery.value(2).toString();
    healthPolicy.mDigerSigortaliTc3 = sqlQuery.value(3).toString();
    healthPolicy.mHastalikBeyani = sqlQuery.value(4).toString();
    healthPolicy.mSigortaSirketi = sqlQuery.value(5).toString();
    healthPolicy.mTeminatlar = sqlQuery.value(6).toString();
    healthPolicy.mBaslangicTarihi = sqlQuery.value(7).toString();
    healthPolicy.mBitisTarihi = sqlQuery.value(8).toString();
    healthPolicy.mOwnerPersonId = sqlQuery.value(9).toInt();
    healthPolicy.mIsActive = sqlQuery.value(10).toInt();
}

void DatabaseManager::setHealthPolicyAddUpdateQueryBindValues(HealthPolicy healthPolicy, QSqlQuery &query)
{
    query.bindValue(0, healthPolicy.mDigerSigortaliTc);
    query.bindValue(1, healthPolicy.mDigerSigortaliTc2);
    query.bindValue(2, healthPolicy.mDigerSigortaliTc3);
    query.bindValue(3, healthPolicy.mHastalikBeyani);
    query.bindValue(4, healthPolicy.mSigortaSirketi);
    query.bindValue(5,healthPolicy.mTeminatlar);
    query.bindValue(6,healthPolicy.mBaslangicTarihi);
    query.bindValue(7,healthPolicy.mBitisTarihi);
    query.bindValue(8,healthPolicy.mOwnerPersonId);
    query.bindValue(9,healthPolicy.mIsActive);
}
