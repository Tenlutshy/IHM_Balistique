QT += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    prevision.cpp \
    settingsmanager.cpp \
    tcpserveur.cpp

HEADERS += \
    databasemanager.h \
    logger.h \
    mainwindow.h \
    prevision.h \
    settingsmanager.h \
    tcpserveur.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    angle.png \
    database.db \
    explosion-de-bombe.png \
    poids.png \
    settings.ini \
    voyage.png

RESOURCES += \
    Ressources.qrc
