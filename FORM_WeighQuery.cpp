#include "FORM_WeighQuery.h"
#include "ui_FORM_WeighQuery.h"
#include <QPainter>
#include "main.h"
#include "wika_GeneralWidget.h"

/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] =
{
    { C1_R04FA0014B1,   0x000000ff,     1,       0   },
    { C1_R04FA0014B2,   0x000000ff,     1,       0   },
    { C1_R04FA0014B3,   0x000000ff,     1,       0   },
    { C1_R04FA0014B4,   0x000000ff,     1,       0   },
    { C1_R04FA0014B5,   0x000000ff,     1,       0   },
    { C1_R04FA0014B6,   0x000000ff,     1,       0   },
    { C1_R04FA0014B78,   0x0000ffff,     1,       0   }
};


FORM_WeighQuery::FORM_WeighQuery(Page *parent) :
    Page(parent),
    m_uiInited(false),
    ui(new Ui::FORM_WeighQuery)
{
    ui->setupUi(this);
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_WeighQuery::~FORM_WeighQuery()
{
    delete ui;
}

void FORM_WeighQuery::showForm(){
    ui->retranslateUi(this);
    canActive(CAN1_DEV, varList, N(varList));

    const QPixmap sGray(":/pic_de260/ico_27.png");
    bool bMainCtrl = getMainCtrlState();
    if(!bMainCtrl){
        ui->light_1->setPixmap(sGray);
        ui->light_2->setPixmap(sGray);
        ui->light_3->setPixmap(sGray);
        ui->light_4->setPixmap(sGray);
    }

    showFullScreen();
}

void FORM_WeighQuery::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);
    canDeactive();
}

void FORM_WeighQuery:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));
}


void FORM_WeighQuery::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    const QPixmap sGreen(":/pic_de260/ico_25.png");
//    const QPixmap sRed(":/pic_de260/ico_28.png");
    const QPixmap sGray(":/pic_de260/ico_27.png");
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
            case C1_R04FA0014B1: //称重报警
                val = (uint8_t)val;
                if(getMainCtrlState()){
                    if(unPackValue(val,0)==1)
                        ui->light_1->setPixmap(sGreen);
                    if(unPackValue(val,0)==0)
                        ui->light_1->setPixmap(sGray);

                    if(unPackValue(val,1)==1)
                        ui->light_2->setPixmap(sGreen);
                    if(unPackValue(val,1)==0 )
                        ui->light_2->setPixmap(sGray);

                    if(unPackValue(val,2)==1)
                        ui->light_3->setPixmap(sGreen);
                    if(unPackValue(val,2)==0)
                        ui->light_3->setPixmap(sGray);

                    if(unPackValue(val,3)==1)
                        ui->light_4->setPixmap(sGreen);
                    if(unPackValue(val,3)==0 )
                        ui->light_4->setPixmap(sGray);
                }
                else{
                    ui->light_1->setPixmap(sGray);
                    ui->light_2->setPixmap(sGray);
                    ui->light_3->setPixmap(sGray);
                    ui->light_4->setPixmap(sGray);
                }
                break;
            case C1_R04FA0014B2: //版本
                val = (uint8_t)val;
                ui->value_1->setText(QString::number(val));
                break;
            case C1_R04FA0014B3: //左前称重传感器压力
                val = (uint8_t)val;
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22)} #f{color: rgb(115, 210, 22)} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" bar");
                ui->value_2->setText(strHTML);
                break;
            case C1_R04FA0014B4: //右前称重传感器压力
                val = (uint8_t)val;
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22)} #f{color: rgb(115, 210, 22)} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" bar");
                ui->value_3->setText(strHTML);
                break;
            case C1_R04FA0014B5: //右后称重传感器压力
                val = (uint8_t)val;
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22)} #f{color: rgb(115, 210, 22)} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" bar");
                ui->value_4->setText(strHTML);
                break;
            case C1_R04FA0014B6: //左后称重传感器压力
                val = (uint8_t)val;
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22)} #f{color: rgb(115, 210, 22)} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" bar");
                ui->value_5->setText(strHTML);
                break;
            case C1_R04FA0014B78: //时时载重量
                val = (int)val;
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color: rgb(115, 210, 22)} #f{color: rgb(115, 210, 22)} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" t");
                ui->value_6->setText(strHTML);
                break;

            }

            break;
        }
    }
}

void FORM_WeighQuery::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_WeighQuery::onRelease_Button2()
{

}
void FORM_WeighQuery::onRelease_Button3()
{

}
void FORM_WeighQuery::onRelease_Button4()
{

}
void FORM_WeighQuery::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}
