#include "FORM_SysManageMenu.h"
#include "ui_FORM_SysManageMenu.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"

FORM_SysManageMenu::FORM_SysManageMenu(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_SysManageMenu),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_SysManageMenu::~FORM_SysManageMenu()
{
    delete ui;
}
void FORM_SysManageMenu::showForm()
{
    ui->retranslateUi(this);
    showFullScreen();
    selextDisplay();

}
void FORM_SysManageMenu::closeForm()
{
    this->hide();//关闭界面执行析够函数->释放资源
}
/********************************************************************/
void FORM_SysManageMenu::selextDisplay()
{
    ui->menu_1->setStyleSheet("color:white");
    ui->menu_2->setStyleSheet("color:white");
    ui->menu_14->setStyleSheet("color:white");

    if(menuIndex==0) ui->menu_2->setStyleSheet("color:red");
    else if(menuIndex==1) ui->menu_1->setStyleSheet("color:red");
    else if(menuIndex==2) ui->menu_14->setStyleSheet("color:red");
}
void FORM_SysManageMenu::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_SysManageMenu::onRelease_Button2()
{
    menuIndex = (menuIndex + 2) % 3;
    selextDisplay();
}
void FORM_SysManageMenu::onRelease_Button3()
{
    menuIndex = (menuIndex + 1) % 3;
    selextDisplay();
}
void FORM_SysManageMenu::onRelease_Button4()
{
    switch(menuIndex)
    {
    case 0:
        if(CurretnErrorQuery == 0)
            CurretnErrorQuery = new FORM_CurrentErrorQuery;
        CurretnErrorQuery->showForm();
        break;
    case 1:
        if(HistoryErrorQuery == 0)
            HistoryErrorQuery = new FORM_HistoryErrorQuery;
        HistoryErrorQuery->showForm();
        break;
    case 2:
        if(EngineFaultRecord == 0)
            EngineFaultRecord = new FORM_EngineFaultRecord;
        EngineFaultRecord->showForm();
        break;
    }
    closeForm();
}
void FORM_SysManageMenu::onRelease_Button5()
{
    if(Menu == 0)
        Menu = new FORM_Menu;
    Menu->showForm();
    closeForm();
}

void FORM_SysManageMenu:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_SysManageMenu::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
