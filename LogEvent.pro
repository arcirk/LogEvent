QT       += core gui
QT += sql
#QT += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogoptions.cpp \
    dialogselectcolumn.cpp \
    dialogselectinfobase.cpp \
    infobases.cpp \
    main.cpp \
    mainwindow.cpp \
    querybuilder.cpp \
    settings.cpp

HEADERS += \
    dialogoptions.h \
    dialogselectcolumn.h \
    dialogselectinfobase.h \
    infobases.h \
    mainwindow.h \
    querybuilder.h \
    settings.h

FORMS += \
    dialogoptions.ui \
    dialogselectcolumn.ui \
    dialogselectinfobase.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resurce.qrc

windows:DEFINES += _WINDOWS
