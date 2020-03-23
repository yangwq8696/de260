#include "FORM_LightSetting.h"
#include "ui_FORM_LightSetting.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"

FORM_LightSetting::FORM_LightSetting(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_LightSetting),
    m_uiInited(false)
{
    ui->setupUi(this);
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);

    lightvalue = 0;
}

FORM_LightSetting::~FORM_LightSetting()
{
    delete ui;
}
/********************************************************************/
void FORM_LightSetting::showForm()
{
    ui->retranslateUi(this);
    showFullScreen();
    this->show();

    eeprom_read(EEPROM_LIGHTSETTING_OFFSET, &lightvalue, EEPROM_LIGHTSETTING_SIZE);
//    qDebug()<<"FORM_LightSetting::showForm():"<<lightvalue;
    ui->horizontalSlider->setValue(lightvalue);
    sys_set_brightness(lightvalue);

    t_F2_Previous = QTime::currentTime();
}
void FORM_LightSetting::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);
}
/********************************************************************/
void FORM_LightSetting::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_LightSetting::onRelease_Button2()
{
    lightvalue++;
    if(lightvalue>100) lightvalue = 100;
    ui->horizontalSlider->setValue(lightvalue);

    if(DelayStaticstics()>150){
        sys_set_brightness(lightvalue);
        t_F2_Previous = QTime::currentTime();
    }else {
        t_F2_Previous.addMSecs(DelayStaticstics());
    }
}
void FORM_LightSetting::onRelease_Button3()
{

    lightvalue--;
    if(lightvalue == 255) lightvalue = 0;
    ui->horizontalSlider->setValue(lightvalue);//horizontalSlider

    if(DelayStaticstics()>150){
        sys_set_brightness(lightvalue);
        t_F2_Previous = QTime::currentTime();
    }else {
        t_F2_Previous.addMSecs(DelayStaticstics());
    }
}
void FORM_LightSetting::onRelease_Button4()
{
    sys_set_brightness(lightvalue);
    uint8_t value = lightvalue;
    eeprom_write(EEPROM_LIGHTSETTING_OFFSET, &value, EEPROM_LIGHTSETTING_SIZE);
}
void FORM_LightSetting::onRelease_Button5()
{
    if(UserSettingMenu == 0)
        UserSettingMenu = new FORM_UserSettingMenu;
    UserSettingMenu->showForm();
    closeForm();
}

void FORM_LightSetting:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_LightSetting::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}

int FORM_LightSetting::DelayStaticstics(){
    int h = QTime::currentTime().hour();
    int m = QTime::currentTime().minute();
    int s = QTime::currentTime().second();
    int msec = QTime::currentTime().msec();

    int h0 = t_F2_Previous.hour();
    int m0 = t_F2_Previous.minute();
    int s0 = t_F2_Previous.second();
    int msec0 = t_F2_Previous.msec();

    int milliseconds = (h-h0)*60*60*1000+(m-m0)*60*1000+(s-s0)*1000+msec-msec0;

    return milliseconds;
}


