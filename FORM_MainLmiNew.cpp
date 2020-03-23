#include "FORM_MainLmiNew.h"
#include "ui_FORM_MainLmiNew.h"
#include "main.h"
#include "page.h"


qreal finger1actangle=0.0;
qreal finger2actangle=0.0;
qreal finger3actangle=0.0;
qreal finger4actangle=0.0;
static const qreal finger1length = 311.0;
static const qreal finger2length = 380.0;
static const qreal finger3length = 290.0;
static const qreal finger4length = 380.0;
static const qreal meterDriveSpeedMin = 0;
static const qreal meterDriveSpeedMax = 80;
static const qreal meterEngineSpeedMin = 0;
static const qreal meterEngineSpeedMax = 3200;
/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] =
{
    { C1_R04FA0001B1,   0x000000ff,     1,       0   },
    { C1_R04FA0001B2,   0x000000ff,     1,       0   },
    { C1_R04FA0001B3,   0x000000ff,     1,       0   },
    { C1_R04FA0001B4,   0x000000ff,     1,       0   },
    { C1_R04FA0001B56,  0x0000ffff,     1,       0   },
    { C1_R04FA0001B78,  0x0000ffff,     1,       0   },
    { C1_R04FA0003B1234,0xffffffff,     1,       0   },
    { C1_R04FA0003B5678,0xffffffff,     1,       0   },
    { C1_R04FA0004B5,   0x000000ff,     1,       0   },
    { C1_R18ff0807B6,   0x000000ff,     1,       0   },
    { C1_R18FF0407B12,  0x0000ffff,     0.1,     0   },
    { C1_R04FA0002B78,  0x0000ffff,     1,       0   },
    { C1_R0CF00400B45,  0x0000ffff,     1,       0   },
    { C1_R18FEE500B1234,0xffffffff,     1,       0   },
    { C1_R04FA0008B2,   0x000000ff,     1,       0   },
    { C1_R04FA0008B3,   0x000000ff,     1,       0   },
    { C1_R04FA0008B1,   0x000000ff,     1,       0   },
    { C1_R04FA0010B2,   0x000000ff,     1,       0   },
    { C1_R04FA0008B7,   0x000000ff,     1,       0   },
    { C1_R04FA0007B1,   0x000000ff,     1,       0   },
    { C1_R04FA0007B2,   0x000000ff,     1,       0   },
    { C1_R04FA0007B5,   0x000000ff,     1,       0   },
    { C1_R18F00503B4,   0x000000ff,     1,       -125},
    { C1_R04FA0014B78,  0x0000ffff,     1,       0   },
    { C1_R500B1,        0x000000ff,     1,       0   },
    { C1_R04FA0009B1,   0x000000ff,     1,       0   },
    { C1_R18FE01F6B1,   0x000000ff,     1,       0   },
    { C1_R18ff0707B78,  0x0000ffff,     0.1,     0   },
    { C1_R04FB0004B3,   0x000000ff,     1,       0   },
    { C1_R18FECA00B18,  0xffffffff,     1,       0   },
    { C1_R18ECFF00B18,  0xffffffff,     1,       0   },
    { C1_R18EBFF00B18,  0xffffffff,     1,       0   },
    { C1_R18FF0000B1,   0x000000ff,     1,       0   },
    { C1_R18FF0000B3,   0x000000ff,     1,       0   },
    { C1_R18FF0000B5,   0x000000ff,     1,       0   },

};
int lanindex=0;

FORM_MainLmiNew::FORM_MainLmiNew(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_MainLmiNew),
    m_finger1(0),
    m_finger2(0),
    m_finger3(0),
    m_finger4(0),
    m_timerLight(0),
    m_fickerLabel_Hanging(0),
    m_uiInited(false)
{
    ui->setupUi(this);

    m_urgent = false;
    m_fickerLabel_turnLeft=0;
    m_fickerLabel_turnRight=0;
    m_fickerLabel_urgent=0;
    m_fickerLabel_positionLight=0;

    m_fickerLabel_Hanging = new FUNC_FickerLabel(ui->ico_9,this);
    m_fickerLabel_turnLeft = new FUNC_FickerLabel(ui->ico_19,this);
    m_fickerLabel_turnRight = new FUNC_FickerLabel(ui->ico_14,this);
    m_fickerLabel_urgent = new FUNC_FickerLabel(ui->urgent_red,this);
    m_fickerLabel_positionLight = new FUNC_FickerLabel(ui->ico_16,this);

    initScene();
//
    //隐藏标签
//    ui->temp_outdoor_3->setVisible(false);
//    ui->label_3->setVisible(false);
    ui->ico_11->setVisible(false);
    ui->ico_13->setVisible(false);
//    ui->label_2->setVisible(false);
//    ui->temp_outdoor_4->setVisible(false);
}



