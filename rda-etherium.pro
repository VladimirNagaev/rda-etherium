QT       += core gui
QT += serialbus serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14


    minimumQbsVersion: "1.7.1"

    CppApplication {
        Depends { name: "Qt.core" }
        Depends { name: "Qt.quick" }
        Depends { name: "Qt.serialbus" }
        Depends { name: "Qt.serialport" }
}
DEFINES += QT_DEPRECATED_WARNINGS
        cpp.cxxLanguageVersion: "c++14"
        consoleApplication: false

SOURCES += \
    deviceconnectinfo.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    deviceconnectinfo.h \
    mainwindow.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
