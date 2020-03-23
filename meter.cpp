#include "meter.h"
#include "ui_meter.h"
/*
 * 仪表显示参数范围
 * 这里使用const定义
 */
static const qreal meterRpmMin = 0;
static const qreal meterRpmMax = 3000;
static const qreal meterEngineTimeMin = 0;
static const qreal meterEngineTimeMax = 999999999;
static const qreal meterTempMin = 0;
static const qreal meterTempMax = 120;
static const qreal meterMileageMin = 0;
static const qreal meterMileageMax = 999999999;
static const qreal meterSpeedMin = 0;
static const qreal meterSpeedMax = 120;
static const qreal meterOilMin = 0;
static const qreal meterOilMax = 1.0;
static int showflg = 0;
//public int showflg = 0;

/*
 * 定时器参数
 */
static const qreal meterStateIntval = 200;      // 灯状态更新定时器周期
static const qreal meterMenuDockIntval = 10000; // 右侧菜单的显示与隐藏
static const qreal meterMsg2Intval = 20000; // 右侧菜单的显示与隐藏
static const qreal meterLightDuration = 800;    // 灯闪烁周期
/*
 * 仪表界面尺寸
 */
static const qreal meterNeedleLen = 320.0;

/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] = {
    { can1_r_18A_b45,   0x0000ffff,     1,       0   }, //发动机转速
    { can1_r_18A_b1,    0x000000ff,     1,      -40  }, //冷却液温度
    { can1_r_28A_b0,    0x000000ff,     1,      0    }, //车速
    { can1_r_28A_b1,    0x000000ff,     1,      -125 }, //当前档位
    { can1_r_28A_b2,    0x000000ff,     1,      0    }, //燃油
    { can1_r_28A_b3,    0x000000ff,     1,      0    }, //尿素液位
    { can1_r_38A_b4567,    0xffffffff,     1,      0    }, //累计里程
    { can1_r_38A_b0123,    0xffffffff,     1,      0    }, //累计时间
    { can1_r_184_b5,    0x000000ff,     1,      0    },
    { can1_r_28A_b6,    0x000000ff,     1,      0    },
    { can1_r_48A_b0,    0x000000ff,     1,      0    },
    { can1_r_48A_b1,    0x000000ff,     1,      0    },
    { can1_r_48A_b2,    0x000000ff,     1,      0    },
    { can1_r_48A_b3,    0x000000ff,     1,      0    },
    { can1_r_48A_b4,    0x000000ff,     1,      -125    },//目标档位
    { can1_r_48A_b5,    0x000000ff,     1,      -125    },//巡航设定速度
    { can1_r_48A_b6,    0x000000ff,     1,      -125    },//
    { can1_r_48A_b7,    0x000000ff,     1,      -125    },//
    { can1_r_184_b0,    0x000000ff,     1,      0    },
    { can1_r_193_b01,    0x0000ffff,     1,      0    },
    { can1_r_193_b23,    0x0000ffff,     1,      0    },
    { can1_r_193_b45,    0x0000ffff,     1,      0    },
    { can1_r_193_b67,    0x0000ffff,     1,      0    },
    { can1_r_293_b01,    0x0000ffff,     1,      0    },
    { can1_r_293_b23,    0x0000ffff,     1,      0    },
    { can1_r_293_b45,    0x0000ffff,     1,      0    },
    { can1_r_293_b67,    0x0000ffff,     1,      0    },
};

Meter::Meter(Page *parent) :
    Page(PageView, parent),
    ui(new Ui::Meter),
    m_diag1(0),
    m_diag2(0),
    m_valRpm(0),
    m_valEngineTime(0),
    m_valTemp(0),
    m_valSpeed(0),
    m_valMileage(0),
    m_valOil(0),
#ifdef SIMULATION_DEMO
    m_timerDemo(0),