FORM_MainLmiNew::~FORM_MainLmiNew()
{
    delete ui;
}

void FORM_MainLmiNew::setCurrentWarningList(){
    currentWaringList.clear();
    QVariantList varList = Func_ErrorCode->getCurrentErrorInfo();
    foreach(QVariant var,varList){
        QStringList list = var.toStringList();
        currentWaringList<<list[0];
    }
}

/********************************************************************/
void FORM_MainLmiNew::showForm()
{
    ui->retranslateUi(this);
    showFullScreen();
    canActive(CAN1_DEV, varList, N(varList));
    dateTimeDisplay();
#if 0
    QDateTime curDateTime = QDateTime::currentDateTime();
    int year = curDateTime.toString("yyyy").toInt();
    int month = curDateTime.toString("MM").toInt();
    int day = curDateTime.toString("dd").toInt();
    int hour = curDateTime.toString("hh").toInt();
    int minute = curDateTime.toString("mm").toInt();
    int second = curDateTime.toString("ss").toInt();

    /*设置时间*/
    ui->dateTimeEdit->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd"));
    ui->dateTimeEdit_2->setText(QDateTime::currentDateTime().toString("HH:mm"));
    m_finger1->setFingerAngle(291);
    m_finger2->setFingerAngle(320);
#endif

    if (m_timerLight) delete m_timerLight;
    m_timerLight = new QTimer(this);
    m_timerLight->stop();
    connect(m_timerLight, SIGNAL(timeout()), this, SLOT(dateTimeDisplay()));
    m_timerLight->start(1000);

    return ;
}

void FORM_MainLmiNew::closeForm()
{
    this->hide();//关闭界面执行析够函数->释放资源
    m_timerLight->stop();
    canDeactive();
    return ;
}
/********************************************************************/
void FORM_MainLmiNew::dateTimeDisplay()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    int year = curDateTime.toString("yyyy").toInt();
    int month = curDateTime.toString("MM").toInt();
    int day = curDateTime.toString("dd").toInt();
    int hour = curDateTime.toString("hh").toInt();
    int minute = curDateTime.toString("mm").toInt();
    int second = curDateTime.toString("ss").toInt();

    /*设置时间*/
    ui->dateTimeEdit->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd"));
    ui->dateTimeEdit_2->setText(QDateTime::currentDateTime().toString("HH:mm"));

    /*发送版本信息给控制器*/
    canopen_write_var_int(CAN1_DEV,C1_T04FB0012B12,100);
    canSendPdo(CAN1_DEV,1);
    /*发送时间信息给控制器*/
    canopen_write_var_int(CAN1_DEV,C1_T281B12,year);
    canSendPdo(CAN1_DEV,2);
    canopen_write_var_int(CAN1_DEV,C1_T281B3,month);
    canSendPdo(CAN1_DEV,2);
    canopen_write_var_int(CAN1_DEV,C1_T281B4,day);
    canSendPdo(CAN1_DEV,2);
    canopen_write_var_int(CAN1_DEV,C1_T281B5,hour);
    canSendPdo(CAN1_DEV,2);
    canopen_write_var_int(CAN1_DEV,C1_T281B6,minute);
    canSendPdo(CAN1_DEV,2);
    canopen_write_var_int(CAN1_DEV,C1_T281B7,second);
    canSendPdo(CAN1_DEV,2);

    /*设置当前报警闪烁标签*/
    static int a = 0;
    setCurrentWarningList();
    int num = currentWaringList.size();
    if(num){
        ui->label_warning->setVisible(true);
        int i = a%num;
        ui->label_warning->setText(currentWaringList[i]);
    }else{
        ui->label_warning->setVisible(false);
    }
    a++;
    if(a>100000)
        a=0;

    /*设置报警信息图标*/
    //GE制动持续报警
    if(RetardContinousAlarm())
        ui->ico_22->setVisible(true);
    else
        ui->ico_22->setVisible(false);
    //GE无推进力报警
    if(NoPropulsionAlarm())
        ui->ico_23->setVisible(true);
    else
        ui->ico_23->setVisible(false);
    //GE系统故障报警
    if(SystemFaultAlarm())
        ui->ico_24->setVisible(true);
    else
        ui->ico_24->setVisible(false);
    //GE无制动力报警
    if(NORetardAlarm())
        ui->ico_25->setVisible(true);
    else
        ui->ico_25->setVisible(false);

    //燃油位低报警
    if(OilLevelAlarm())
        ui->ico_5->setPixmap(QPixmap(":/alarm/oil.png"));
    else
        ui->ico_5->setPixmap(QPixmap(":/pic_de260/ico_01.png"));

    //车门未关闭报警
    if(DoorAlarm())
        ui->ico_27->setVisible(true);
    else
        ui->ico_27->setVisible(false);

    //安全带报警
    if(SeatBeltAlarm())
        ui->ico_26->setVisible(true);
    else
        ui->ico_26->setVisible(false);

    //回油过滤器堵塞报警
    if(ReturnFilterCloggingAlarm())
        ui->ico_28->setVisible(true);
    else
        ui->ico_28->setVisible(false);

    //高压过滤器堵塞报警
    if(HighPressureFilterCloggingAlarm())
        ui->ico_29->setVisible(true);
    else
        ui->ico_29->setVisible(false);

    //空滤堵塞报警
    if(AirFilterBlockAlarm())
        ui->ico_30->setVisible(true);
    else
        ui->ico_30->setVisible(false);

    //转向压力低报警
    if(SteeringPressureLowAlarm())
        ui->ico_31->setVisible(true);
    else
        ui->ico_31->setVisible(false);
}
/********************************************************************/
/**
 * 按键名称初始化
 */
