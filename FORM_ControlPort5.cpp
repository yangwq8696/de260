#include "FORM_ControlPort5.h"
#include "ui_FORM_ControlPort5.h"
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
    { C1_R04FB0006B5,   0x000000ff,     1,       0   },
    { C1_R04FB0006B6,   0x000000ff,     1,       0   },
    { C1_R04FB0006B7,   0x000000ff,     1,       0   },
};


FORM_ControlPort5::FORM_ControlPort5(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_ControlPort5),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_ControlPort5::~FORM_ControlPort5()
{
    delete ui;
}
/********************************************************************/
void FORM_ControlPort5::showForm()
{
    ui->retranslateUi(this);
    ui->text6->setVisible(false);
    ui->light_6->setVisible(false);
    ui->text7->setVisible(false);
    ui->light_7->setVisible(false);

    canActive(CAN1_DEV, varList, N(varList));
    this->show();
}
void FORM_ControlPort5::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_ControlPort5::loadFunctionKey(void)
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
void FORM_ControlPort5::onRelease_Button1()
{
    if(ControlPort2 == 0)
        ControlPort2 = new FORM_ControlPort2;
    ControlPort2->showForm();
    closeForm();
}
void FORM_ControlPort5::onRelease_Button2()
{
    if(ControlPort4 == 0)
        ControlPort4 = new FORM_ControlPort4;
    ControlPort4->showForm();
    closeForm();
}

void FORM_ControlPort5::onRelease_Button3()
{
    if(ControlPort6 == 0){
        ControlPort6 = new FORM_ControlPort6;
        qDebug()<<"ControlPort6:"<<ControlPort6;
    }else {
        qDebug()<<"ControlPort6:"<<0;
    }
    ControlPort6->showForm();
    closeForm();
}

void FORM_ControlPort5::onRelease_Button4()
{

}

void FORM_ControlPort5::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

/********************************************************************/
void FORM_ControlPort5::onCanRecv(int code, msg_can_t *m)
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
            case C1_R04FB0006B5://
                   val = uint16_t(val);
                   //DO1-转向蓄能器卸荷电磁阀
                   if(unPackValue(val,0)==1) ui->light_1->setPixmap(sGreen);
                   else ui->light_1->setPixmap(sGray);
                   //DO2-装载制动电磁阀
                   if(unPackValue(val,1)==1) ui->light_2->setPixmap(sGreen);
                   else ui->light_2->setPixmap(sGray);
                   //DO3-举升限位电磁铁
                   if(unPackValue(val,2)==1) ui->light_3->setPixmap(sGreen);
                   else ui->light_3->setPixmap(sGray);
                break;
            case C1_R04FB0006B6:
                //DO6-车梯上升电磁铁
                if(unPackValue(val,1)==1) ui->light_8->setPixmap(sGreen);
                else ui->light_8->setPixmap(sGray);
                //DO7-车梯下降电磁铁
                if(unPackValue(val,2)==1) ui->light_11->setPixmap(sGreen);
                else ui->light_11->setPixmap(sGray);
                //DO8-车梯上升到位指示灯
                if(unPackValue(val,3)==1) ui->light_9->setPixmap(sGreen);
                else ui->light_9->setPixmap(sGray);
                //DO9-车梯下降到位指示灯
                if(unPackValue(val,4)==1) ui->light_10->setPixmap(sGreen);
                else ui->light_10->setPixmap(sGray);
                //DO10-车梯室内蜂鸣器
                if(unPackValue(val,5)==1) ui->light_6->setPixmap(sGreen);
                else ui->light_6->setPixmap(sGray);
                //DO11-车梯室外蜂鸣器
                if(unPackValue(val,6)==1) ui->light_7->setPixmap(sGreen);
                else ui->light_7->setPixmap(sGray);
                break;
            case C1_R04FB0006B7://
                   val = uint16_t(val);
                   //DO4-刹车灯信号
                   if(unPackValue(val,3)==1) ui->light_4->setPixmap(sGreen);
                   else ui->light_4->setPixmap(sGray);
                   //DO5-电缓行灯
                   if(unPackValue(val,4)==1) ui->light_5->setPixmap(sGreen);
                   else ui->light_5->setPixmap(sGray);
                break;
            }
            break;
        }
    }
}

void FORM_ControlPort5:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_ControlPort5::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
