#include "Form_GE.h"
#include "ui_Form_GE.h"
#include "main.h"
#include "wika_GeneralWidget.h"


/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] =
{
    { C1_R18ff0507B12,   0x0000ffff,     0.1,       0   },
    { C1_R18ff0507B34,   0x0000ffff,     0.1,       0   },
    { C1_R18ff0507B56,   0x0000ffff,     0.1,       0   },
    { C1_R18ff0507B78,   0x0000ffff,     0.1,       0   },
    { C1_R18ff0607B1,    0x0000ff,       1,         0   },
    { C1_R18ff0607B2,    0x0000ff,       1,         0   },
    { C1_R18ff0607B3,    0x0000ff,       1,         0   },
    { C1_R18ff0707B34,   0x0000ffff,     1,         0   },
    { C1_R18ff0707B56,   0x0000ffff,     1,         0   },
    { C1_R18ff0707B78,   0x0000ffff,     0.1,       0   },
    { C1_R18ff0807B6,    0x0000ff,       1,         0   },
    { C1_R18ff0907B12,   0x0000ffff,     13.55,     0   },
    { C1_R18ff0907B34,   0x0000ffff,     13.55,     0   },
    { C1_R18ff0907B56,   0x0000ffff,     1,         0   },
    { C1_R18ff0907B78,   0x0000ffff,     1,         0   },


};


Form_GE::Form_GE(QWidget *parent) :
    Page(parent),
    ui(new Ui::Form_GE)
{
    ui->setupUi(this);
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

Form_GE::~Form_GE()
{
    delete ui;
}

void Form_GE::showForm()
{
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList));
    showFullScreen();
}

void Form_GE::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);
    canDeactive();
}

void Form_GE:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));
}


/********************************************************************/
void Form_GE::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void Form_GE::onRelease_Button2()
{

}
void Form_GE::onRelease_Button3()
{

}
void Form_GE::onRelease_Button4()
{

}
void Form_GE::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}


void Form_GE::onCanRecv(int code, msg_can_t *m)
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
            case C1_R18ff0507B12: //前左轮速度
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val*100/62.0,0,'f',1).arg(" km/h");
                ui->text1_5->setText(strHTML);
                break;
            case C1_R18ff0507B34: //前右轮速度
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val*100/62.0,0,'f',1).arg(" km/h");
                ui->text1_6->setText(strHTML);
                break;
            case C1_R18ff0507B56: //后左轮速度
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val*100/62.0,0,'f',1).arg(" km/h");
                ui->text1_7->setText(strHTML);
                break;
            case C1_R18ff0507B78: //后右轮速度
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val*100/62.0,0,'f',1).arg(" km/h");
                ui->text1_8->setText(strHTML);
                break;
            case C1_R18ff0607B1: //加速踏板百分比
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
                ui->text1_9->setText(strHTML);
                break;
            case C1_R18ff0607B2: //制动踏板百分比
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
                ui->text1_11->setText(strHTML);
                break;
            case C1_R18ff0607B3: //档位状态编码
                if(val == 0)
                    ui->text1_14->setText("Neutral");
                else if(val == 1)
                    ui->text1_14->setText("Forward");
                else if(val == 2)
                    ui->text1_14->setText("Reverse");
                else
                    ui->text1_14->setText("");;
                break;
            case C1_R18ff0707B34: //发电机输入马力
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val,0,'f',1).arg(" HP");
                ui->text1_16->setText(strHTML);
                break;
            case C1_R18ff0707B56: //制动马力
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val,0,'f',1).arg(" HP");
                ui->text1_17->setText(strHTML);
                break;
            case C1_R18ff0707B78: //母线电压
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val,0,'f',1).arg(" V");
                ui->text1_20->setText(strHTML);
                break;
            case C1_R18ff0807B6: //环境温度
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" ℃");
                ui->text1_21->setText(strHTML);
                break;
            case C1_R18ff0907B12: //电机1转矩
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val,0,'f',1).arg(" Ｎ*m");
                ui->text1_23->setText(strHTML);
                break;
            case C1_R18ff0907B34: //电机2转矩
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val,0,'f',1).arg(" Ｎ*m");
                ui->text1_26->setText(strHTML);
                break;
            case C1_R18ff0907B56: //电机1转速
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" rpm");
                ui->text1_28->setText(strHTML);
                break;
            case C1_R18ff0907B78: //电机2转速
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22);} #f{color: rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" rpm");
                ui->text1_29->setText(strHTML);
                break;
            }

            break;
        }
    }
}