void FORM_MainLmiNew::loadFunctionKey(void)
{
    const QPixmap picKey1(":/pic_de260/27.png");
    const QPixmap picKey2(":/pic_de260/28.png");
    const QPixmap picKey3(":/pic_de260/29.png");
    const QPixmap picKey4(":/pic_de260/30.png");
    const QPixmap picKey5(":/pic_de260/31.png");
    delete buttonBar;
    buttonBar = new FUN_ButtonBar(picKey1,picKey2,picKey3,picKey4,picKey5,this);

    return ;
}
/********************************************************************/
int FORM_MainLmiNew::initScene()
{
    QPixmap m_needlePix1(":/pic_de260/figner_04.png");
    QPixmap m_needlePix2(":/pic_de260/figner_02.png");
    QPixmap m_needlePix3(":/pic_de260/figner_01.png");
    QPixmap m_needlePix4(":/pic_de260/figner_04.png");

    int originFinger1X = -16.1;
    int originFinger1Y = 605.4;
    int originFinger2X = 273;
    int originFinger2Y = 649;
    int originFinger3X = 963;
    int originFinger3Y = 350;
    int originFinger4X = 1645;
    int originFinger4Y = 649;

    if (m_finger1)
        delete  m_finger1;
    if (m_finger2)
        delete  m_finger2;
    if (m_finger3)
        delete  m_finger3;
    if (m_finger4)
        delete  m_finger4;

    /*机油压力指针*/
    m_finger1 = new FUN_MeterFinger("",m_needlePix1, originFinger1X, originFinger1Y, finger1length, this);
    m_finger1->raise();
    /*冷却液温度指针*/
    m_finger2 = new FUN_MeterFinger("",m_needlePix2, originFinger2X, originFinger2Y, finger2length, this);
    m_finger2->raise();
    /*发动机转速指针*/
    m_finger3 = new FUN_MeterFinger("",m_needlePix3, originFinger3X, originFinger3Y, finger3length, this);
    m_finger3->lower();
    /*机油温度指针*/
    m_finger4 = new FUN_MeterFinger("",m_needlePix4, originFinger4X, originFinger4Y, finger4length, this);
    m_finger4->raise();

    return 0;
}
/********************************************************************/
/*用于设置机油压力仪表*/
qreal FORM_MainLmiNew::setOilPressure(int val)
{
    qreal angle=0.0;

    if(val>1000)
            val=1000;
    angle = valueToAngle(291, 43, 0, 1000, 1000 - val);
    m_finger1->setFingerAngle(angle);
    return 0;
}
/*用于设置冷却液温度*/
qreal FORM_MainLmiNew::setWaterTemperature(int val)
{
    qreal angle=0.0;

    if(val>75)
        val=75;
    angle = valueToAngle(288, 42.5, 0, 75, 75-val);

    m_finger2->setFingerAngle(angle);
    return 0;
}
/*用于设置车速*/
qreal FORM_MainLmiNew::setEngineSpeed(int val)
{
    qreal angle=0.0;

    if(val>80)
        val=80;
    angle = valueToAngle(134, 241, 0, 80, val);

    m_finger3->setFingerAngle(angle);

//    cpu = sys_cpu_usage();
//    mem = sys_mem_usage();
//    qDebug() << "cpu=" << cpu << "mem=" << mem;
    return 0;
}
/*用于设置发动机转速*/
qreal FORM_MainLmiNew::setOilTemperature(int val)
{
    qreal angle=0.0;

//    finger4actangle++;
//    if(finger4actangle > 300) finger4actangle = 0.0;
//    else if(finger4actangle < 0) finger4actangle = 0.0;
    if(val>3000)
        val=3000;
    angle = valueToAngle(210, 40, 0, 3000, val);
//    qDebug()<<"angle:"<<angle;

    m_finger4->setFingerAngle(angle);
    return 0;
}
/*
 * 用于指针的数值转换为角度
*/
qreal FORM_MainLmiNew::valueToAngle(qreal minangle, qreal anglearea, qreal minvalue, qreal maxvalue, qreal value)
{
    if(maxvalue <= minvalue) return -1;
    qreal a = anglearea * (value - minvalue) / (maxvalue - minvalue) + minangle;

    while (a > 360)
        a -= 360;
    while (a < 0)
        a += 360;

    return a;
}
/********************************************************************/
void FORM_MainLmiNew::onRelease_Button1()
{
    if(Menu == 0)
        Menu = new FORM_Menu;
    Menu->showForm();
    closeForm();
//    if(Func_ErrorCode == 0)
//        Func_ErrorCode = new FUNC_ErrorCode();
}
void FORM_MainLmiNew::onRelease_Button2()
{

}
void FORM_MainLmiNew::onRelease_Button3()
{

}
void FORM_MainLmiNew::onRelease_Button4()
{

}
void FORM_MainLmiNew::onRelease_Button5()
{

}
/********************************************************************/
void FORM_MainLmiNew::lan_change()
{
    lanindex = (lanindex+1)%2;
    //lanindex = 0;
    QTranslator translator;
    if(lanindex==0)
        translator.load(QString(":/language/llang_English.qm"));
    else
        translator.load(QString(":/language/llang_Chinese.qm"));
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    //qDebug() <<"lanindex="<<lanindex<<"1"<<translator.load(QString(":/language/llang_Chinese.qm"));
}
/********************************************************************/
void FORM_MainLmiNew::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    long long value;

    const QPixmap sNormal(":/alarm/HydraulicsPressure_gray.png");
    const QPixmap sAlarm(":/alarm/HydraulicsPressure_red.png");
    const QPixmap sNormal2(":/alarm/WarterTemperature_gray.png");
    const QPixmap sAlarm2(":/alarm/WarterTemperature_red.png");
    const QPixmap sNormal3(":/pic_de260/ico_01.png");
    const QPixmap sAlarm3(":/pic_de260/ico_15.png");

    if (code != MSG_CAN_OD)
        return;