#endif
    m_timerLight(0),
     m_msgflg(0),
    m_msg2Dock(0),
    m_msg3Dock(0),
    m_timerDock(0)
{
    ui->setupUi(this);
    setTopPage(true);

    for (unsigned int i=0; i<N(m_lights); i++) {
        m_lights[i].state = MeterLightOff;
        m_lights[i].duration = 0;
        m_lights[i].blinkOn = false;

    }
	/*
	 * 设置静态（背影贴图）效果
	 */
    initBackground();
	/*
	 * 设置动态（指针）变量
	 */
    initScene();
    initmsgmenu();

    /*
     * 初始化定时器，用于Light
     */
    m_timerLight = new QTimer(this);
    m_timerLight->stop();
    connect(m_timerLight, SIGNAL(timeout()), this, SLOT(updateState()));
    m_timerLight->start(meterStateIntval);


    //用于控制中间图片的初始化状态
    m_msgflg =   new QTimer(this);
    connect(m_msgflg, SIGNAL(timeout()), this, SLOT(initmsg()));
    m_msgflg->start(5000);
    ui->labelCenterR1_BusStatus->hide();
    ui->labelCenterR2_SwerveStatus->hide();
    ui->labelCenterR4_CurrentMode->hide();
    ui->labelCenterR5_TargetMode->hide();



    /*
     * 初始化定时器，用于右侧菜单的显示与隐藏
     */

    m_timerDock = new QTimer(this);
    connect(m_timerDock, SIGNAL(timeout()), ui->menuDock, SLOT(hide()));
    m_timerDock->start(meterMenuDockIntval);


    m_msg2Dock=   new QTimer(this);
//    m_msg2Dock->stop();
    connect(m_msg2Dock, SIGNAL(timeout()), this, SLOT(showMsg1Dock()));
    m_msg2Dock->start(meterMenuDockIntval);

    m_msg3Dock =   new QTimer(this);
//    m_msg3Dock->stop();
     m_msg3Dock->start(meterMenuDockIntval);
     connect(m_msg3Dock, SIGNAL(timeout()), this, SLOT(showMsg1Dock()));



     connect(this, SIGNAL(reShowSingal()), this, SLOT(startMsgTimer()));


//    m_msg1Dock->stop();
//    m_msg2Dock->stop();
//    m_msg3Dock->stop();



#ifdef SIMULATION_DEMO
    /*
     * 初始化定时器，用于Demo演示动态效果
     * 	获取外部数据信息，更新仪表显示
     */
    m_timerDemo = new QTimer(this);
    connect(m_timerDemo, SIGNAL(timeout()), this, SLOT(meterDemo()));
    m_timerDemo->start(100);
#endif
    /*
     * 设置初始化值
     */
    setRpm(meterRpmMin);
    setTemp(meterTempMin);
    setSpeed(meterSpeedMin);
    setOil(meterOilMin);
    /*
     * 使能CAN接收
     */

    canActive(varList, N(varList));
//    qDebug() << "222222222222222222222222222222222222222222222222222222222"<< "initmeter" << showflg ;



  //  canSetVars(CAN_DEV1_ID,varList,N(varList));



    showMsg1Dock();
}

Meter::~Meter()
{
    /*
     * 离开时必须销毁所有动态创建的对象（包括new, malloc等）
     */
#ifdef SIMULATION_DEMO
    if (m_timerDemo)
        delete m_timerDemo;
#endif
    if (m_timerLight)
        delete m_timerLight;
    if (m_timerDock)
        delete m_timerDock;
    if (m_msg2Dock)
        delete m_msg2Dock;
    if (m_msg3Dock)
        delete m_msg3Dock;
    if (m_msgflg)
        delete m_msgflg;

    if (m_diag1)
        delete m_diag1;
    if (m_diag2)
        delete m_diag2;

    delete ui;
}


int Meter::initBackground()
{
    /*
     * 控件透明、可见性、层次，字体颜色等可以通过UI设计器、stylesheet等进行配置
	 */
    //ui->iconAlert->setVisible(false);

    return 0;
}

int Meter::initmsgmenu()
{
    ui->msg1Dock->show();
    ui->msg2Dock->hide();
    ui->msg3Dock->hide();

  }
void Meter::initmsg()
{
    showflg =1;
   m_msgflg->stop();
}

void Meter::startMsgTimer(void)
{
   m_msgflg->start(5000);
}

void Meter::updateStat(void)
{

   m_msgflg->start(5000);
}

int Meter::initScene()
{
    QPixmap m_needlePix(":/meter/needle.png");
    QPixmap m_needlePix2(":/meter/needle2.png");
    //m_needlePix = m_needlePix.scaled(meterNeedleLen, 22, Qt::KeepAspectRatio);
    //qDebug() << m_needlePix.width() << m_needlePix.height();

    // +2：因为表盘图片不规整（原点不在图片中心），这里做的偏移
//    qreal scale = meterNeedleLen / m_needlePix.width();
//    int originRoundX = ui->bgRound->x() + ui->bgRound->width()/2 + 1;
//    int originRoundY = ui->bgRound->y() + ui->bgRound->height()/2 - 6;
//    int originSpeedX = ui->bgSpeed->x() + ui->bgSpeed->width()/2 + 1;
//    int originSpeedY = ui->bgSpeed->y() + ui->bgSpeed->height()/2 - 6;
    int originRoundX = 360 + 2;
    int originRoundY = 360 + 1;
    int originSpeedX = 1920 - originRoundX;
    int originSpeedY = originRoundY;

	/*
	 * 俩表盘：表盘1是转速、温度
	 * 	注意防止表心及文字被覆盖
	 */
    m_diag1 = new meterNeedle(m_needlePix, m_needlePix2, originRoundX, originRoundY, meterNeedleLen, this);
    m_diag1->raise();
//    ui->bgRoundCenter->raise();
//    ui->labelRoundVal->raise();
//    ui->labelRoundUnit->raise();

	/*
	 * 俩表盘：表盘2是时速、油量
	 * 	注意防止表心及文字被覆盖
	 */
    m_diag2 = new meterNeedle(m_needlePix, m_needlePix2, originSpeedX, originSpeedY, meterNeedleLen, this);
    m_diag2->raise();
//    ui->bgSpeedCenter->raise();
//    ui->labelSpeedVal->raise();
//    ui->labelSpeedUnit->raise();

    return 0;
}

