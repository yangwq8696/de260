#include "wika_GeneralWidget.h"
#include "ui_wika_GeneralWidget.h"
#include "FUN_MeterFinger.h"
#include "wika_Label.h"
#include <QDateTime>
#include <QTimer>


/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] =
{
    { C1_R18FF0407B12,   0x0000ffff,     0.1,       0   },
    { C1_R0CF00400B45,   0x0000ffff,     1,       0   },
    { C1_R04FA0003B5678,   0xffffffff,     1,       0   }

};


wika_GeneralWidget::wika_GeneralWidget(Page *parent) :
    Page(parent),
    ui(new Ui::wika_GeneralWidget)
{
    ui->setupUi(this);
    setGeometry(0,0,450,720);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(setTimeDate()));
    m_timer->setInterval(10000);
    m_timer->start();




    //单位字体
    createFinger();
    QFont font;
    font.setFamily("TakaoPGothic");
    font.setPointSize(15);
    font.setItalic(true);

    //车速
    m_labelSpeed = new wika_Label(100,60,this);
    m_labelSpeed->setText("120");
    m_labelSpeed->setAngel(-20);
    m_labelSpeed->move(3,411);

    m_labelSpeed->setFontSize(20);
    wika_Label* speedUnit = new wika_Label(100,60,this);
    speedUnit->setText("km/h");
    speedUnit->setAngel(-20);
    speedUnit->move(67,401);
    speedUnit->setFont(font);
    speedUnit->setColor(Qt::gray);


    //转速
    m_labelRad = new wika_Label(100,60,this);
    m_labelRad->setText("11800");
    m_labelRad->setAngel(-20);
    m_labelRad->move(220,320);
    m_labelRad->setFontSize(20);
    wika_Label* radUnit = new wika_Label(100,60,this);
    radUnit->setText("r/min");
    radUnit->setAngel(-20);
    radUnit->move(230,340);
    radUnit->setFont(font);
    radUnit->setColor(Qt::gray);


    QString strCurrentTime = QDateTime::currentDateTime().toString("hh:mm");
    QString strCurrentDate = QDateTime::currentDateTime().toString("yyyy/MM/dd A");
    //时间
    m_labelTime = new wika_Label(100,65,this);
    m_labelTime->setText(strCurrentTime);
    m_labelTime->setAngel(-30);
    m_labelTime->move(5,560);
    m_labelTime->setFontSize(22);

    //日期
    m_labelDate = new wika_Label(120,70,this);
    m_labelDate->setText(strCurrentDate);
    m_labelDate->setAngel(-30);
    m_labelDate->move(5,590);
    m_labelDate->setFontSize(10);

    //里程
    m_mileage = new wika_Label(160,60,this);
    m_mileage->setText("965368");
    m_mileage->setAngel(7);
    m_mileage->move(9,105);
    m_mileage->setFontSize(18);
    wika_Label* mileageUnit = new wika_Label(80,65,this);
    mileageUnit->setText("ODO km");
    mileageUnit->setAngel(5);
    mileageUnit->move(63,130);
    font.setItalic(false);
    font.setPointSize(8);
    mileageUnit->setFont(font);
    mileageUnit->setColor(Qt::gray);

    canActive(CAN1_DEV, varList, N(varList));
}

wika_GeneralWidget::~wika_GeneralWidget()
{
    delete ui;
}
void wika_GeneralWidget::setTimeDate(){
    QString strCurrentTime = QDateTime::currentDateTime().toString("hh:mm");
    QString strCurrentDate = QDateTime::currentDateTime().toString("yyyy/MM/dd A");

    m_labelTime->setText(strCurrentTime);
    m_labelDate->setText(strCurrentDate);
}

void wika_GeneralWidget::createFinger(){
    m_finger1 = new FUN_MeterFinger("",
                                    QPixmap(":/pic_de260/figner_05.png"),
                                    -54.8,
                                    734.3,
                                    428,
                                    this);
    m_finger1->setFingerAngle(-50.6);  //-50.6 —— -69     0--80
    m_finger1->raise();


    m_finger2 = new FUN_MeterFinger("",
                                    QPixmap(":/pic_de260/figner_06.png"),
                                    180.5,
                                    484.6,
                                    264,
                                    this);
    m_finger2->setFingerAngle(-32.3); //-32.3 —— -59.7   0--3

}

void wika_GeneralWidget::setSpeed(QString strSpeed){
    m_labelSpeed->setText(strSpeed);
}
void wika_GeneralWidget::setRad(QString strRad){
    m_labelRad->setText(strRad);

}

void wika_GeneralWidget::setMileage(QString strMileage){
    m_mileage->setText(strMileage);
}

void wika_GeneralWidget::setTime(QString strTime){
    m_labelTime->setText(strTime);
}
void wika_GeneralWidget::setDate(QString strDate){
    m_labelDate->setText(strDate);
}

void wika_GeneralWidget::setAngle1(qreal angle){
    m_finger1->setFingerAngle(angle);
}
void wika_GeneralWidget::setAngle2(qreal angle){
    m_finger2->setFingerAngle(angle);
}

qreal valueToAngle(qreal minangle, qreal anglearea, qreal minvalue, qreal maxvalue, qreal value)
{
    if(maxvalue <= minvalue) return -1;
    if(value>maxvalue)
        value = maxvalue;
    qreal a = minangle + anglearea*(maxvalue - value  )/( maxvalue- minvalue) ;

    return a;
}
void wika_GeneralWidget::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    QString strSpeed;
    qreal angle;

    if (code != MSG_CAN_OD)
        return;
    for (unsigned int i=0; i<N(varList); i++)
    {
        if (varList[i].var == m->var)
        {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

            switch (m->var)
            {
                case C1_R18FF0407B12: //车速
                val = val*1.609;
                strSpeed = QString("%1").arg(val,0,'f',2);
                m_labelSpeed->setText(strSpeed);
                angle = valueToAngle(-69, 18.4, 0, 80, val);
                m_finger1->setFingerAngle(angle);
                break;

                case C1_R0CF00400B45: //转速
                m_labelRad->setText(QString::number(int(val*0.125)));
                angle = valueToAngle(-59.7, 27.4, 0, 3000, val*0.125);
                m_finger2->setFingerAngle(angle);
                break;

                case C1_R04FA0003B5678: //总里程
                m_mileage->setText(QString::number(val*0.1));
                break;
            }
            break;
        }
    }
}