//    qDebug() << "test008" << m->var;
    for (unsigned int i=0; i<N(varList); i++)
    {
        if (varList[i].var == m->var)
        {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

            switch (m->var)
            {
            case C1_R04FA0001B1://液压油温
                break;
            case C1_R04FA0001B2://燃油油位
                ui->label_9->setText(QString::number(val));
                break;
            case C1_R04FA0001B3://发动机水温
                ui->label_6->setText(QString::number(val-40));
                if(val>160) val = 160;
                if(val<85) val=85;
                setWaterTemperature(val-40-45);
                break;
            case C1_R18FF0000B1:
                //机油压力低报警
                if(unPackValue(val,4)==1 && unPackValue(val,5)==0){
                    ui->ico_20->setVisible(true);
                    ui->ico_20->setPixmap(QPixmap(":/alarm/hydraulicsPressure.png"));
                }
                else if(unPackValue(val,6)==1 && unPackValue(val,7)==0){
                    ui->ico_20->setVisible(true);
                    ui->ico_20->setPixmap(QPixmap(":/alarm/HydraulicsPressure_red.png"));
                }
                else
                    ui->ico_20->setVisible(false);
                break;
            case C1_R18FF0000B3:
                //冷却液位低报警
                if(unPackValue(val,0)==1 && unPackValue(val,1)==0){
                    ui->ico_coolantLevel->setVisible(true);
                }
                else
                    ui->ico_coolantLevel->setVisible(false);
                break;
            case C1_R18FF0000B5:
                //发动机水温过高
                if(unPackValue(val,2)==1 && unPackValue(val,3)==0){
                    ui->ico_21->setVisible(true);
                    ui->ico_21->setPixmap(QPixmap(":/alarm/waterTemperature.png"));
                }
                else if(unPackValue(val,4)==1 && unPackValue(val,5)==0){
                    ui->ico_21->setVisible(true);
                    ui->ico_21->setPixmap(QPixmap(":/alarm/WarterTemperature_red.png"));
                }
                else
                    ui->ico_21->setVisible(false);
                break;

            case C1_R04FA0001B56://机油压力
                ui->label_5->setText(QString::number(val));
                setOilPressure(val);
                break;
            case C1_R04FA0001B78://系统电压
                ui->label_10->setText(QString("%1").arg(val/100,0,'f',1)+"V");
                break;
            case C1_R18FF0407B12://车速
                val = val*1.609;
                setEngineSpeed(int(val));
                break;
            case C1_R0CF00400B45://发动机转速
                ui->label_4->setText(QString::number(int(val*0.125))); //标签
                setOilTemperature(int(val*0.125)); //指针
                break;
            case C1_R04FA0002B78://实时载重量
                    ui->label_2->setText(QString::number(val));
                break;
            case C1_R04FA0004B5: //载重百分比
                ui->label_3->setText(QString::number(val));
                break;
            case C1_R04FA0003B1234://发动机小时
                ui->label_8->setText(QString::number(val*0.1,'f',1));
                break;
            case C1_R04FA0003B5678://运行总里程
                ui->label_7->setText(QString::number(val*0.1,'f',1));
                break;
            case C1_R18ff0807B6://环境温度
                ui->label_11->setText(QString::number(val)+"℃");
                break;
            case C1_R04FA0008B2:
                val = int(val);
                if(unPackValue(val,2)==1){ //驻车灯
                    ui->ico_1->setVisible(true);
                }
                else{
                    ui->ico_1->setVisible(false);
                }
                //装载制动
                if(unPackValue(val,6)==1)
                    ui->ico_6->setVisible(true);
                else
                    ui->ico_6->setVisible(false);
                //行车制动
                if(unPackValue(val,4)==1)
                    ui->ico_8->setVisible(true);
                else
                    ui->ico_8->setVisible(false);
                break;
            case C1_R04FA0008B3:
                val = int(val);
                //电制动
                if(unPackValue(val,0)==1)
                    ui->ico_10->setVisible(true);
                else
                    ui->ico_10->setVisible(false);
                break;
            case C1_R04FA0008B7:
                val = int(val);
                if(unPackValue(val,3)==1) //举升互锁信号
                    ui->ico_13->setVisible(true);
                else
                    ui->ico_13->setVisible(false);
                break;
            case C1_R18FE01F6B1:
                if(unPackValue(val,7)==1){ //紧急信号灯
                    m_urgent = true;
                    //                    ui->urgent_red->setVisible(true);
                    m_fickerLabel_urgent->setFicker(400,400);
                    m_fickerLabel_turnLeft->setFicker(400,400);
                    m_fickerLabel_turnRight->setFicker(400,400);
//                    m_fickerLabel_positionLight->setFicker(1000,500);
                }
                else{
                    m_urgent = false;
                    m_fickerLabel_urgent->resetFicker();
                    m_fickerLabel_turnLeft->resetFicker();
                    m_fickerLabel_turnRight->resetFicker();
//                    m_fickerLabel_positionLight->resetFicker();
                    /*没有开启紧急信号灯时，才处理示廓灯信号*/
                    if(unPackValue(val,3)==1){ //示廓灯
                        ui->ico_16->setVisible(true);
//                        ui->ico_19->setVisible(false);
//                        ui->ico_14->setVisible(false);
                    }
                    else{
                        ui->ico_16->setVisible(false);
//                        ui->ico_19->setVisible(false);
//                        ui->ico_14->setVisible(false);
                    }
                }
                break;
            case C1_R04FA0008B1:
                val = int(val);
                if(unPackValue(val,5)==1) //远光灯
                    ui->ico_17->setVisible(true);
                else
                    ui->ico_17->setVisible(false);
                if(unPackValue(val,7)==1) //雾灯
                    ui->ico_15->setVisible(true);
                else
                    ui->ico_15->setVisible(false);
                if(unPackValue(val,4)==1){//近光灯
                    ui->ico_18->setVisible(true);
                    ui->ico_16->setVisible(true);
                }
                else{
                    ui->ico_18->setVisible(false);
                    ui->ico_16->setVisible(false);
                }
                if(unPackValue(val,0)==1) //左转灯闪烁
                    m_fickerLabel_turnLeft->setFicker(400,400);
                else
                    m_fickerLabel_turnLeft->resetFicker();
                if(unPackValue(val,1)==1) //右转灯闪烁
                    m_fickerLabel_turnRight->setFicker(400,400);
                else
                    m_fickerLabel_turnRight->resetFicker();

                break;
            case C1_R04FA0007B1:
                    val = int(val);
                    //液压油温高报警
                    if(unPackValue(val,5)==1)
                        ui->ico_9->setVisible(true);
                    else
                       ui->ico_9->setVisible(false);
                    //燃油油位低
                    if(unPackValue(val,6)==1) ui->ico_5->setPixmap(sAlarm3);
                    else ui->ico_5->setPixmap(sNormal3);
                break;
           case C1_R04FA0007B2:
                    val = int(val);
                    if(unPackValue(val,4)==1) //系统电压低报警
                        ui->ico_4->setVisible(true);
                    else
                       ui->ico_4->setVisible(false);
                break;
            case C1_R04FA0009B1:
                if(unPackValue(val,0)==1){//驻车档信号
                    ui->grade_2->setStyleSheet("color: red;"
                                               "font: 39pt \"Ubuntu\";");
                    ui->grade_1->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_3->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_4->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                }else if(unPackValue(val,1)==1){//空档信号
                    ui->grade_3->setStyleSheet("color: red;"
                                               "font: 39pt \"Ubuntu\";");
                    ui->grade_1->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_2->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_4->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                }else if(unPackValue(val,2)==1){//倒档信号
                    ui->grade_1->setStyleSheet("color: red;"
                                               "font: 39pt \"Ubuntu\";");
                    ui->grade_3->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_2->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_4->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                }else if(unPackValue(val,3)==1){//前进档信号
                    ui->grade_4->setStyleSheet("color: red;"
                                               "font: 39pt \"Ubuntu\";");
                    ui->grade_3->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_2->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_1->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                }else{
                    ui->grade_4->setStyleSheet("color: white;"
                                               "font: 39pt \"Ubuntu\";");
                    ui->grade_3->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_2->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                    ui->grade_1->setStyleSheet("color: white;"
                                           "font: 39pt \"Ubuntu\";");
                }
                break;
            case C1_R18ff0707B78: //母线电压
                ui->label_13->setText(QString("%1 %2").arg(val,0,'f',1).arg("V"));
                break;

            case C1_R04FA0014B78: //时时载重量
                val = (int)val;
                ui->label_2->setText(QString("%1").arg(val));
                break;
            case C1_R04FB0004B3://
                val = uint16_t(val);
//                qDebug()<<"C1_R04FB0004B3:"<<val;
                /*0x10 -- 回落到位,0x00 -- 举升到位,0x20 -- 货箱悬空*/
                if(unPackValue(val,4)==0&&unPackValue(val,5)==0){//回落到位
                    m_fickerLabel_Hanging->resetFicker();
                    ui->ico_32->setPixmap(QPixmap(":/alarm/_6.png"));
                    ui->ico_9->setPixmap(QPixmap(":/alarm/_7.png"));
                    ui->ico_7->setPixmap(QPixmap(":/alarm/_1.png"));
                }else if(unPackValue(val,4)==1&&unPackValue(val,5)==1){//举升到位
                    m_fickerLabel_Hanging->resetFicker();
                    ui->ico_7->setPixmap(QPixmap(":/alarm/_2.png"));
                    ui->ico_9->setPixmap(QPixmap(":/alarm/_7.png"));
                    ui->ico_32->setPixmap(QPixmap(":/alarm/_4.png"));
                }else{ //货箱悬空
                    ui->ico_9->setPixmap(QPixmap(":/alarm/_8.png"));
                    m_fickerLabel_Hanging->setFicker(500,500);
                    ui->ico_7->setPixmap(QPixmap(":/alarm/_1.png"));
                    ui->ico_32->setPixmap(QPixmap(":/alarm/_4.png"));
                }
                break;
                /*发动机红黄白灯*/
            case C1_R18FECA00B18:
//                qDebug()<<"C1_R18FECA00B18";
                value = canopen_read_var_uint64(CAN1_DEV, C1_R18FECA00B18, NULL);
                changeLongLongToChar8(value,ch_feca);
                if(unPackValue(ch_feca[0],0)==1 && unPackValue(ch_feca[0],1)==0){
                    ui->engine_white->setVisible(true);
                }
                else{
                    ui->engine_white->setVisible(false);
                }
                //yellow
                if(unPackValue(ch_feca[0],2)==1 && unPackValue(ch_feca[0],3)==0){
                    ui->engine_yellow->setVisible(true);
                }
                else{
                    ui->engine_yellow->setVisible(false);
                }
                //red
                if(unPackValue(ch_feca[0],4)==1 && unPackValue(ch_feca[0],5)==0){
                    ui->engine_red->setVisible(true);
                }
                else{
                    ui->engine_red->setVisible(false);
                }
                break;
            case C1_R18EBFF00B18:
//                qDebug()<<"C1_R18EBFF00B18";
                value = canopen_read_var_uint64(CAN1_DEV, C1_R18EBFF00B18, NULL);
                changeLongLongToChar8(value,data_EB);
                if(data_EB[0]==1){
                    //white
                    if(unPackValue(data_EB[1],0)==1 && unPackValue(data_EB[1],1)==0){
                        ui->engine_white->setVisible(true);
                    }
                    else{
                        ui->engine_white->setVisible(false);
                    }
                    //yellow
                    if(unPackValue(data_EB[1],2)==1 && unPackValue(data_EB[1],3)==0){
                        ui->engine_yellow->setVisible(true);
                    }
                    else{
                        ui->engine_yellow->setVisible(false);
                    }
                    //red
                    if(unPackValue(data_EB[1],4)==1 && unPackValue(data_EB[1],5)==0){
                        ui->engine_red->setVisible(true);
                    }
                    else{
                        ui->engine_red->setVisible(false);
                    }
                }
                break;
            }
            break;
        }
    }
}

