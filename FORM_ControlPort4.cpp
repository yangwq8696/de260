#include "FORM_ControlPort4.h"
#include "ui_FORM_ControlPort4.h"
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
    { C1_R04FB0004B3,   0x000000ff,     1,       0   },
    { C1_R04FB0004B2,   0x000000ff,     1,       0   },    
    { C1_R04FB0006B1,   0x000000ff,     1,       0   },
    { C1_R04FB0006B2,   0x000000ff,     1,       0   },
};

FORM_ControlPort4::FORM_ControlPort4(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_ControlPort4),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_ControlPort4::~FORM_ControlPort4()
{
    delete ui;
}
/********************************************************************/
void FORM_ControlPort4::showForm()
{
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList)); //激活can监听，并配置监听的can变量
    this->show();

}
void FORM_ControlPort4::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_ControlPort4::loadFunctionKey(void)
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
void FORM_ControlPort4::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_ControlPort4::onRelease_Button2()
{
    if(ControlPort3 == 0)
        ControlPort3 = new FORM_ControlPort3;
    ControlPort3->showForm();
    closeForm();
}
void FORM_ControlPort4::onRelease_Button3()
{
    if(ControlPort5 == 0)
        ControlPort5 = new FORM_ControlPort5;
    ControlPort5->showForm();
    closeForm();
}
void FORM_ControlPort4::onRelease_Button4()
{

}
void FORM_ControlPort4::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

/********************************************************************/
void FORM_ControlPort4::onCanRecv(int code, msg_can_t *m)
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
                case C1_R04FB0004B3://
                   val = uint16_t(val);
                   //DI1-接近开关前
                   if(unPackValue(val,4)==1) ui->light_1->setPixmap(sGreen);
                   else ui->light_1->setPixmap(sGray);
                   //DI2-接近开关后
                   if(unPackValue(val,5)==1) ui->light_2->setPixmap(sGreen);
                   else ui->light_2->setPixmap(sGray);
                break;
                case C1_R04FB0006B2://
                    val = uint16_t(val);
                    //DI17-转向蓄能器预冲压力低报警1
                    if(unPackValue(val,0)==1) ui->light_11->setPixmap(sGreen);
                    else ui->light_11->setPixmap(sGray);
                    //DI18-转向蓄能器预冲压力低报警2
                    if(unPackValue(val,1)==1) ui->light_12->setPixmap(sGreen);
                    else ui->light_12->setPixmap(sGray);
                    //DI20-制动蓄能器预冲压力报警1
                    if(unPackValue(val,3)==1) ui->light_15->setPixmap(sGreen);
                    else ui->light_15->setPixmap(sGray);
                    //DI21-制动蓄能器预冲压力报警2
                    if(unPackValue(val,4)==1) ui->light_16->setPixmap(sGreen);
                    else ui->light_16->setPixmap(sGray);
                    //DI22-制动蓄能器预冲压力报警3
                    if(unPackValue(val,5)==1) ui->light_17->setPixmap(sGreen);
                    else ui->light_17->setPixmap(sGray);
                    //DI9-驾驶室右门检测
                    if(unPackValue(val,6)==1) ui->light_6->setPixmap(sGreen);
                    else ui->light_6->setPixmap(sGray);
                    //DI4-主驾安全带检测
                    if(unPackValue(val,7)==1) ui->light_4->setPixmap(sGreen);
                    else ui->light_4->setPixmap(sGray);
                break;
            case C1_R04FB0006B1://
                    val = uint16_t(val);
                    //DI12-回油过滤器堵塞报警(前)
                    if(unPackValue(val,0)==1) ui->light_7->setPixmap(sGreen);
                    else ui->light_7->setPixmap(sGray);
                    //DI13-回油过滤器堵塞报警(后)
                    if(unPackValue(val,1)==1) ui->light_8->setPixmap(sGreen);
                    else ui->light_8->setPixmap(sGray);
                    //DI14-高压过滤器堵塞报警(前)
                    if(unPackValue(val,5)==1) ui->light_9->setPixmap(sGreen);
                    else ui->light_9->setPixmap(sGray);
                    //DI14-高压过滤器堵塞报警(中)
                    if(unPackValue(val,6)==1) ui->light_13->setPixmap(sGreen);
                    else ui->light_13->setPixmap(sGray);
                    //DI14-高压过滤器堵塞报警(后)
                    if(unPackValue(val,7)==1) ui->light_14->setPixmap(sGreen);
                    else ui->light_14->setPixmap(sGray);
//                    //DI5-高压过滤器堵塞报警1
//                    if(unPackValue(val,5)==1) ui->light_5->setPixmap(sGreen);
//                    else ui->light_5->setPixmap(sGray);

                break;
            }
            break;
        }
    }
}

void FORM_ControlPort4:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_ControlPort4::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
