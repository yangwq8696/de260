#include "main.h"
#include <QTranslator>
#include <QtDBus/QDBusConnection>
#include <QSplashScreen>

/*
 * eeprom使用说明：
 * 0：亮度值
 * 1：语言编号
 * 2：公英制编号
 * 3：NewConsole--Flag1
 * 4：NewConsole--Flag2
 * 5：NewConsole--Flag3
 * 6：NewConsole--Flag4
 * 998:故障存储区已存储满  标志位
 * 999~1000:故障记录区域索引编号，比如总共１００条故障记录区域，当前记录到低５０条区域了
 * 1001～2400：故障存储使用
*/
#include "hsmlib.h"
#include <QApplication>

FUNC_ErrorCode *Func_ErrorCode;
FORM_MainLmiNew *MainLmiNew=0;
FORM_Menu *Menu=0;
FORM_ControllerPort *ControllerPort=0;
FORM_GPSQuery *GPSQuery=0;
FORM_WeighQuery *WeighQuery=0;
FORM_EngineFaultRecord *EngineFaultRecord=0;
FORM_LanguageSetting *LanguageSetting=0;
Form_ProgramUpgrade *ProgramUpgrade=0;
FORM_LightSetting *LightSetting=0;
FORM_TimeDateSetting *TimeDateSetting=0;
FORM_ControlPort2 *ControlPort2=0;
FORM_ControlPort3 *ControlPort3=0;
FORM_ControlPort4 *ControlPort4=0;
FORM_ControlPort5 *ControlPort5=0;
FORM_ControlPort6 *ControlPort6=0;
FORM_InfoQueryMenu *InfoQueryMenu=0;
FORM_CanBusQuery *CanBusQuery=0;
FORM_HydraulicQuery *HydraulicQuery=0;
FORM_MotoQuery *MotoQuery=0;
FORM_UserSettingMenu *UserSettingMenu=0;
FORM_SysManageMenu *SysManageMenu=0;
FORM_HistoryErrorQuery *HistoryErrorQuery=0;
FORM_CurrentErrorQuery* CurretnErrorQuery=0;
Form_GE* Ge=0;

int main(int argc, char *argv[])
{
    if (getenv("DISPLAY") == NULL)
        setenv("DISPLAY", ":0", 1);
    if (getenv("QT_GSTREAMER_CAMERABIN_VIDEOSRC") == NULL)
        setenv("QT_GSTREAMER_CAMERABIN_VIDEOSRC", "imxv4l2src", 1);

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false); // 防止子窗口关闭导致程序退出

    QPixmap pixmap(":/pic_de260/logo.png");
    QSplashScreen splash(pixmap);
    splash.show();

    hsm_lib_init();
    hsm_lib_debug(NULL, LOG_WARNING, 0);

    Func_ErrorCode = new FUNC_ErrorCode();

#if 0
    QTime now;
    now.start();
    ControllerPort = new FORM_ControllerPort;
    Menu = new FORM_Menu;
    WeighQuery = new FORM_WeighQuery;
    GPSQuery = new FORM_GPSQuery;
    EngineFaultRecord = new FORM_EngineFaultRecord;
    LanguageSetting = new FORM_LanguageSetting;
    ProgramUpgrade = new Form_ProgramUpgrade;
    LightSetting = new FORM_LightSetting;
    TimeDateSetting = new FORM_TimeDateSetting;
    ControlPort2 =new FORM_ControlPort2;
    ControlPort3 =new FORM_ControlPort3;
    ControlPort4 =new FORM_ControlPort4;
    ControlPort5 =new FORM_ControlPort5;
    ControlPort6 =new FORM_ControlPort6;
    InfoQueryMenu =new FORM_InfoQueryMenu;
    CanBusQuery =new FORM_CanBusQuery;
    HydraulicQuery =new FORM_HydraulicQuery;
    MotoQuery =new FORM_MotoQuery;
    UserSettingMenu =new FORM_UserSettingMenu;
    SysManageMenu =new FORM_SysManageMenu;
    HistoryErrorQuery =new FORM_HistoryErrorQuery;
    CurretnErrorQuery=new FORM_CurrentErrorQuery;
    Ge = new Form_GE;
    qDebug()<<now.elapsed();
#endif

    uint8_t lightvalue = 0;
    eeprom_read(EEPROM_LIGHTSETTING_OFFSET, &lightvalue, EEPROM_LIGHTSETTING_SIZE);
    if(lightvalue == 0)
        lightvalue = 90;
    sys_set_brightness(lightvalue);

    MainLmiNew = new FORM_MainLmiNew;
    MainLmiNew->showForm();
    splash.finish(MainLmiNew);

    int ret = a.exec();

    hsm_lib_exit();
    return ret;
}
void changeLanguage(int lan_label)
{
    QString qmFilename;
    static QTranslator* translator;
    if(translator != NULL)
    {
        qApp->removeTranslator(translator);
        delete translator;
        translator = NULL;
    }
    translator = new QTranslator;

    QString runPath = QCoreApplication::applicationDirPath();
    if(lan_label == 1)
    {
        qmFilename = runPath + "/llang_Chinese.qm";
    }
    else if(lan_label == 2)
    {
        qmFilename = runPath + "/llang_English.qm";
    }
    if(translator->load(qmFilename))
    {
        qApp->installTranslator(translator);
    }
}

uint8_t unPackValue(uint16_t value,uint16_t num)
{
    return (value >> num) & 1;
}