void FORM_MainLmiNew:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/background.png"));

}

void FORM_MainLmiNew::showEvent(QShowEvent*)
{

}

bool FORM_MainLmiNew::RetardContinousAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==254){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::NoPropulsionAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==284){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::SystemFaultAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==253){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::NORetardAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==283){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::OilLevelAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==107){
            ret = 1;
            break;
        }
    }
    return ret;
}
bool FORM_MainLmiNew::DoorAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==201){
            ret = 1;
            break;
        }
    }
    return ret;
}
bool FORM_MainLmiNew::SeatBeltAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==202){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::ReturnFilterCloggingAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==304 || *(p+i)==305 || *(p+i)==306 ){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::HighPressureFilterCloggingAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==307 || *(p+i)==308 || *(p+i)==309){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::AirFilterBlockAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==101 || *(p+i)==102 || *(p+i)==103 || *(p+i)==104 || *(p+i)==105 || *(p+i)==106){
            ret = 1;
            break;
        }
    }
    return ret;
}

bool FORM_MainLmiNew::SteeringPressureLowAlarm(){
    bool ret = 0;
    unsigned short count;
    unsigned short*p = Func_ErrorCode->API_GetCurrentError(count);
    for(int i=0;i<count;i++){
        if(*(p+i)==367){
            ret = 1;
            break;
        }
    }
    return ret;
}

void FORM_MainLmiNew::changeLongLongToChar8(long long var,unsigned char data[8]){
    memset(data,0,8);
    memcpy(data,&var,sizeof(var));
#if 0
    qDebug()<<QString::number(data[0],16)+" "
            <<QString::number(data[1],16)+" "
            <<QString::number(data[2],16)+" "
            <<QString::number(data[3],16)+" "
            <<QString::number(data[4],16)+" "
            <<QString::number(data[5],16)+" "
            <<QString::number(data[6],16)+" "
            <<QString::number(data[7],16)+" ";
#endif
}

