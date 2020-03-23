#include "FORM_ControllerPort.h"
#include "ui_FORM_ControllerPort.h"
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
    { C1_R04FA0010B1,   0x000000ff,     1,       0   },
    { C1_R04FA0010B2,   0x000000ff,     1,       0   },
    { C1_R04FA0010B3,   0x000000ff,     1,       0   }
};


FORM_ControllerPort::FORM_ControllerPort(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_ControllerPort),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_ControllerPort::~FORM_ControllerPort()
{
    delete ui;
}
/********************************************************************/
void FORM_ControllerPort::showForm()
{
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList));
    this->show();
}
void FORM_ControllerPort::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_ControllerPort::loadFunctionKey(void)
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
void FORM_ControllerPort::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_ControllerPort::onRelease_Button2()
{
    if(ControlPort6 == 0)
        ControlPort6 = new FORM_ControlPort6;
    ControlPort6->showForm();
    closeForm();
}
void FORM_ControllerPort::onRelease_Button3()
{
    if(ControlPort2 == 0)
        ControlPort2 = new FORM_ControlPort2;
    ControlPort2->showForm();
    closeForm();
}
void FORM_ControllerPort::onRelease_Button4()
{

}
void FORM_ControllerPort::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}
/********************************************************************/
void FORM_ControllerPort::onCanRecv(int code, msg_can_t *m)
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
                case C1_R04FA0010B1://
                   val = uint16_t(val);
                   //DO15-前大灯组合开关--组合仪表供电
                   if(unPackValue(val,0)==1) ui->light_15->setPixmap(sGreen);
                   else ui->light_15->setPixmap(sGray);
                   //DO1-互锁继电器
                   if(unPackValue(val,1)==1) ui->light_1->setPixmap(sGreen);
                   else ui->light_1->setPixmap(sGray);
                   //DO2-倒车继电器
                   if(unPackValue(val,2)==1) ui->light_2->setPixmap(sGreen);
                   else ui->light_2->setPixmap(sGray);
                   //DO3-后视镜加热继电器
                   if(unPackValue(val,3)==1) ui->light_3->setPixmap(sGreen);
                   else ui->light_3->setPixmap(sGray);
                   //DO4-雾灯继电器
                   if(unPackValue(val,4)==1) ui->light_4->setPixmap(sGreen);
                   else ui->light_4->setPixmap(sGray);
                   //DO5-梯灯继电器
                   if(unPackValue(val,5)==1) ui->light_5->setPixmap(sGreen);
                   else ui->light_5->setPixmap(sGray);
                   //DO6-增压风机继电器
                   if(unPackValue(val,6)==1) ui->light_6->setPixmap(sGreen);
                   else ui->light_6->setPixmap(sGray);
                   //DO7-雨刮点动继电器
                   if(unPackValue(val,7)==1) ui->light_7->setPixmap(sGreen);
                   else ui->light_7->setPixmap(sGray);
                break;
                case C1_R04FA0010B2://
                    val = uint16_t(val);
                    //DO8-倒车灯继电器
                    if(unPackValue(val,0)==1) ui->light_8->setPixmap(sGreen);
                    else ui->light_8->setPixmap(sGray);
                    //DO9-驻车自动电磁阀
                    if(unPackValue(val,1)==1) ui->light_9->setPixmap(sGreen);
                    else ui->light_9->setPixmap(sGray);
                    //D10-左转向
                    if(unPackValue(val,2)==1) ui->light_10->setPixmap(sGreen);
                    else ui->light_10->setPixmap(sGray);
                    //DO11-右转向
                    if(unPackValue(val,3)==1) ui->light_11->setPixmap(sGreen);
                    else ui->light_11->setPixmap(sGray);
                    //DO12-绿色灯
                    if(unPackValue(val,4)==1) ui->light_12->setPixmap(sGreen);
                    else ui->light_12->setPixmap(sGray);
                    //DO13-黄色灯
                    if(unPackValue(val,5)==1) ui->light_13->setPixmap(sGreen);
                    else ui->light_13->setPixmap(sGray);
                    //DO14-红色灯
                    if(unPackValue(val,6)==1) ui->light_14->setPixmap(sGreen);
                    else ui->light_14->setPixmap(sGray);
//                    //DO15-前大灯组合开关--组合仪表供电
//                    if(1) ui->light_15->setPixmap(sGreen);
//                    else ui->light_15->setPixmap(sGray);
//                    //DO16-组合仪表电源1
//                    if(unPackValue(val,7)==1) ui->light_16->setPixmap(sGreen);
//                    else ui->light_16->setPixmap(sGray);
//                    //DO17-组合仪表电源2
//                    if(unPackValue(val,7)==1) ui->light_17->setPixmap(sGreen);
//                    else ui->light_17->setPixmap(sGray);
                break;
            case C1_R04FA0010B3:
                val = uint16_t(val);
                //DO18-驻车制动信号
                if(unPackValue(val,3)==1) ui->light_18->setPixmap(sGreen);
                else ui->light_18->setPixmap(sGray);
                break;
            }
            break;
        }
    }
}


void FORM_ControllerPort:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_ControllerPort::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
