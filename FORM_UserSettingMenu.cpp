#include "FORM_UserSettingMenu.h"
#include "ui_FORM_UserSettingMenu.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"

FORM_UserSettingMenu::FORM_UserSettingMenu(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_UserSettingMenu),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_UserSettingMenu::~FORM_UserSettingMenu()
{
    delete ui;
}
void FORM_UserSettingMenu::showForm()
{
    ui->retranslateUi(this);
    showFullScreen();
    selextDisplay();
}
void FORM_UserSettingMenu::closeForm()
{
    this->hide();//关闭界面执行析够函数->释放资源
}
/********************************************************************/
void FORM_UserSettingMenu::selextDisplay()
{
    ui->menu_1->setStyleSheet("color:white");
    ui->menu_2->setStyleSheet("color:white");
    ui->menu_5->setStyleSheet("color:white");
    ui->menu_7->setStyleSheet("color:white");


    if     (menuIndex==0) ui->menu_1->setStyleSheet("color:red");
    else if(menuIndex==1) ui->menu_2->setStyleSheet("color:red");
    else if(menuIndex==2) ui->menu_5->setStyleSheet("color:red");
    else if(menuIndex==3) ui->menu_7->setStyleSheet("color:red");
}
void FORM_UserSettingMenu::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_UserSettingMenu::onRelease_Button2()
{
    menuIndex = (menuIndex + 3) % 4;
    selextDisplay();
}
void FORM_UserSettingMenu::onRelease_Button3()
{
    menuIndex = (menuIndex + 1) % 4;
    selextDisplay();
}
void FORM_UserSettingMenu::onRelease_Button4()
{
    switch(menuIndex)
    {
    case 0:
        if(TimeDateSetting == 0)
            TimeDateSetting = new FORM_TimeDateSetting;
        TimeDateSetting->showForm();
        break;
    case 1:
        if(LightSetting == 0)
            LightSetting = new FORM_LightSetting;
        LightSetting->showForm();
        break;
    case 2:
        if(LanguageSetting == 0)
            LanguageSetting = new FORM_LanguageSetting;
        LanguageSetting->showForm();
        break;
    case 3:
        if(ProgramUpgrade == 0)
            ProgramUpgrade = new Form_ProgramUpgrade;
        ProgramUpgrade->showForm();
        break;
    }
    closeForm();
}

void FORM_UserSettingMenu::onRelease_Button5()
{
    if(Menu == 0)
        Menu = new FORM_Menu;
    Menu->showForm();
    closeForm();
}

void FORM_UserSettingMenu:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_UserSettingMenu::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
