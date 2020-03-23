#include "FORM_MotoQuery.h"
#include "ui_FORM_MotoQuery.h"
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
    { C1_R0CF00400B45,   0x0000ffff,     1,       0   },
    { C1_R04FA0003B1234,   0xffffffff,     1,       0   },
    { C1_R04FA0001B56,   0x0000ffff,     1,       0   },
    { C1_R04FA0001B2,   0x000000ff,     1,       0   },
    { C1_R04FA0002B34,   0x0000ffff,     1,       0   },
    { C1_R04FA0004B1234,   0xffffffff,     1,       0   },
    { C1_R04FA0001B3,   0x000000ff,     1,       0   },
    { C1_R04FA0003B5678,   0xffffffff,     1,       0   },
};


FORM_MotoQuery::FORM_MotoQuery(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_MotoQuery),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_MotoQuery::~FORM_MotoQuery()
{
    delete ui;
}
/********************************************************************/
void FORM_MotoQuery::showForm()
{
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList));
    showFullScreen();
}
void FORM_MotoQuery::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
void FORM_MotoQuery::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_MotoQuery::onRelease_Button2()
{

}
void FORM_MotoQuery::onRelease_Button3()
{

}
void FORM_MotoQuery::onRelease_Button4()
{

}
void FORM_MotoQuery::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

/********************************************************************/
void FORM_MotoQuery::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    QString strHTML;

    if (code != MSG_CAN_OD)
        return;
    for (unsigned int i=0; i<N(varList); i++)
    {
        if (varList[i].var == m->var)
        {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

            switch (m->var)
            {
                case C1_R0CF00400B45://发动机转速
                         val = int(val*0.125);
                         strHTML = QString("<html> \
                                       <head> \
                                       <style> \
                                        font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                       </style> \
                                       </head> \
                                       <body> \
                                       <font>%1</font><font id=\"f\">%2</font> \
                                       </body> \
                                       </html>").arg(val).arg(" rmp");
                        ui->value1->setText(strHTML);
                        break;
                case C1_R04FA0003B1234://发动机小时
                        strHTML = QString("<html> \
                                       <head> \
                                       <style> \
                                        font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                       </style> \
                                       </head> \
                                       <body> \
                                       <font>%1</font><font id=\"f\">%2</font> \
                                       </body> \
                                       </html>").arg(val*0.1).arg(" h");
                        ui->value2->setText(strHTML);
                        break;
                case C1_R04FA0001B56://机油压力
                        strHTML = QString("<html> \
                               <head> \
                               <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                               </style> \
                               </head> \
                               <body> \
                               <font>%1</font><font id=\"f\">%2</font> \
                               </body> \
                               </html>").arg(val).arg(" kPa");
                         ui->value3->setText(strHTML);
                         break;
                case C1_R04FA0001B2://燃油油位
                        strHTML = QString("<html> \
                                <head> \
                                <style> \
                                   font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" %");
                         ui->value4->setText(strHTML);
                         break;
                case C1_R04FA0002B34://实时油耗
                        strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val*0.05).arg(" L");
                        ui->value5->setText(strHTML);
                        break;
                case C1_R04FA0004B1234://里程油耗
                        strHTML = QString("<html> \
                                 <head> \
                                 <style> \
                                 font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                 </style> \
                                 </head> \
                                 <body> \
                                 <font>%1</font><font id=\"f\">%2</font> \
                                 </body> \
                                 </html>").arg(val*0.5).arg(" L");
                        ui->value6->setText(strHTML);
                        break;
                case C1_R04FA0001B3://水温
                        strHTML = QString("<html> \
                                 <head> \
                                 <style> \
                                 font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                 </style> \
                                 </head> \
                                 <body> \
                                 <font>%1</font><font id=\"f\">%2</font> \
                                 </body> \
                                 </html>").arg(val-40).arg(" ℃");
                        ui->value7->setText(strHTML);
                        break;
                case C1_R04FA0003B5678://总里程
                        strHTML = QString("<html> \
                                 <head> \
                                 <style> \
                                 font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                 </style> \
                                 </head> \
                                 <body> \
                                 <font>%1</font><font id=\"f\">%2</font> \
                                 </body> \
                                 </html>").arg(val*0.1).arg(" km");
                        ui->value8->setText(strHTML);
                        break;
            }
            break;
        }
    }
}

void FORM_MotoQuery:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_MotoQuery::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
