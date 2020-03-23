#include "FORM_HydraulicQuery.h"
#include "ui_FORM_HydraulicQuery.h"
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
    { C1_R04FA0001B1,    0x000000ff,     1,       0   },
    { C1_R04FA0002B56,   0x0000ffff,     1,       0   },
};


FORM_HydraulicQuery::FORM_HydraulicQuery(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_HydraulicQuery),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_HydraulicQuery::~FORM_HydraulicQuery()
{
    delete ui;
}
/********************************************************************/
void FORM_HydraulicQuery::showForm()
{
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList));
    showFullScreen();

}
void FORM_HydraulicQuery::closeForm()
{
    this->hide();
    canDeactive();
}
/********************************************************************/

/********************************************************************/
void FORM_HydraulicQuery::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_HydraulicQuery::onRelease_Button2()
{

}
void FORM_HydraulicQuery::onRelease_Button3()
{

}
void FORM_HydraulicQuery::onRelease_Button4()
{

}
void FORM_HydraulicQuery::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

/********************************************************************/
void FORM_HydraulicQuery::onCanRecv(int code, msg_can_t *m)
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
                case C1_R04FC0001B34://转向阀压力
                     strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" bar");
                     ui->value1->setText(strHTML);
                     break;
                case C1_R04FB0002B12://转向蓄能器压力
                     strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" bar");
                    ui->value2->setText(strHTML);
                    break;
                case C1_R04FC0001B12://举升阀压力
                     strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" bar");
                    ui->value3->setText(strHTML);
                    break;
                case C1_R04FB0001B56://前桥制动蓄能器压力
                     strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" bar");
                     ui->value4->setText(strHTML);
                     break;
                case C1_R04FB0001B78://后桥制动蓄能器压力
                     strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" bar");
                     ui->value5->setText(strHTML);
                     break;
                case C1_R04FB0001B12://前桥制动压力
                     strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" bar");
                     ui->value6->setText(strHTML);
                     break;
                case C1_R04FB0001B34://后桥制动压力
                     strHTML = QString("<html> \
                                <head> \
                                <style> \
                                font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                </style> \
                                </head> \
                                <body> \
                                <font>%1</font><font id=\"f\">%2</font> \
                                </body> \
                                </html>").arg(val).arg(" bar");
                     ui->value7->setText(strHTML);
                     break;
                case C1_R04FA0001B1://液压油箱油温
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
                     ui->value8->setText(strHTML);
                     break;
                case C1_R04FA0002B56://驻车制动压力
                     strHTML = QString("<html> \
                                 <head> \
                                 <style> \
                                 font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                 </style> \
                                 </head> \
                                 <body> \
                                 <font>%1</font><font id=\"f\">%2</font> \
                                 </body> \
                                 </html>").arg(val).arg(" bar");
                     ui->value9->setText(strHTML);
                     break;
            }
            break;
        }
    }
}

void FORM_HydraulicQuery:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_HydraulicQuery::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
