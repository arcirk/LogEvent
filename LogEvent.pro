QT += core gui
QT += widgets
QT += sql

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogabout.cpp \
    dialogoptions.cpp \
    dialogselectcolumn.cpp \
    dialogselectedrow.cpp \
    dialogselectinfobase.cpp \
    dialogselectitemsfilter.cpp \
    dialogsqlfilter.cpp \
    dialogsavefilter.cpp \
    filteritem.cpp \
    filtermanager.cpp \
    infobases.cpp \
    main.cpp \
    mainwindow.cpp \
    querybuilder.cpp \
    selectiondialog.cpp \
    settings.cpp

HEADERS += \
    dialogabout.h \
    dialogoptions.h \
    dialogselectcolumn.h \
    dialogselectedrow.h \
    dialogselectinfobase.h \
    dialogselectitemsfilter.h \
    dialogsqlfilter.h \
    dialogsavefilter.h \
    filteritem.h \
    filtermanager.h \
    infobases.h \
    mainwindow.h \
    querybuilder.h \
    selectiondialog.h \
    settings.h

FORMS += \
    dialogabout.ui \
    dialogoptions.ui \
    dialogselectcolumn.ui \
    dialogselectedrow.ui \
    dialogselectinfobase.ui \
    dialogselectitemsfilter.ui \
    dialogsqlfilter.ui \
    dialogsavefilter.ui \
    mainwindow.ui \
    selectiondialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resurce.qrc

windows:DEFINES += _WINDOWS

win32 {
    RC_FILE = logevent.rc
    }

DISTFILES += \
    logevent.rc

OTHER_FILES += \
    logevent.rc
