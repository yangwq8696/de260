#include "FORM_GPSQuery.h"
#include "ui_FORM_GPSQuery.h"
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
    { C1_R18FE08F4B2,   0x000000ff,     1,       0   },
    { C1_R18FE08F4B3,   0x000000ff,     1,       0   },
    { C1_R18FE08F4B4,   0x000000ff,     1,       0   },

    { C1_R18FE0FF6B1,   0x000000ff,     1,       0   },
    { C1_R18FE0FF6B2,   0x000000ff,     1,       0   },
    { C1_R18FE0FF6B34,   0x0000ffff,     1,       0   },
    { C1_R18FE0FF6B5,   0x000000ff,     1,       0   },

    { C1_R18FE0FF7B1,   0x000000ff,     1,       0   },
    { C1_R18FE0FF7B2,   0x000000ff,     1,       0   },
    { C1_R18FE0FF7B3,   0x000000ff,     1,       0   },
    { C1_R18FE0FF7B4,   0x000000ff,     1,       0   },
    { C1_R18FE0FF7B5,   0x000000ff,     1,       0   },
    { C1_R18FE0FF7B6,   0x000000ff,     1,       0   },
    { C1_R18FE0FF7B7,   0x000000ff,     1,       0   },
    { C1_R18FE0FF7B8,   0x000000ff,     1,       0   }
};


FORM_GPSQuery::FORM_GPSQuery(Page *parent) :
    Page(parent),
    m_timer(0),
    ui(new Ui::FORM_GPSQuery),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner(); //
    m_SN = "";
    m_EW = "";
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_GPSQuery::~FORM_GPSQuery()
{
    if (m_timer) delete m_timer;
    delete ui;
}
/********************************************************************/
void FORM_GPSQuery::showForm()
{
    ui->retranslateUi(this);
    if (m_timer){ delete m_timer; m_timer=0;}
    m_timer = new QTimer(this);
    m_timer->stop();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(GPSstateDisplay()));
    m_timer->start(500);

    canActive(CAN1_DEV, varList, N(varList));
    showFullScreen();
}
void FORM_GPSQuery::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);
    canDeactive();
}
/********************************************************************/