/*
 * 用于指针1的数值转换为角度
 */
qreal Meter::toAngle1(qreal min, qreal max, qreal val)
{
	/*
	 * 360.0 - 90.0：可用刻度不是360度
	 * +90+45：指针图片默认指向 与表盘0点刻度不一致，进行偏移
	 */
    qreal a = (360.0 - 90.0) * val / (max - min) + 90 + 45;

    while (a > 360)
        a -= 360;
    while (a < 0)
        a += 360;

    return a;
}

/*
 * 用于指针2的数值转换为角度
 */
qreal Meter::toAngle2(qreal min, qreal max, qreal val)
{
	/*
	 * 45.0：可用刻度仅45度，不是360度
	 * 90 + 22.5：指针图片默认指向 与表盘0点刻度不一致，进行偏移
	 */
    qreal a = 45.0 * val / (max - min);

    a = 90 + 22.5 - a;

    while (a > 360)
        a -= 360;
    while (a < 0)
        a += 360;
    return a;
}

/*
 * 用于设置转速
 */
qreal Meter::setRpm(qreal val)
{
    if (val < meterRpmMin || val > meterRpmMax)
        return -1;

    m_valRpm = val;
	// 更新指针1及文字
    m_diag1->setAngle1(toAngle1(meterRpmMin, meterRpmMax, m_valRpm));
//    ui->labelML_EngineTime->setText(QString("%1").arg(m_valRpm, 0, 'f', 0));

    return m_valRpm;
}

/*
 * 发动机工作时间
 */
qreal Meter::setEngineTime(qreal val)
{
    if (val < meterEngineTimeMin || val > meterEngineTimeMax)
        return -1;

    m_valEngineTime = val;
    ui->labelML_EngineTime->setText(QString("%1").arg(m_valEngineTime, 0, 'f', 0));

    return m_valEngineTime;
}

/*
 * 用于温度
 */
qreal Meter::setTemp(qreal val)
{
    if (val < meterTempMin || val > meterTempMax)
        return -1;

    m_valTemp = val;
	// 更新指针2
    m_diag1->setAngle2(toAngle2(meterTempMin, meterTempMax, m_valTemp));

    return m_valTemp;
}

/*
 * 用于设置时速
 */
qreal Meter::setSpeed(qreal val)
{
    if (val < meterSpeedMin || val > meterSpeedMax)
        return -1;

    m_valSpeed = val;
    // 更新指针1及文字
    m_diag2->setAngle1(toAngle1(meterSpeedMin, meterSpeedMax, m_valSpeed));
    //ui->labelSpeedVal->setText(QString("%1").arg(m_valSpeed, 0, 'f', 0));

    return m_valSpeed;


}

/*
 * 已行驶里程
 */
qreal Meter::setMileage(qreal val)
{
    if (val < meterMileageMin || val > meterMileageMax)
        return -1;

    m_valMileage = val;
    ui->labelMR_Mileage->setText(QString("%1").arg(m_valMileage/1000, 0, 'f', 0));

    return m_valMileage;
}

/*
 * 用于设置油量
 */
qreal Meter::setOil(qreal val)
{
    if (val < meterOilMin || val > meterOilMax)
        return -1;
    m_valOil = val;
	// 更新指针2
    m_diag2->setAngle2(toAngle2(meterOilMin, meterOilMax, m_valOil));
    return m_valOil;
}

/*
 * 用于各种指示灯状态的配置
 *	状态包括：开、关、闪烁
 */
int Meter::setLight(MeterLight_t light, MeterLightState_t state)
{
    if (light >= MeterLightNum)
        return -1;
    if (state >= MeterLightStateNum)
        return -1;

    m_lights[light].state = state;
    m_lights[light].duration = 0;
    m_lights[light].blinkOn = false;
    return 0;
}

/*
 * 用于屏中的几行文字信息的显示
 *	可以通过级别自定义文字颜色、样式等
 */
