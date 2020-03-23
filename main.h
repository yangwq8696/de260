#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <sstream>
#include <QMatrix>
#include <QImage>
#include <QString>
#include <QPainter>
#include <QPushButton>
#include <QFontDatabase>
#include <QFont>
#include <QTranslator>
#include <QVBoxLayout>
#include <QPoint>
#include <QMainWindow>
#include <QPaintEvent>
#include <QDateTimeEdit>
#include "hsmlib.h"
#include "utility.h"
#include "page.h"
#include "defines.h"
#include "FUN_ButtonBar.h"
#include "FUN_MeterFinger.h"
#include "FUN_GetErrorInfo.h"
#include "wika_tableView.h"
#include "FORM_MainLmiNew.h"
#include "FORM_Menu.h"
#include "FORM_ControllerPort.h"
#include "FORM_GPSQuery.h"
#include "FORM_EngineFaultRecord.h"
#include "FORM_LanguageSetting.h"
#include "FORM_LightSetting.h"
#include "FORM_TimeDateSetting.h"
#include "FORM_ControlPort2.h"
#include "FORM_ControlPort3.h"
#include "FORM_ControlPort4.h"
#include "FORM_ControlPort5.h"
#include "FORM_ControlPort6.h"
#include "FORM_InfoQueryMenu.h"
#include "FORM_CanBusQuery.h"
#include "FORM_HydraulicQuery.h"
#include "FORM_MotoQuery.h"
#include "FORM_UserSettingMenu.h"
#include "FORM_SysManageMenu.h"
#include "FORM_HistoryErrorQuery.h"
#include "FORM_WeighQuery.h"
#include "Form_GE.h"
#include "FUNC_ErrorCode.h"
#include "FORM_CurrentErrorQuery.h"
#include "Form_ProgramUpgrade.h"


typedef  struct{
    int devid;
    int nodeid;
    int baudrate;
    char *edsfile;
}CANOPENINFO;

extern FUNC_ErrorCode *Func_ErrorCode;
extern FUN_ButtonBar *buttonBar;
extern FORM_MainLmiNew *MainLmiNew;
extern FORM_Menu *Menu;
extern FORM_ControllerPort *ControllerPort;
extern FORM_WeighQuery *WeighQuery;
extern FORM_GPSQuery *GPSQuery;
extern FORM_EngineFaultRecord *EngineFaultRecord;
extern FORM_LanguageSetting *LanguageSetting;
extern Form_ProgramUpgrade *ProgramUpgrade;

extern FORM_LightSetting *LightSetting;
extern FORM_TimeDateSetting *TimeDateSetting;
extern FORM_ControlPort2 *ControlPort2;
extern FORM_ControlPort3 *ControlPort3;
extern FORM_ControlPort4 *ControlPort4;
extern FORM_ControlPort5 *ControlPort5;
extern FORM_ControlPort6 *ControlPort6;
extern FORM_InfoQueryMenu *InfoQueryMenu;
extern FORM_CanBusQuery *CanBusQuery;
extern FORM_HydraulicQuery *HydraulicQuery;
extern FORM_MotoQuery *MotoQuery;
extern FORM_UserSettingMenu *UserSettingMenu;
extern FORM_SysManageMenu *SysManageMenu;
extern FORM_HistoryErrorQuery *HistoryErrorQuery;
extern FORM_CurrentErrorQuery* CurretnErrorQuery;
extern Form_GE* Ge;

void changeLanguage(int lan_label);
uint8_t unPackValue(uint16_t value,uint16_t num);
#endif // MAIN_H