void FORM_GPSQuery::onCanRecv(int code, msg_can_t *m)
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
            case C1_R18FE08F4B2: //gps电源状态
                val = (uint8_t)val;
                m_powerState=val;
                break;
            case C1_R18FE08F4B3: //gps天线状态
                val = (uint8_t)val;
                m_lineState = val;
                break;
            case C1_R18FE08F4B4: //SIM卡状态
                val = (uint8_t)val;
                m_SIMState = val;
                break;
            case C1_R18FE0FF6B1: //定位标志
                val = (uint8_t)val;
                m_bEnabled = val;  //1--已定位,0--未定位
                break;
            case C1_R18FE0FF6B2: //东西南北
                val = (uint8_t)val;
                if(unPackValue(val,0)==1) //北纬
                    m_SN = "N";
                else //南纬
                    m_SN = "S";
                if(unPackValue(val,1)==1) //东经
                    m_EW = "E";
                else //西经
                    m_EW = "W";
                break;
            case C1_R18FE0FF6B34: //海拔
                val = (int)val;
                strHTML = QString("<html> \
                                   <head> \
                                   <style> \
                                    font{color:rgb(115, 210, 22);} #f{color:rgb(115, 210, 22);} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font><font id=\"f\">%2</font> \
                                   </body> \
                                   </html>").arg(val).arg(" m");
                m_Altitude = strHTML;
                break;
            case C1_R18FE0FF6B5: //卫星个数
                val = (int)val;
                m_satelliteNum = val;
                break;
            case C1_R18FE0FF7B1: //经度 度
                val = (int)val;
                m_degree_jing = val;
                break;
            case C1_R18FE0FF7B2: //经度 分
                val = (int)val;
                m_min_jing = val;
                break;
            case C1_R18FE0FF7B3: //经度 秒1
                val = (int)val;
                m_second1_jing = val;
                break;
            case C1_R18FE0FF7B4: //经度 秒2
                val = (int)val;
                m_second2_jing = val;
                break;
            case C1_R18FE0FF7B5: //纬度 度
                val = (int)val;
                m_degree_wei = val;
                break;
            case C1_R18FE0FF7B6: //纬度 分
                val = (int)val;
                m_min_wei = val;
                break;
            case C1_R18FE0FF7B7: //纬度 秒1
                val = (int)val;
                m_second1_wei = val;
                break;
            case C1_R18FE0FF7B8: //纬度 秒2
                val = (int)val;
                m_second2_wei = val;
                break;

            }

            break;
        }
    }
}
void FORM_GPSQuery::GPSstateDisplay(){
    //经纬度
    QString strLongitudeAndLatitude;
    QString strLongitude;
    QString strLatitude;
    strLongitude = m_EW+QString("%1").arg(m_degree_jing)+"°"+QString("%1").arg(QString::number(m_min_jing),2,'0')+"."+QString("%1").arg(QString::number(m_second1_jing),2,'0')+QString("%1").arg(QString::number(m_second2_jing),2,'0')+"'";
    strLatitude = m_SN+QString("%1").arg(m_degree_wei)+"°"+QString("%1").arg(QString::number(m_min_wei),2,'0')+"."+QString("%1").arg(QString::number(m_second1_wei),2,'0')+QString("%1").arg(QString::number(m_second2_wei),2,'0')+"'";
    strLongitudeAndLatitude = strLongitude + "  " + strLatitude;

    if(m_bEnabled == 1)
        ui->value_1->setText(strLongitudeAndLatitude);
    else
        ui->value_1->setText("E0°.0' W0°.0'");

    //海拔
    ui->value_2->setText(m_Altitude);

    //卫星数
    ui->menu_13->setText(QString("%1").arg(m_satelliteNum));

    //电源
    if(m_powerState == 0x0C){
        ui->value_4->setText("正常");
        ui->value_4->setStyleSheet("color: rgb(0, 255, 0);");
    }
    if(m_powerState == 0x0D){
        ui->value_4->setText("异常");
        ui->value_4->setStyleSheet("color: rgb(255,0,0);");
    }

    //天线
    if(m_lineState == 0x0E){
        ui->value_5->setText("正常");
        ui->value_5->setStyleSheet("color: rgb(0, 255, 0);");
    }
    if(m_lineState == 0x0F){
        ui->value_5->setText("异常");
        ui->value_5->setStyleSheet("color:red");
    }


    //SIM
    if(m_SIMState == 0x10){
        ui->value_6->setText("正常");
        ui->value_6->setStyleSheet("color: rgb(0, 255, 0);");
    }
    if(m_SIMState == 0x11){
        ui->value_6->setText("异常");
        ui->value_6->setStyleSheet("color: rgb(255,0,0);");
    }

}
/********************************************************************/
void FORM_GPSQuery::onRelease_Button1()
{

    MainLmiNew->showForm();
    closeForm();
}
void FORM_GPSQuery::onRelease_Button2()
{

}
void FORM_GPSQuery::onRelease_Button3()
{

}
void FORM_GPSQuery::onRelease_Button4()
{

}
void FORM_GPSQuery::onRelease_Button5()
{
    if(InfoQueryMenu == 0)
        InfoQueryMenu = new FORM_InfoQueryMenu;
    InfoQueryMenu->showForm();
    closeForm();
}

void FORM_GPSQuery:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}

QString FORM_GPSQuery::creatHtmlText(qreal val,char* pUint,int num){
    QString strHtml;
    strHtml = QString("<html> \
             <head> \
             <style> \
             font{color:white;} #f{color:green;} \
             </style> \
             </head> \
             <body> \
             <font>%1</font><font id=\"f\">%2</font> \
             </body> \
             </html>").arg(QString::number(val,'f',num)).arg(pUint);
   return strHtml;
}

void FORM_GPSQuery::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}
