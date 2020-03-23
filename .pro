#-------------------------------------------------
#
# Project created by QtCreator 2018-12-04T11:48:35
#
#-------------------------------------------------

QT += core gui widgets
TARGET = meter2
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

LIBS += -lhsm

SOURCES += \
    main.cpp \
    page.cpp \
    meter.cpp \
    menumain.cpp \
    menudiagnose.cpp \
    menuassist.cpp \
    cfgsystem.cpp \
    viewnet.cpp \
    viewport.cpp \
    viewj1939.cpp \
    viewsteering.cpp \
    libmsg.cpp \
    meterNeedle.cpp \
    menuchassislight.cpp\
    chassisassist.cpp \
    chassisstatemonitor.cpp \
    chassisstatemonitor2.cpp \
    test.cpp \
    menumaintain/menumaintainDaily.cpp \
    menumaintain/menumaintainsubpage.cpp \
    menumaintain/menumaintainEngine_1.cpp \
    menumaintain/menumaintainIntervalmile.cpp \
    menumaintain/menumaintainTenance.cpp \
    menumaintain/menumaintainOilchangecycle.cpp \
    factorySet.cpp \
    menumaintain.cpp \
    test.cpp

HEADERS += \
    page.h \
    meter.h \
    cfgsystem.h \
    menumain.h \
    menudiagnose.h \
    menuassist.h \
    viewnet.h \
    viewport.h \
    viewj1939.h \
    viewsteering.h \
    libmsg.h \
    defines.h \
    meterNeedle.h \
    menuchassislight.h\
    chassisassist.h \
    chassisstatemonitor.h \
    chassisstatemonitor2.h \
    test.h \
    menumaintain/menumaintainDaily.h \
    menumaintain/menumaintainheaders.h \
    menumaintain/menumaintainsubpage.h \
    menumaintain/menumaintainEngine_1.h \
    menumaintain/menumaintainIntervalmile.h \
    menumaintain/menumaintainTenance.h \
    menumaintain/menumaintainOilchangecycle.h \
    factorySet.h \
    view1939erroocode.h \
    menumaintain.h \
    test.h

FORMS += \
    meter.ui \
    menumain.ui \
    menudiagnose.ui \
    menuassist.ui \
    cfgsystem.ui \
    viewnet.ui \
    viewport.ui \
    viewj1939.ui \
    viewsteering.ui \
    menuchassislight.ui\
    chassisassist.ui \
    chassisstatemonitor.ui \
    chassisstatemonitor2.ui \
    test.ui \
    menumaintain/menumaintainOilchangecycle.ui \
    menumaintain/menumaintainIntervalmile.ui \
    menumaintain/menumaintainEngine_1.ui \
    menumaintain/menumaintainDaily.ui \
    menumaintain/menumaintainTenance.ui \
    factorySet.ui \
    menumaintain.ui \
    test.ui

target.path = /opt/$${TARGET}/bin
INSTALLS += target

RESOURCES += \
    resources.qrc
