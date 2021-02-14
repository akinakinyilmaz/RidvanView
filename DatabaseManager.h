#pragma once
#include<QString>
#include<QtSql\qsqldatabase.h>
#include<QList>
#include<QtSql/QSqlQuery>
#include<QVariant>
#include<Definitions.h>
#include<QDateTime>
#define DATE_FORMAT "yyyy-MM-dd-hh-mm-ss"
#define VISUAL_DATE_FORMAT "dd/MM/yyyy hh:mm:ss"
namespace RidvanView
{
	class DatabaseManager
	{
	public:
		DatabaseManager(QString dbFileName = "");
		~DatabaseManager();
		bool connectToDatabase(QString dbFileName);
		void closeDatabaseConnection();
		bool isConnected();
		QSqlDatabase getSqlDatabase();
		void setDefaultDBName(QString defaultDbName);
		QString getDefaultDbName();
		QString getDBName();
		bool backUpCurrentDatabase(QString databaseBackUpPath);
		bool resetCurrentDatabase();
		bool restoreBackupDatabase(QString dbFile);
        Person getPersonWithId(int personId);
        Person getPersonWithRegisterNo(QString registerNo);
        QList<Person> getPersonsWithSearchText(QString searchText);
        bool addUpdatePerson(Person person);
        bool deletePerson(int personId);
        void getPersonFromSqlQuery(Person& personToFill, QSqlQuery& sqlQuery);
        void setPersonAddUpdateQueryBindValues(Person person, QSqlQuery& query);

        Vehicle getVehicleWithId(int vehicleId);
        QList<Vehicle> getVehiclesWithOwnerId(int ownerId);
        bool addUpdateVehicle(Vehicle vehicle);
        bool deleteVehicle(int vehicleId);
        void getVehicleFromSqlQuery(Vehicle& vehicleToFill,QSqlQuery& sqlQuery);
        void setVehicleAddUpdateQueryBindValues(Vehicle vehicle,QSqlQuery& query);

        TrafficPolicy getTrafficPolicyWithId(int trafficQueryId);
        QList<TrafficPolicy> getTrafficPoliciesWithOwnerVehicleId(int ownerVehicleId);
        QList<TrafficPolicy> getTrafficPoliciesBetweenDates(QDateTime startDate,QDateTime endDate);
        bool addUpdateTrafficPolicy(TrafficPolicy trafficPolicy);
        bool deleteTrafficPolicy(int trafficPolicyId);
        void getTrafficPolicyFromSqlQuery(TrafficPolicy& trafficPolicy, QSqlQuery& sqlQuery);
        void setTrafficPolicyAddUpdateQueryBindValues(TrafficPolicy trafficPolicy,QSqlQuery& query);

        HealthPolicy getHealthPolicyWithId(int healthPolicyId);
        QList<HealthPolicy> getHealthPoliciesWithOwnerId(int ownerId);
        QList<HealthPolicy> getHealthPoliciesBetweenDates(QDateTime startDate,QDateTime endDate);
        bool addUpdateHealthPolicy(HealthPolicy healthPolicy);
        bool deleteHealthPolicy(int healthPolicyId);
        void getHealthPolicyFromSqlQuery(HealthPolicy& healthPolicy,QSqlQuery& sqlQuery);
        void setHealthPolicyAddUpdateQueryBindValues(HealthPolicy healthPolicy,QSqlQuery&query);

	protected:
	private:
		QSqlDatabase mSqlDatabase;
		bool mDatabaseOpenedStatus;
		QString mDBFileName;
		QString mDefaultDBFileName;
	};
}