int Meter::setMessage(MeterMsgLine_t line, MeterMsgLevel_t level, const QString &msg)
{
    if (line >= MeterMsgLineNum)
        return -1;
    if (level >= MeterMsgLevelNum)
        return -1;

    QLabel *objLabels[] = {
        ui->labelCenterR1_BusStatus,
        ui->labelCenterR2_SwerveStatus,
        ui->labelCenterR3_AdblueLevel,
        ui->labelCenterR4_CurrentMode,
        ui->labelCenterR5_TargetMode,
    };

    QString c = "color:rgb(60, 197, 228);";

    switch (level) {
    case MeterMsgNormal:
        c = "color:rgb(60, 197, 228);";
        break;
    case MeterMsgWarning:
    case MeterMsgCritical:
        c = "color:red;";
        break;
    default:
        break;
    }

    objLabels[line]->setText(msg);
    objLabels[line]->setStyleSheet(c);

    return 0;
}

/*
 * 维护表盘状态的定时器
 */
void Meter::updateState()
{
    QWidget *objs[] = {
        qobject_cast<QWidget *>(ui->iconR1C1_FrontFog),
        qobject_cast<QWidget *>(ui->iconR1C2_RearFog),
        qobject_cast<QWidget *>(ui->iconR1C3_Beam),
        qobject_cast<QWidget *>(ui->iconR1C4_Position),
        qobject_cast<QWidget *>(ui->iconR1C5_ReverseGear),

        qobject_cast<QWidget *>(ui->iconR2C1_PTO),
        qobject_cast<QWidget *>(ui->iconR2C2_BrakeWear),
        qobject_cast<QWidget *>(ui->iconR2C3_ABSFault),
        qobject_cast<QWidget *>(ui->iconR2C4_Exhaust),
        qobject_cast<QWidget *>(ui->iconR2C5_TCUFaultyellow),
        qobject_cast<QWidget *>(ui->iconR2C6_TCUFaultred),
        qobject_cast<QWidget *>(ui->iconR2C7_FDXOilTempHigh),
        qobject_cast<QWidget *>(ui->iconR2C8_AirPressLow),
        qobject_cast<QWidget *>(ui->iconR2C9_SwerveFault),

        qobject_cast<QWidget *>(ui->iconR3C1_AxleDiff),
        qobject_cast<QWidget *>(ui->iconR3C2_WheelDiff),
        qobject_cast<QWidget *>(ui->iconR3C3_ManyAxle),
        qobject_cast<QWidget *>(ui->iconR3C4_LowGear),
        qobject_cast<QWidget *>(ui->iconR3C5_YuRe),
        qobject_cast<QWidget *>(ui->iconR3C6_YZHS),
        qobject_cast<QWidget *>(ui->iconR3C7_EngineFault),
        qobject_cast<QWidget *>(ui->iconR3C8_PaiFang),
        qobject_cast<QWidget *>(ui->iconR3C9_AdblueLow),

        qobject_cast<QWidget *>(ui->iconMLC1_Unfastened),
        qobject_cast<QWidget *>(ui->iconMLC2_GearAdd),
        qobject_cast<QWidget *>(ui->iconMLC2_GearSub),
//        qobject_cast<QWidget *>(ui->iconMLC3_Temp),

        qobject_cast<QWidget *>(ui->iconMRC1_Park),
        qobject_cast<QWidget *>(ui->iconMRC2_OilPress),
        qobject_cast<QWidget *>(ui->iconMRC3_Cruise),

//        qobject_cast<QWidget *>(ui->iconMRC4_Oil),

//        qobject_cast<QWidget *>(ui->labelML_EngineTime),
//        qobject_cast<QWidget *>(ui->labelML_EngineTimeUnit),
//        qobject_cast<QWidget *>(ui->labelML_GearValueTarget),
//        qobject_cast<QWidget *>(ui->labelML_GearValue),
//        qobject_cast<QWidget *>(ui->labelML_RpmTitle),

//        qobject_cast<QWidget *>(ui->labelMR_Mileage),
//        qobject_cast<QWidget *>(ui->labelMR_MileageUnit),
//        qobject_cast<QWidget *>(ui->labelMR_SpeedUnit),

//        qobject_cast<QWidget *>(ui->labelTop_Date),
//        qobject_cast<QWidget *>(ui->labelTop_Time),
//        qobject_cast<QWidget *>(ui->labelTop_Temp),

//        qobject_cast<QWidget *>(ui->labelCenterR1_BusStatus),
//        qobject_cast<QWidget *>(ui->labelCenterR2_SwerveStatus),
//        qobject_cast<QWidget *>(ui->labelCenterR3_AdblueLevel),
//        qobject_cast<QWidget *>(ui->labelCenterR4_CurrentMode),
//        qobject_cast<QWidget *>(ui->labelCenterR5_TargetMode),
    };

    for (unsigned int i=0; i<N(m_lights); i++) {
        if (i >= N(objs) || !objs[i])
            continue;

        bool oldVisible = objs[i]->isVisible();
        bool newVisible = oldVisible;
        switch (m_lights[i].state) {
        case MeterLightOn:
            newVisible = true;
            break;
        case MeterLightOff:
            newVisible = false;
            break;
        case MeterLightBlink:
            /*
             * 动态闪烁（显示一段时间、隐藏一段时间）
             */
            m_lights[i].duration += meterStateIntval;
            if (m_lights[i].duration >= meterLightDuration) {
                m_lights[i].blinkOn = !m_lights[i].blinkOn;
                newVisible = m_lights[i].blinkOn;
                m_lights[i].duration = 0;
            }
            break;
        default:
            break;
        }

        if (oldVisible != newVisible)
            objs[i]->setVisible(newVisible);
    }

    QDateTime nowTime = QDateTime::currentDateTime();
    ui->labelTop_Date->setText(nowTime.toString("yyyy/MM/dd"));
    ui->labelTop_Time->setText(nowTime.toString("hh:mm:ss"));
    int temp = 0;
    sensor_temp_read(&temp);
    ui->labelTop_Temp->setText(QString("%1℃").arg((int)(temp/1000)));


}

