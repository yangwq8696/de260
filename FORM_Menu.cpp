#include "FORM_Menu.h"
#include "ui_FORM_Menu.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"


FORM_Menu::FORM_Menu(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_Menu),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}
FORM_Menu::~FORM_Menu()
{
//    qDebug()<<"~FORM_Menu()";
    delete ui;
}
void FORM_Menu::showForm()
{
    ui->retranslateUi(this);
    showFullScreen();
    selextDisplay();    
}
void FORM_Menu::closeForm()
{
    this->hide();
//    close();  //释放窗口
    setAttribute(Qt::WA_DeleteOnClose,1);//关闭界面执行析够函数->释放资源
}
/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_Menu::loadFunctionKey(void)
{
    const QPixmap picKey1(":/button/button0.png");
    const QPixmap picKey2(":/button/button1.png");
    const QPixmap picKey3(":/button/button2.png");
    const QPixmap picKey4(":/button/button3.png");
    const QPixmap picKey5(":/button/button4.png");
    delete buttonBar;
//    qDebug() << "test01";
}
/********************************************************************/
void FORM_Menu::selextDisplay()
{
//    qDebug()<<"test  begin";
    ui->menu_1->setStyleSheet("color:white");
    ui->menu_2->setStyleSheet("color:white");
    ui->menu_3->setStyleSheet("color:white");


    if     (menuIndex==0) ui->menu_1->setStyleSheet("color:red");
    else if(menuIndex==1) ui->menu_2->setStyleSheet("color:red");
    else if(menuIndex==2) ui->menu_3->setStyleSheet("color:red");
//    qDebug()<<"test  end";

}
void FORM_Menu::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_Menu::onRelease_Button2()
{
    menuIndex = (menuIndex + 2) % 3;
    selextDisplay();
}
void FORM_Menu::onRelease_Button3()
{
    menuIndex = (menuIndex + 1) % 3;
    selextDisplay();
}
void FORM_Menu::onRelease_Button4()
{
    switch(menuIndex)
    {
    case 0:
        if(InfoQueryMenu == 0)
            InfoQueryMenu = new FORM_InfoQueryMenu;
        InfoQueryMenu->showForm();
        break;
    case 1:
        if(UserSettingMenu == 0)
            UserSettingMenu = new FORM_UserSettingMenu;
        UserSettingMenu->showForm();
        break;
    case 2:
        if(SysManageMenu == 0)
            SysManageMenu = new FORM_SysManageMenu;
        SysManageMenu->showForm();
        break;
    default:
        break;
    }
    closeForm();
}
void FORM_Menu::onRelease_Button5()
{
    MainLmiNew->showForm();
    closeForm();
}


void FORM_Menu:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_Menu::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
