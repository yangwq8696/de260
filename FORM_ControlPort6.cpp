#include "FORM_ControlPort6.h"
#include "ui_FORM_ControlPort6.h"
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
    { C1_R04FC0001B34,   0x0000ffff,     1,       0   },
    { C1_R04FB0002B12,   0x0000ffff,     1,       0   },
    { C1_R04FC0001B12,   0x0000ffff,     1,       0   },
    { C1_R04FB0001B56,   0x0000ffff,     1,       0   },
    { C1_R04FB0001B78,   0x0000ffff,     1,       0   },
    { C1_R04FB0001B12,   0x0000ffff,     1,       0   },
    { C1_R04FB0001B34,   0x0000ffff,     1,       0   },
    { C1_R04FB0006B3,   0x000000ff,     1,       0   },
};

FORM_ControlPort6::FORM_ControlPort6(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_ControlPort6),
    m_uiInited(false)
{
    ui->setupUi(this);

    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_ControlPort6::~FORM_ControlPort6()
{
    delete ui;
}
/********************************************************************/
void FORM_ControlPort6::showForm()
{
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList));
    this->show();
}
void FORM_ControlPort6::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_ControlPort6::loadFunctionKey(void)
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
void FORM_ControlPort6::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}

void FORM_ControlPort6::onRelease_Button2()
{
    if(ControlPort5 == 0)
        ControlPort5 = new FORM_ControlPort5;
    ControlPort5->showForm();
    closeForm();
}

void FORM_ControlPort6::onRelease_Button3()
{
    if(ControllerPort == 0)
        ControllerPort = new FORM_ControllerPort;
    ControllerPort->showForm();
    closeForm();
}

void FORM_ControlPort6::onRelease_Button4()
{

}

void FORM_ControlPort6::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

/********************************************************************/
void FORM_ControlPort6::onCanRecv(int code, msg_can_t *m)
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
            case C1_R04FB0006B3:
                //AI12-车梯上升按钮
                if(unPackValue(val,0)==1) ui->light_18->setPixmap(sGreen);
                else ui->light_18->setPixmap(sGray);
                //AI13-车梯下降按钮1
                if(unPackValue(val,1)==1) ui->light_10->setPixmap(sGreen);
                else ui->light_10->setPixmap(sGray);
                //AI14-车梯下降按钮2
                if(unPackValue(val,2)==1) ui->light_19->setPixmap(sGreen);
                else ui->light_19->setPixmap(sGray);
                //AI15-车梯上升到位
                if(unPackValue(val,3)==1) ui->light_20->setPixmap(sGreen);
                else ui->light_20->setPixmap(sGray);
                //AI16-车梯下降到位
                if(unPackValue(val,4)==1) ui->light_21->setPixmap(sGreen);
                else ui->light_21->setPixmap(sGray);
                break;
            case C1_R04FC0001B34://转向阀压力
                    ui->value_1->setText(QString::number(val)+" bar");
                break;
            case C1_R04FB0002B12://转向蓄能器压力
                    ui->value_2->setText(QString::number(val)+" bar");
                break;
            case C1_R04FC0001B12://举升阀压力
                    ui->value_3->setText(QString::number(val)+" bar");
                break;
            case C1_R04FB0001B56://前桥制动蓄能器压力
                    ui->value_4->setText(QString::number(val)+" bar");
                break;
            case C1_R04FB0001B78://后桥制动蓄能器压力
                    ui->value_5->setText(QString::number(val)+" bar");
                break;
            case C1_R04FB0001B12://前桥制动压力
                    ui->value_6->setText(QString::number(val)+" bar");
                break;
            case C1_R04FB0001B34://后桥制动压力
                    ui->value_7->setText(QString::number(val)+" bar");
                break;
            }
            break;
        }
    }
}

void FORM_ControlPort6:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}

void FORM_ControlPort6::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
