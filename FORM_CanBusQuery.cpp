#include "FORM_CanBusQuery.h"
#include "ui_FORM_CanBusQuery.h"
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
    { C1_R04FA0004B6,   0x000000ff,     1,       0   },
//    { C1_R04FA0001B2,   0x000000ff,     1,       0   },
};

FORM_CanBusQuery::FORM_CanBusQuery(Page *parent) :
    Page(parent),
    m_timer(0),
    ui(new Ui::FORM_CanBusQuery),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);

    //总线心跳
    m_count_canBas = 0;
    m_canBusData = 0;
    m_canBusDataOld = 0;
    m_count = 0;

    m_canBusState = false;
}

FORM_CanBusQuery::~FORM_CanBusQuery()
{
    delete ui;
    if (m_timer){ delete m_timer; m_timer=0;}
}
/********************************************************************/
void FORM_CanBusQuery::showForm()
{
    ui->retranslateUi(this);
    initView();

    if (m_timer){ delete m_timer; m_timer=0;}
    m_timer = new QTimer();
    m_timer->stop();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(canBusStateDisplay()));
    m_timer->start(1000);

    canActive(CAN1_DEV, varList, N(varList));
    this->show();

}
void FORM_CanBusQuery::closeForm()
{
    m_timer->stop();
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);
    canDeactive();
}
void FORM_CanBusQuery::initView(){
    /*初始化灯光*/
    const QPixmap sRed(":/pic_de260/ico_28.png");
    ui->light_1->setPixmap(sRed);
     ui->light_2->setPixmap(sRed);
     ui->light_3->setPixmap(sRed);
     ui->light_4->setPixmap(sRed);
     ui->light_5->setPixmap(sRed);
     ui->light_6->setPixmap(sRed);
     ui->light_7->setPixmap(sRed);
}
/********************************************************************/

void FORM_CanBusQuery::canBusStateDisplay(){
    const QPixmap sGreen(":/pic_de260/ico_25.png");
    const QPixmap sRed(":/pic_de260/ico_28.png");


    //can线断开
    m_canBusData = canopen_read_var_int(CAN_DEV1_ID,C1_R04FA0004B6,NULL);
    if(m_canBusData == m_canBusDataOld)
        m_count++;
    //    qDebug()<<"m_canBusData:"<<m_canBusData<<"m_canBusData_Old:"<<m_canBusDataOld;
    if(m_count >= 3){//不跳变或者can线断开
            ui->light_1->setPixmap(sRed);
            ui->light_2->setPixmap(sRed);
            ui->light_3->setPixmap(sRed);
            ui->light_4->setPixmap(sRed);
            ui->light_5->setPixmap(sRed);
            ui->light_6->setPixmap(sRed);
            ui->light_7->setPixmap(sRed);
            return;
    }else{//跳变
        ui->light_1->setPixmap(sGreen);
        //与故障代码关联的总线
        setCanState();
    }
}

void FORM_CanBusQuery::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    const QPixmap sGreen(":/pic_de260/ico_25.png");
    const QPixmap sRed(":/pic_de260/ico_28.png");

    if (code != MSG_CAN_OD)
        return;
    for (unsigned int i=0; i<N(varList); i++)
    {
        if (varList[i].var == m->var)
        {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

            switch (m->var)
            {
            case C1_R04FA0004B6:
                if(val != m_canBusDataOld)//跳变
                    m_count=0;
                m_canBusDataOld = val;

//                qDebug()<<"m_canBusDataOld:"<<m_canBusDataOld
//                          <<"m_canBusState:"<<m_canBusState
//                            <<"m_count:"<<m_count;
                break;
            }
            break;
        }
    }
}

/********************************************************************/
void FORM_CanBusQuery::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_CanBusQuery::onRelease_Button2()
{

}
void FORM_CanBusQuery::onRelease_Button3()
{

}
void FORM_CanBusQuery::onRelease_Button4()
{

}
void FORM_CanBusQuery::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

void FORM_CanBusQuery:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));
}

void FORM_CanBusQuery::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}

bool FORM_CanBusQuery::getEngineState(){
    bool ret = 1;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==252)
            ret = 0;
    }
    return ret;
}

bool FORM_CanBusQuery::getButtonPanelState(){
    bool ret = 1;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==210)
            ret = 0;
    }
    return ret;

}

bool FORM_CanBusQuery::getMeterCanState(){
    bool ret = 1;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==211)
            ret = 0;
    }
    return ret;
}

bool FORM_CanBusQuery::getGECanState(){
    bool ret = 1;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==281)
            ret = 0;
    }
    return ret;

}

bool FORM_CanBusQuery::getWeighingSystemState(){
    bool ret = 1;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==216)
            ret = 0;
    }
    return ret;
}

bool FORM_CanBusQuery::getHydraulicControllerState(){
    bool ret = 1;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==214)
            ret = 0;
    }
    return ret;

}

bool FORM_CanBusQuery::getAuxiliaryContrullerState(){
    bool ret = 1;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==215)
            ret = 0;
    }
    return ret;

}

void FORM_CanBusQuery::setCanState(){
    const QPixmap sGreen(":/pic_de260/ico_25.png");
    const QPixmap sRed(":/pic_de260/ico_28.png");

    //仪表
    if(getMeterCanState()){
        ui->light_4->setPixmap(sGreen);
    }else{
        ui->light_4->setPixmap(sRed);
    }
    //发动机
    if(getEngineState())
        ui->light_2->setPixmap(sGreen);
    else
        ui->light_2->setPixmap(sRed);

    //按钮板
    if(getButtonPanelState())
        ui->light_5->setPixmap(sGreen);
    else
        ui->light_5->setPixmap(sRed);

    //GE
    if(getGECanState())
        ui->light_6->setPixmap(sGreen);
    else
        ui->light_6->setPixmap(sRed);

    //液压
    if(getHydraulicControllerState())
        ui->light_3->setPixmap(sGreen);
    else
        ui->light_3->setPixmap(sRed);

    //称重
    if(getWeighingSystemState())
        ui->light_7->setPixmap(sGreen);
    else
        ui->light_7->setPixmap(sRed);
}
