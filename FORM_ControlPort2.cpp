#include "FORM_ControlPort2.h"
#include "ui_FORM_ControlPort2.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"

/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] =
{
    { C1_R04FA0009B1,   0x000000ff,     1,       0   },
    { C1_R04FA0009B2,   0x000000ff,     1,       0   },
};

FORM_ControlPort2::FORM_ControlPort2(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_ControlPort2),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_ControlPort2::~FORM_ControlPort2()
{
    delete ui;
}
/********************************************************************/
void FORM_ControlPort2::showForm()
{

    canActive(CAN1_DEV, varList, N(varList));
    this->show();
    ui->retranslateUi(this);

}
void FORM_ControlPort2::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_ControlPort2::loadFunctionKey(void)
{
    const QPixmap picKey1(":/button/button0.png");
    const QPixmap picKey2(":/button/button1.png");
    const QPixmap picKey3(":/button/button2.png");
    const QPixmap picKey4(":/button/button3.png");
    const QPixmap picKey5(":/button/button4.png");
    delete buttonBar;
    buttonBar = new FUN_ButtonBar(picKey1,picKey2,picKey3,picKey4,picKey5,this);
}
/********************************************************************/
void FORM_ControlPort2::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_ControlPort2::onRelease_Button2()
{
    if(ControllerPort == 0)
        ControllerPort = new FORM_ControllerPort;
    ControllerPort->showForm();
    closeForm();
}
void FORM_ControlPort2::onRelease_Button3()
{
    if(ControlPort3 == 0)
        ControlPort3 = new FORM_ControlPort3;
    ControlPort3->showForm();
    closeForm();
}
void FORM_ControlPort2::onRelease_Button4()
{

}
void FORM_ControlPort2::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

/********************************************************************/
void FORM_ControlPort2::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    const QPixmap sGreen(":/pic_de260/ico_25.png");
    const QPixmap sGray(":/pic_de260/ico_27.png");

    if (code != MSG_CAN_OD)
        return;
    for (unsigned int i=0; i<N(varList); i++)
    {
        if (varList[i].var == m->var)
        {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

            switch (m->var)
            {
                case C1_R04FA0009B1://
                   val = uint16_t(val);
                   //DI1-驻车制动
                   if(unPackValue(val,0)==1) ui->light_1->setPixmap(sGreen);
                   else ui->light_1->setPixmap(sGray);
                   //DI2-
                   if(unPackValue(val,1)==1) ui->light_2->setPixmap(sGreen);
                   else ui->light_2->setPixmap(sGray);
                   //DI3-
                   if(unPackValue(val,2)==1) ui->light_3->setPixmap(sGreen);
                   else ui->light_3->setPixmap(sGray);
                   //DI4-
                   if(unPackValue(val,3)==1) ui->light_4->setPixmap(sGreen);
                   else ui->light_4->setPixmap(sGray);
                   //DI5-左转向信号
                   if(unPackValue(val,4)==1) ui->light_5->setPixmap(sGreen);
                   else ui->light_5->setPixmap(sGray);
                   //DI6-右转向信号
                   if(unPackValue(val,5)==1) ui->light_6->setPixmap(sGreen);
                   else ui->light_6->setPixmap(sGray);
                   //DI7-梯灯开关
                   if(unPackValue(val,6)==1) ui->light_7->setPixmap(sGreen);
                   else ui->light_7->setPixmap(sGray);
                   //DI8-雨刮点动
                   if(unPackValue(val,7)==1) ui->light_8->setPixmap(sGreen);
                   else ui->light_8->setPixmap(sGray);
                break;
                case C1_R04FA0009B2://
                    val = uint16_t(val);
//                    //DI9-GE牵引信号
//                    if(unPackValue(val,0)==1) ui->light_8->setPixmap(sGreen);
//                    else ui->light_8->setPixmap(sGray);
//                    //DI10-GE电制动信号
//                    if(unPackValue(val,1)==1) ui->light_9->setPixmap(sGreen);
//                    else ui->light_9->setPixmap(sGray);
//                    //DI11-GE车速信号
//                    if(unPackValue(val,2)==1) ui->light_10->setPixmap(sGreen);
//                    else ui->light_10->setPixmap(sGray);
                    //DI12-空滤堵塞报警1
                    if(unPackValue(val,0)==1) ui->light_12->setPixmap(sGreen);
                    else ui->light_12->setPixmap(sGray);
                    //DI13-空滤堵塞报警2
                    if(unPackValue(val,1)==1) ui->light_13->setPixmap(sGreen);
                    else ui->light_13->setPixmap(sGray);
                    //DI14-空滤堵塞报警3
                    if(unPackValue(val,2)==1) ui->light_14->setPixmap(sGreen);
                    else ui->light_14->setPixmap(sGray);
                    //DI15-空滤堵塞报警4
                    if(unPackValue(val,3)==1) ui->light_15->setPixmap(sGreen);
                    else ui->light_15->setPixmap(sGray);
                break;
            }
            break;
        }
    }
}

void FORM_ControlPort2:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_ControlPort2::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
