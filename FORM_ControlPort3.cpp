#include "FORM_ControlPort3.h"
#include "ui_FORM_ControlPort3.h"
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
    { C1_R04FA0001B1,   0x000000ff,     1,       0   },
    { C1_R04FA0001B2,   0x000000ff,     1,       0   },
    { C1_R04FA0009B2,   0x000000ff,     1,       0   },
    { C1_R04FA0002B56,   0x0000ffff,     1,       0   },
};

FORM_ControlPort3::FORM_ControlPort3(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_ControlPort3),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_ControlPort3::~FORM_ControlPort3()
{
    delete ui;
}
/********************************************************************/
void FORM_ControlPort3::showForm()
{
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList));
    this->show();    
}
void FORM_ControlPort3::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_ControlPort3::loadFunctionKey(void)
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
void FORM_ControlPort3::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_ControlPort3::onRelease_Button2()
{
    if(ControlPort2 == 0)
        ControlPort2 = new FORM_ControlPort2;
    ControlPort2->showForm();
    closeForm();
}
void FORM_ControlPort3::onRelease_Button3()
{
    if(ControlPort4 == 0)
        ControlPort4 = new FORM_ControlPort4;
    ControlPort4->showForm();
    closeForm();
}
void FORM_ControlPort3::onRelease_Button4()
{

}
void FORM_ControlPort3::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}
/********************************************************************/
void FORM_ControlPort3::onCanRecv(int code, msg_can_t *m)
{
    qreal val;

    if (code != MSG_CAN_OD)
        return;
    for (unsigned int i=0; i<N(varList); i++)
    {
        if (varList[i].var == m->var)
        {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

            switch (m->var)
            {
                case C1_R04FA0001B1://
                   ui->value_1->setText(QString::number(val-40)+" ℃");
                break;
                case C1_R04FA0001B2:
                   ui->value_2->setText(QString::number(val)+" %");
                break;
                case C1_R04FA0009B2:
                   ui->value_3->setText(QString::number(unPackValue(uint16_t(val),4)));
                break;
            case C1_R04FA0002B56:
               ui->value_4->setText(QString::number(val)+" bar");
            break;
            }
            break;
        }
    }
}

void FORM_ControlPort3:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_ControlPort3::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}