void Meter::showMenuDock()
{
    /*
     * 当用户活动时，重新显示菜单，并启动隐藏定时器
     */

    m_timerDock->stop();
    connect(m_timerDock, SIGNAL(timeout()), ui->menuDock, SLOT(hide()));
    m_timerDock->start(meterMenuDockIntval);
    ui->menuDock->show();

}

void Meter::showMsg1Dock()
{
    /*
     * 当用户活动时，重新显示菜单，并启动隐藏定时器
     */
     m_msg2Dock->stop();
     m_msg3Dock->stop();
     ui->msg2Dock->hide();
     ui->msg3Dock->hide();
     ui->msg1Dock->show();
     ui->msg1Dock->setGeometry(ui->msg1Dock->x(),ui->msg1Dock->y(),ui->msg1Dock->width(),ui->msg1Dock->height());
//     qDebug() << "showMsg1Dock"<< "showflg" << showflg ;
}
void Meter::showMsg2Dock()
{

//    qDebug() << "showMsg2Dock" ;    qDebug() << "showMsg2Dock"<< "showflg" << showflg ;
    if (showflg == 1)
    {
         m_msg3Dock->stop();
    /*
     * 当用户活动时，重新显示菜单，并启动隐藏定时器
     */
         m_msg2Dock->start(10000);
         ui->msg1Dock->hide();
         ui->msg3Dock->hide();
         ui->msg2Dock->show();
         ui->msg2Dock->setGeometry(ui->msg1Dock->x(),ui->msg1Dock->y(),ui->msg1Dock->width(),ui->msg1Dock->height());
        }
}
void Meter::showMsg3Dock()
{
    /*
     * 当用户活动时，重新显示菜单，并启动隐藏定时器
     */

//    qDebug() << "showMsg3Dock" ;
//    qDebug() << "showMsg3Dock"<< "showflg" << showflg ;
    if (showflg == 1)
    {
        m_msg2Dock->stop();
        m_msg3Dock->start(10000);
        ui->msg1Dock->hide();
        ui->msg2Dock->hide();
        ui->msg3Dock->show();
        ui->msg3Dock->setGeometry(ui->msg1Dock->x(),ui->msg1Dock->y(),ui->msg1Dock->width(),ui->msg1Dock->height());
        ui->ccslbl->setText(QString("%1" "%2").arg(m_valCcsSpeed,0,'f',0).arg("km/h"));
    }
}
void Meter::keyPressEvent(QKeyEvent *event)
{
    bool dockVisible = ui->menuDock->isVisible();

    showMenuDock();
    if (!dockVisible)
        return;
    /*
     * 设置菜单快捷键
     */
    switch (event->key()) {
    case Qt::Key_F1:
        on_btnPort_clicked();
        break;
    case Qt::Key_F2:
        on_btnJ1939_clicked();
        break;
    case Qt::Key_F3:
        on_btnNet_clicked();
        break;
    case Qt::Key_F4:
        on_btnSteering_clicked();
        break;
    case Qt::Key_F5:
        on_btnMain_clicked();
        break;
    default:
        Page::keyPressEvent(event);
        break;
    }
}

void Meter::mousePressEvent(QMouseEvent* event)
{
    showMenuDock();
    Page::mousePressEvent(event);
}

void Meter::onPageBack(int code, QString msg)
{
    showMenuDock();
    Page::onPageBack(code, msg);
}

void Meter::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    static qreal gearValue_current;

//    qDebug() << "333333333333333333333333333333333333333"<< "initmeter" << showflg ;

