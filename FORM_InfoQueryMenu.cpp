#include "FORM_InfoQueryMenu.h"
#include "ui_FORM_InfoQueryMenu.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"


FORM_InfoQueryMenu::FORM_InfoQueryMenu(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_InfoQueryMenu),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_InfoQueryMenu::~FORM_InfoQueryMenu()
{
    delete ui;
}
void FORM_InfoQueryMenu::showForm()
{
    ui->retranslateUi(this);
    showFullScreen();
    selextDisplay();
}
void FORM_InfoQueryMenu::closeForm()
{
    this->hide();//关闭界面执行析够函数->释放资源
}
/********************************************************************/
void FORM_InfoQueryMenu::selextDisplay()
{
    ui->menu_1->setStyleSheet("color:white");
    ui->menu_2->setStyleSheet("color:white");
    ui->menu_3->setStyleSheet("color:white");
    ui->menu_4->setStyleSheet("color:white");
    ui->menu_10->setStyleSheet("color:white");
    ui->menu_11->setStyleSheet("color:white");
    ui->menu_12->setStyleSheet("color:white");
//    ui->menu_13->setStyleSheet("color:white");

    if     (menuIndex==0) ui->menu_1->setStyleSheet("color:red");
    else if(menuIndex==1) ui->menu_2->setStyleSheet("color:red");
    else if(menuIndex==2) ui->menu_3->setStyleSheet("color:red");
    else if(menuIndex==3) ui->menu_4->setStyleSheet("color:red");
    else if(menuIndex==4) ui->menu_10->setStyleSheet("color:red");
    else if(menuIndex==5) ui->menu_11->setStyleSheet("color:red");
    else if(menuIndex==6) ui->menu_12->setStyleSheet("color:red");
//    else if(menuIndex==7) ui->menu_13->setStyleSheet("color:red");
}
void FORM_InfoQueryMenu::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_InfoQueryMenu::onRelease_Button2()
{
    menuIndex = (menuIndex + 6) % 7;
    selextDisplay();
}
void FORM_InfoQueryMenu::onRelease_Button3()
{
    menuIndex = (menuIndex + 1) % 7;
    selextDisplay();
}
void FORM_InfoQueryMenu::onRelease_Button4()
{
    switch(menuIndex)
    {
    case 0:
        if(MotoQuery == 0)
            MotoQuery = new FORM_MotoQuery;
        MotoQuery->showForm();
        break;
    case 1:
        if(HydraulicQuery == 0)
            HydraulicQuery = new FORM_HydraulicQuery;
        HydraulicQuery->showForm();
        break;
    case 2:
        if(ControllerPort == 0)
            ControllerPort = new FORM_ControllerPort;
        ControllerPort->showForm();
        break;
    case 3:
        if(CanBusQuery == 0)
            CanBusQuery = new FORM_CanBusQuery;
        CanBusQuery->showForm();
        break;
    case 4:
        if(GPSQuery == 0)
            GPSQuery = new FORM_GPSQuery;
        GPSQuery->showForm();
        break;
    case 5:
        if(WeighQuery == 0)
            WeighQuery = new FORM_WeighQuery;
        WeighQuery->showForm();
        break;
    case 6:
        if(Ge == 0)
            Ge = new Form_GE;
        Ge->showForm();
        break;
    }
    closeForm();
}
void FORM_InfoQueryMenu::onRelease_Button5()
{
    if(Menu == 0)
        Menu = new FORM_Menu;
    Menu->showForm();
    closeForm();
}

void FORM_InfoQueryMenu:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_InfoQueryMenu::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
