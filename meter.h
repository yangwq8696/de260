#ifndef METER_H
#define METER_H

#include <QPixmap>
#include <QPainter>
#include <QTransform>
#include <QtWidgets/QGraphicsView>
#include <QGraphicsPixmapItem>
#include "page.h"
#include "meterNeedle.h"
#include "FORM_Menu.h"
//#define SIMULATION_DEMO     1

namespace Ui {
class Meter;
}


/*
 * 指示灯列表
 */
enum MeterLight_t {
    MeterLightR1C1_FrontFog,
    MeterLightR1C2_RearFog,
    MeterLightR1C3_Beam,
    MeterLightR1C4_Position,
    MeterLightR1C5_ReverseGear,

    MeterLightR2C1_PTO,
    MeterLightR2C2_BrakeWear,
    MeterLightR2C3_ABSFault,
    MeterLightR2C4_Exhaust,
    MeterLightR2C5_TCUFaultyellow,
    MeterLightR2C6_TCUFaultred,
    MeterLightR2C7_FDXOilTempHigh,
    MeterLightR2C8_AirPressLow,
    MeterLightR2C9_SwerveFault,

    MeterLightR3C1_AxleDiff,
    MeterLightR3C2_WheelDiff,
    MeterLightR3C3_ManyAxle,
    MeterLightR3C4_LowGear,
    MeterLightR3C5_YuRe,
    MeterLightR3C6_YZHS,
    MeterLightR3C7_EngineFault,
    MeterLightR3C8_PaiFang,
    MeterLightR3C9_AdblueLow,

    MeterLightMLC1_Unfastened,
    MeterLightMLC2_GearAdd,
    MeterLightMLC2_GearSub,
//    MeterLightMLC3_Temp,


    MeterLightMRC1_Park,
    MeterLightMRC2_OilPress,
    MeterLightMRC3_Cruise,
//    MeterLightMRC4_Oil,

    MeterLightNum
};

/*
 * 指示灯状态
 */
enum MeterLightState_t {
    MeterLightOff           = 0,
    MeterLightOn,
    MeterLightBlink,

    MeterLightStateNum
};

/*
 * 消息行
 */
enum MeterMsgLine_t {
    MeterMsgLine1           = 0,
    MeterMsgLine2,
    MeterMsgLine3,
    MeterMsgLine4,
    MeterMsgLine5,
    MeterMsgLine6,

    MeterMsgLineNum
};

/*
 * 消息级别
 */
enum MeterMsgLevel_t {
    MeterMsgNormal     = 0,
    MeterMsgWarning,
    MeterMsgCritical,

    MeterMsgLevelNum
};

/*
 * 指示灯状态结构
 */
typedef struct MeterLightInfo_t {
    MeterLightState_t state;
    unsigned int duration;
    bool blinkOn;
}MeterLightInfo_t;

class Meter : public Page
{
    Q_OBJECT

public:
    explicit Meter(Page *parent = nullptr);
    ~Meter() override;

    /*
     * 仪表的对外接口
     */
    qreal setRpm(qreal val);
    qreal setEngineTime(qreal val);
    qreal setTemp(qreal val);
    qreal setSpeed(qreal val);
    qreal setMileage(qreal val);
    qreal setOil(qreal val);
    int setLight(MeterLight_t light, MeterLightState_t state);
    int setMessage(MeterMsgLine_t line, MeterMsgLevel_t level, const QString &msg);

    void startMsgTimer(void);

    void updateStat(void);
private:
    Ui::Meter *ui;

    /*
     * 表盘指针
     */
    meterNeedle *m_diag1;
    meterNeedle *m_diag2;

    /*
     * 缓存值
     */
    qreal m_valRpm;
    qreal m_valEngineTime;
    qreal m_valTemp;
    qreal m_valSpeed;
    qreal m_valMileage;
    qreal m_valOil;
    qreal m_valCcsSpeed;
    MeterLightInfo_t m_lights[MeterLightNum];

    /*
     * 定时器
     */
#ifdef SIMULATION_DEMO
    QTimer *m_timerDemo;
#endif
    QTimer *m_timerLight;
    QTimer *m_timerDock;
    QTimer *m_msgflg;
    QTimer *m_msg2Dock;
    QTimer *m_msg3Dock;

    /*
     * 内部初始化函数
     */
    int initBackground();
    int initScene();
    int initmsgmenu();

    // 上指针转换函数,如果表盘刻度（图片）调整的话，需要相应调整
    qreal toAngle1(qreal min, qreal max, qreal val);
    // 下指针转换函数,如果表盘刻度（图片）调整的话，需要相应调整
    qreal toAngle2(qreal min, qreal max, qreal val);

private slots:
    /*
     * 信号处理函数
     */
#ifdef SIMULATION_DEMO
    void meterDemo();
#endif
    //void onPageBack(int code, QString msg) override;
    void onCanRecv(int code, msg_can_t *m) override;
    void updateState();
    void showMenuDock();
    void showMsg1Dock();
    void showMsg2Dock();
    void showMsg3Dock();
    void initmsg();


    void on_btnPort_clicked();
    void on_btnJ1939_clicked();
    void on_btnNet_clicked();
    void on_btnSteering_clicked();
    void on_btnMain_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // METER_H
