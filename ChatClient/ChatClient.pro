QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/

SOURCES += \
    cpp/addclientdlg.cpp \
    cpp/clienttablemodel.cpp \
    cpp/settingsdlg.cpp \
    main.cpp \
    cpp/mainwindow.cpp \
    cpp/tcpworker.cpp

HEADERS += \
    include/addclientdlg.h \
    include/clienttablemodel.h \
    include/settingsdlg.h \
    include/mainwindow.h \
    include/tcpworker.h

FORMS += \
    ui/addclientdlg.ui \
    ui/settingsdlg.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
