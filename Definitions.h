#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include<QString>
namespace RidvanView
{
    class Person
    {
    public:
        int mId = -1;
        QString mRegisterNo;
        QString mName;
        QString mSurname;
        QString mPhoneNumber;
    };
    class Vehicle
    {
    public:
        int mId=-1;
        QString mMarka;
        QString mTipi;
        QString mAracKodu;
        QString mKullanimTarzi;
        QString mKullanimSinifi;
        int mOwnerPersonId = -1;
        QString mPlaka;
        QString mRuhsatSeriNo;
        bool mEngelliAracimi;
    };
    class TrafficPolicy
    {
    public:
        int mId=-1;
        QString mBaslangicTarihi;
        QString mBitisTarihi;
        QString mSigortaSirketi;
        bool mKaskomu;
        int mOwnerVehicleId = -1;
        bool mIsActive = true;
    };
    class HealthPolicy
    {
    public:
        int mId=-1;
        QString mDigerSigortaliTc;
        QString mDigerSigortaliTc2;
        QString mDigerSigortaliTc3;
        QString mHastalikBeyani;
        QString mSigortaSirketi;
        QString mTeminatlar;
        QString mBaslangicTarihi;
        QString mBitisTarihi;
        int mOwnerPersonId = -1;
        bool mIsActive = true;
    };
}
#endif // DEFINITIONS_H
