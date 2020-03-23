#-------------------------------------------------
#
# Project created by QtCreator 2018-12-04T11:48:35
#
#-------------------------------------------------

QT += core gui widgets sql
QT += dbus
TARGET = DE260
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

MOC_DIR = ./Obj
RCC_DIR = ./Obj
OBJECTS_DIR = ./Obj

LIBS += -lhsm

TRANSLATIONS += language/LANG_en.ts
TRANSLATIONS += language/LANG_zh.ts
TRANSLATIONS += language/LANG_ru.ts

SOURCES += \
    main.cpp \
    page.cpp \
    libmsg.cpp \ 
    meterNeedle.cpp \
    FORM_Menu.cpp \
    FUN_MeterFinger.cpp \
    FUN_ButtonBar.cpp \
    FORM_MainLmiNew.cpp \
    FORM_ControllerPort.cpp \
    FORM_GPSQuery.cpp \
    FORM_TimeDateSetting.cpp \
    FORM_LightSetting.cpp \
    FORM_LanguageSetting.cpp \
    FORM_ControlPort2.cpp \
    FORM_ControlPort3.cpp \
    FORM_ControlPort4.cpp \
    FORM_ControlPort5.cpp \
    FORM_ControlPort6.cpp \
    FORM_InfoQueryMenu.cpp \
    FORM_MotoQuery.cpp \
    FORM_HydraulicQuery.cpp \
    FORM_CanBusQuery.cpp \
    FORM_UserSettingMenu.cpp \
    FORM_SysManageMenu.cpp \
    FUN_GetErrorInfo.cpp \
    wika_tableView.cpp \
    FORM_WeighQuery.cpp \
    wika_Label.cpp \
    wika_GeneralWidget.cpp \
    Form_GE.cpp \
    FUNC_ErrorCode.cpp \
    FORM_CurrentErrorQuery.cpp \
    FORM_EngineFaultRecord.cpp \
    FUNC_FickerLabel.cpp \
    FORM_HistoryErrorQuery.cpp \
    Form_ProgramUpgrade.cpp
HEADERS += \
    page.h \
    libmsg.h \
    defines.h \ 
    meterNeedle.h \
    FORM_Menu.h \
    FUN_MeterFinger.h \
    FUN_ButtonBar.h \
    main.h \
    FORM_MainLmiNew.h \
    FORM_ControllerPort.h \
    FORM_GPSQuery.h \
    FORM_TimeDateSetting.h \
    FORM_LightSetting.h \
    FORM_LanguageSetting.h \
    FORM_ControlPort2.h \
    FORM_ControlPort3.h \
    FORM_ControlPort4.h \
    FORM_ControlPort5.h \
    FORM_ControlPort6.h \
    FORM_InfoQueryMenu.h \
    FORM_MotoQuery.h \
    FORM_HydraulicQuery.h \
    FORM_CanBusQuery.h \
    FORM_UserSettingMenu.h \
    FORM_SysManageMenu.h \
    FORM_HistoryErrorQuery.h \
    FUN_GetErrorInfo.h \
    wika_tableView.h \
    FORM_WeighQuery.h \
    wika_Label.h \
    wika_GeneralWidget.h \
    Form_GE.h \
    FUNC_ErrorCode.h \
    ErrorTypedef.h \
    FORM_CurrentErrorQuery.h \
    FORM_EngineFaultRecord.h \
    FUNC_FickerLabel.h \
    Form_ProgramUpgrade.h

FORMS += \
    FORM_Menu.ui \
    FORM_MainLmiNew.ui \
    FORM_ControllerPort.ui \
    FORM_GPSQuery.ui \
    FORM_TimeDateSetting.ui \
    FORM_LightSetting.ui \
    FORM_LanguageSetting.ui \
    FORM_ControlPort2.ui \
    FORM_ControlPort3.ui \
    FORM_ControlPort4.ui \
    FORM_ControlPort5.ui \
    FORM_ControlPort6.ui \
    FORM_InfoQueryMenu.ui \
    FORM_MotoQuery.ui \
    FORM_HydraulicQuery.ui \
    FORM_CanBusQuery.ui \
    FORM_UserSettingMenu.ui \
    FORM_SysManageMenu.ui \
    FORM_HistoryErrorQuery.ui \
    FORM_WeighQuery.ui \
    wika_GeneralWidget.ui \
    Form_GE.ui \
    FORM_CurrentErrorQuery.ui \
    FORM_EngineFaultRecord.ui \
    Form_ProgramUpgrade.ui

target.path = /apps
INSTALLS += target

RESOURCES += \
    resources.qrc

#TRANSLATIONS = meter2_zh_CN.ts