//    qDebug() << "Meter" << __func__ << __LINE__ << "var: " << m->var << "code=" << code;

    if (code != MSG_CAN_OD)
        return;

    for (unsigned int i=0; i<N(varList); i++) {
        if (varList[i].var == m->var) {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;
            /*
             * 注意：这里的代码仅是编程示例，与实际应用并不相同
             */
//            qDebug() << "Meter" << __func__ << __LINE__ << "var: " << m->var << "val=" << val;
            switch (m->var) {
            case can1_r_18A_b45:    // rpm
                setRpm(val);
                break;
            case can1_r_18A_b1:    // temp
                setTemp(val);
                break;
            case can1_r_28A_b0:    // speed
                setSpeed(val);


                break;
            case can1_r_28A_b1:    // 当前档位
                if(val==0)
                {
                    ui->labelML_GearValue->setText("N");
                }
                else if(val>0)
                {
                    ui->labelML_GearValue->setText("D"+QString("%1").arg(qAbs(val),0,'f',0));
                }
                else
                {
                    ui->labelML_GearValue->setText("R"+QString("%1").arg(qAbs(val),0,'f',0));


                }
                gearValue_current = val;
                break;
            case can1_r_48A_b4:    // 目标档位

                if(val>gearValue_current)
                {
                    ui->labelML_GearValueTarget->setVisible(true);
                    ui->labelML_GearValueTarget->setText(QString("%1").arg(qAbs(val),0,'f',0));
                    setLight(MeterLightMLC2_GearAdd, MeterLightOn);
                    setLight(MeterLightMLC2_GearSub, MeterLightOff);
                }
                else if(val<gearValue_current)
                {
                    ui->labelML_GearValueTarget->setVisible(true);
                    ui->labelML_GearValueTarget->setText(QString("%1").arg(qAbs(val),0,'f',0));
                    setLight(MeterLightMLC2_GearAdd, MeterLightOff);
                    setLight(MeterLightMLC2_GearSub, MeterLightOn);
                }
                else
                {
                    ui->labelML_GearValueTarget->setVisible(false);
                    setLight(MeterLightMLC2_GearAdd, MeterLightOff);
                    setLight(MeterLightMLC2_GearSub, MeterLightOff);
                }
                break;
            case can1_r_28A_b2:    // 燃油表
                setOil(val);
                break;
            case can1_r_28A_b3:    // 尿素液位
                ui->labelCenterR3_AdblueLevel->setText("尿素液位："+QString("%1").arg(val, 0, 'f', 0)+"%");
                break;
            case can1_r_38A_b4567:    // 累计里程
                setMileage(val);
                break;
            case can1_r_38A_b0123:    // 累计时间
                ui->labelML_EngineTime->setText(QString("%1").arg(val/60, 0, 'f', 0));
                break;
            case can1_r_184_b5:    // 行车灯
                setLight(MeterLightR1C4_Position, (m->val_int32 & (1 << 7)) ? MeterLightOn : MeterLightOff);
                break;
            case can1_r_28A_b6:    // ABS故障
                setLight(MeterLightR2C3_ABSFault, (m->val_int32 & (1 << 2)) ? MeterLightOn : MeterLightOff);
                break;
            case can1_r_48A_b0:    //
                setLight(MeterLightR2C1_PTO, (m->val_int32 & (1 << 0)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C2_WheelDiff, (m->val_int32 & (1 << 2)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C1_AxleDiff, (m->val_int32 & (1 << 3)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightMRC1_Park, (m->val_int32 & (1 << 4)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C9_AdblueLow, (m->val_int32 & (1 << 5)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR1C3_Beam, (m->val_int32 & (1 << 6)) ? MeterLightOn : MeterLightOff);
                break;
            case can1_r_48A_b1:    //
                setLight(MeterLightR1C5_ReverseGear, (m->val_int32 & (1 << 0)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR1C2_RearFog, (m->val_int32 & (1 << 1)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR1C1_FrontFog, (m->val_int32 & (1 << 2)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C5_YuRe, (m->val_int32 & (1 << 3)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C6_YZHS, (m->val_int32 & (1 << 4)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C7_EngineFault, (m->val_int32 & (1 << 5)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C8_PaiFang, (m->val_int32 & (1 << 7)) ? MeterLightOn : MeterLightOff);
                break;
             case can1_r_48A_b2:
                setLight(MeterLightR2C9_SwerveFault, (m->val_int32 & (1 << 5)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightMLC1_Unfastened, (m->val_int32 & (1 << 6)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR2C2_BrakeWear, (m->val_int32 & (1 << 7)) ? MeterLightOn : MeterLightOff);
                break;
             case can1_r_48A_b3:

                setLight(MeterLightR2C7_FDXOilTempHigh, (m->val_int32 & (1 << 0)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR2C5_TCUFaultyellow, (m->val_int32 & (1 << 1)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR2C6_TCUFaultred, (m->val_int32 & (1 << 2)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR2C4_Exhaust, (m->val_int32 & (1 << 3)) ? MeterLightOn : MeterLightOff);
                setLight(MeterLightR3C4_LowGear, (m->val_int32 & (1 << 4)) ? MeterLightOn : MeterLightOff);
                break;

            case can1_r_193_b01:
                showMsg2Dock();

               if((m->val_int32 & 0x00ff)== 0x01 )//制冷
               {
                   QPixmap *pixmaphot = new QPixmap(":/Airconditioner/8_1.png");
                   QPixmap *pixmapcold = new QPixmap(":/Airconditioner/7_2.png");
                   ui->air_hotmode->setPixmap(*pixmaphot);
                   ui->air_coldmode->setPixmap(*pixmapcold);
               }

               else if ((m->val_int32 & 0x00ff)== 0x02 )//通风
               {
                   QPixmap *pixmaphot = new QPixmap(":/Airconditioner/8_1.png");
                   QPixmap *pixmapcold = new QPixmap(":/Airconditioner/7_1.png");
                   ui->air_hotmode->setPixmap(*pixmaphot);
                   ui->air_coldmode->setPixmap(*pixmapcold);
               }
               else if ((m->val_int32 & 0x00ff)== 0x03 )// 制热
               {
                   QPixmap *pixmaphot = new QPixmap(":/Airconditioner/8_2.png");
                   QPixmap *pixmapcold = new QPixmap(":/Airconditioner/7_1.png");
                   ui->air_hotmode->setPixmap(*pixmaphot);
                   ui->air_coldmode->setPixmap(*pixmapcold);
               }

               if ((m->val_int32 & 0xff00)== 0x0100 )//自动关闭
               {
                   QPixmap *pixauto = new QPixmap(":/Airconditioner/9_2.png");
                   ui->air_automode->setPixmap(*pixauto);
               }
               else if ((m->val_int32 & 0xff00)== 0x0000 ) // 自动
               {
                   QPixmap *pixauto = new QPixmap(":/Airconditioner/9_1.png");
                   ui->air_automode->setPixmap(*pixauto);
               }

                break;
            case can1_r_193_b23:
                showMsg2Dock();

                // 设定温度显示
                ui->air_temp->setText(QString("%1" "%2").arg(m->val_int32&0xff,0,'f',0).arg("℃"));
                    break;
            case can1_r_193_b45:
                showMsg2Dock();
                if ((m->val_int32 & 0x00ff)== 0x0001 )//吹脚
                {
                 QPixmap *pixfacemode = new QPixmap(":/Airconditioner/3_1.png");
                 QPixmap *pixfootmode = new QPixmap(":/Airconditioner/4_2.png");
                 QPixmap *pixfrontmode = new QPixmap(":/Airconditioner/1_1.png");
                 QPixmap *pixfrontfacemode = new QPixmap(":/Airconditioner/2_1.png");

                 ui->air_foot->setPixmap(*pixfootmode );
                 ui->air_face->setPixmap(*pixfacemode);
                 ui->air_front_face->setPixmap(*pixfrontfacemode);
                 ui->air_front->setPixmap(*pixfrontmode);
                }

                else  if ((m->val_int32 & 0x00ff)== 0x0002 )//吹头
                {
                 QPixmap *pixfacemode = new QPixmap(":/Airconditioner/3_2.png");
                 QPixmap *pixfootmode = new QPixmap(":/Airconditioner/4_1.png");
                 QPixmap *pixfrontmode = new QPixmap(":/Airconditioner/1_1.png");
                 QPixmap *pixfrontfacemode = new QPixmap(":/Airconditioner/2_1.png");

                 ui->air_foot->setPixmap(*pixfootmode );
                 ui->air_face->setPixmap(*pixfacemode);
                 ui->air_front_face->setPixmap(*pixfrontfacemode);
                 ui->air_front->setPixmap(*pixfrontmode);
                }
               else if ((m->val_int32 & 0x00ff)== 0x0003 )// 除雾
                {
                 QPixmap *pixfacemode = new QPixmap(":/Airconditioner/3_1.png");
                 QPixmap *pixfootmode = new QPixmap(":/Airconditioner/4_1.png");
                 QPixmap *pixfrontmode = new QPixmap(":/Airconditioner/1_2.png");
                 QPixmap *pixfrontfacemode = new QPixmap(":/Airconditioner/2_1.png");
                 ui->air_foot->setPixmap(*pixfootmode );
                 ui->air_face->setPixmap(*pixfacemode);
                 ui->air_front_face->setPixmap(*pixfrontfacemode);
                 ui->air_front->setPixmap(*pixfrontmode);
                }
                else  if ((m->val_int32 & 0x00ff)== 0x0004 ) // 吹脚除雾
                {
                 QPixmap *pixfacemode = new QPixmap(":/Airconditioner/3_1.png");
                 QPixmap *pixfootmode = new QPixmap(":/Airconditioner/4_1.png");
                 QPixmap *pixfrontmode = new QPixmap(":/Airconditioner/1_1.png");
                 QPixmap *pixfrontfacemode = new QPixmap(":/Airconditioner/2_2.png");

                 ui->air_foot->setPixmap(*pixfootmode );
                 ui->air_face->setPixmap(*pixfacemode);
                 ui->air_front_face->setPixmap(*pixfrontfacemode);
                 ui->air_front->setPixmap(*pixfrontmode);
                }
                break;
            case can1_r_193_b67:
                showMsg2Dock();

              break;

            case can1_r_293_b01:
//                showMsg2Dock();
                if ((m->val_int32 & 0x00ff)== 0x0000 ) // 关闭
                {
                 QPixmap *pixairspeed = new QPixmap(":/Airconditioner/6_1.png");
                 ui->air_speed->setPixmap(*pixairspeed );
                }
                else if((m->val_int32 & 0x00ff)== 0x0001 ) // 1档
                {
                 QPixmap *pixairspeed = new QPixmap(":/Airconditioner/6_2.png");
                 ui->air_speed->setPixmap(*pixairspeed );
                }
                else if((m->val_int32 & 0x00ff)== 0x0002 ) // 2档
                {
                 QPixmap *pixairspeed = new QPixmap(":/Airconditioner/6_3.png");
                 ui->air_speed->setPixmap(*pixairspeed );
                }
                else if((m->val_int32 & 0x00ff)== 0x0003 ) // 3档
                {
                 QPixmap *pixairspeed = new QPixmap(":/Airconditioner/6_4.png");
                 ui->air_speed->setPixmap(*pixairspeed );
                }
                else if((m->val_int32 & 0x00ff)== 0x0004 ) // 4档
                {
                 QPixmap *pixairspeed = new QPixmap(":/Airconditioner/6_5.png");
                 ui->air_speed->setPixmap(*pixairspeed );
                }
                break;

            case can1_r_48A_b6://定速巡航
                m_valCcsSpeed = m->val_int32;
                showMsg3Dock();
                break;
            }
            break;
        }
    }
}

void Meter::on_btnPort_clicked()
{
    showflg = 2;
//    qDebug() << "on_btnPort_clicked"<< "showflg" << showflg ;
    //openSubPage(new viewPort());
}

void Meter::on_btnJ1939_clicked()
{
    showflg = 2;
//    qDebug() << "on_btnJ1939_clicked"<< "showflg" << showflg ;
    //openSubPage(new viewJ1939());
}

void Meter::on_btnNet_clicked()
{
    showflg =2;
    //openSubPage(new viewNet());
}

void Meter::on_btnSteering_clicked()
{
    showflg = 2;
//    qDebug() << "on_btnSteering_clicked"<< "showflg" << showflg ;
    //openSubPage(new viewSteering());
}

void Meter::on_btnMain_clicked()
{
    showflg = 2;

//    qDebug() << "on_btnMain_clicked"<< "showflg" << showflg ;
    //openSubPage(new menuMain());
}

#ifdef SIMULATION_DEMO
/*
 * 用于演示的定时器：演示接口及效果
 *	实际编程时可以移除
 */
void Meter::meterDemo()
{
    static bool r = true, t = true, s = true, o = true;

    setRpm(m_valRpm + qrand() % (int)meterRpmMax * 0.02 * (r?1:-1));
    setEngineTime(m_valEngineTime + qrand() % 3);
    setTemp(m_valTemp + qrand() % (int)meterTempMax * 0.02 * (t?1:-1));
    setSpeed(m_valSpeed + qrand() % (int)meterSpeedMax * 0.02 * (s?1:-1));
    setMileage(m_valMileage + qrand() % 10);
    setOil(m_valOil + qrand() * 1.0 / RAND_MAX * 0.02 * (o?1:-1));

    if (m_valRpm >= meterRpmMax * 0.95)
        r = false;
    if (m_valTemp >= meterTempMax * 0.95)
        t = false;
    if (m_valSpeed >= meterSpeedMax * 0.95)
        s = false;
    if (m_valOil >= 1 * 0.9)
        o = false;

    if (m_valRpm <= meterRpmMax * 0.05)
        r = true;
    if (m_valTemp <= meterTempMax * 0.05)
        t = true;
    if (m_valSpeed <= meterSpeedMax * 0.05)
        s = true;
    if (m_valOil <= 1 * 0.05)
        o = true;

    static bool b = false;
    if (!b) {
        b = true;
        setLight(MeterLightLeft, MeterLightBlink);
        setLight(MeterLightReverse, MeterLightOn);
        setLight(MeterLightB1, MeterLightOn);
        setLight(MeterLightB2, MeterLightOn);
        setLight(MeterLightB3, MeterLightOn);
        setLight(MeterLightB4, MeterLightOn);
        setMessage(MeterMsgLine6, MeterMsgWarning, tr("这是一个演示程序"));
    }
}
#endif
