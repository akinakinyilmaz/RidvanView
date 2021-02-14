QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DatabaseManager.cpp \
    addupdatehealthpolicydialog.cpp \
    addupdatepersondialog.cpp \
    addupdatetrafficpolicydialog.cpp \
    addupdatevehicledialog.cpp \
    main.cpp \
    mainwindow.cpp \
    persondetails.cpp

HEADERS += \
    DatabaseManager.h \
    Definitions.h \
    addupdatehealthpolicydialog.h \
    addupdatepersondialog.h \
    addupdatetrafficpolicydialog.h \
    addupdatevehicledialog.h \
    mainwindow.h \
    persondetails.h

FORMS += \
    addupdatehealthpolicydialog.ui \
    addupdatepersondialog.ui \
    addupdatetrafficpolicydialog.ui \
    addupdatevehicledialog.ui \
    mainwindow.ui \
    persondetails.ui

TRANSLATIONS += \
    RidvanView_tr_TR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
