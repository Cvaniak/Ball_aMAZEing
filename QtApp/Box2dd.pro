QT       += core gui opengl serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bubble.cpp \
    comport.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    mcontactlistener.cpp \
    qcustomplot.cpp

HEADERS += \
    bubble.h \
    comport.h \
    game.h \
    mainwindow.h \
    mcontactlistener.h \
    qcustomplot.h

FORMS += \
    comport.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx|win32: LIBS += -L$$PWD/../../../Pulpit/box2d-master/build/src/ -lbox2d

INCLUDEPATH += $$PWD/../../../Pulpit/box2d-master/include/box2d
DEPENDPATH += $$PWD/../../../Pulpit/box2d-master/include/box2d

INCLUDEPATH += $$PWD/../../../Pulpit/box2d-master/include
DEPENDPATH += $$PWD/../../../Pulpit/box2d-master/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../Pulpit/box2d-master/build/src/box2d.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/../../../Pulpit/box2d-master/build/src/libbox2d.a
