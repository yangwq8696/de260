#ifndef FORM_MAINLMINEW_H
#define FORM_MAINLMINEW_H

#include <QWidget>
#include "FUN_MeterFinger.h"
#include "FUN_ButtonBar.h"
#include "page.h"
#include "FUNC_FickerLabel.h"


namespace Ui {
class FORM_MainLmiNew;
}

class FORM_MainLmiNew : public Page
{
    Q_OBJECT

public:
    explicit FORM_MainLmiNew(Page *parent = nullptr);
    ~FORM_MainLmiNew();
    void showForm();
    void closeForm();

private slots:
    void onCanRecv(int code, msg_can_t *m) override;
    void dateTimeDisplay();
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();
protected:
    void paintEvent(QPaintEvent *event);      //绘图事件
    void showEvent(QShowEvent*);
private:
    Ui::FORM_MainLmiNew *ui;
    FUN_ButtonBar *buttonBar;
    FUN_MeterFinger *m_finger1;
    FUN_MeterFinger *m_finger2;
    FUN_MeterFinger *m_finger3;
    FUN_MeterFinger *m_finger4;
    QTimer *m_timerLight;
    bool m_uiInited;
    QStringList currentWaringList;

    FUNC_FickerLabel* m_fickerLabel_Hanging;  //回落闪烁标签
    FUNC_FickerLabel* m_fickerLabel_turnLeft; //左转灯闪烁标签
    FUNC_FickerLabel* m_fickerLabel_turnRight;//右转灯闪烁标签
    FUNC_FickerLabel* m_fickerLabel_urgent;   //紧急信号灯闪烁标签
    FUNC_FickerLabel* m_fickerLabel_positionLight;   //灯闪烁标签

    /*转向灯优先级处理*/
    bool m_urgent;        //双闪


 private:
    void setCurrentWarningList();

    qreal setOilTemperature(int val);
    qreal setOilPressure(int val);
    qreal setWaterTemperature(int val);
    qreal setEngineSpeed(int val);

    void loadFunctionKey(void);
    int initScene();
    void lan_change();
    qreal valueToAngle(qreal minangle, qreal anglearea, qreal minvalue, qreal maxvalue, qreal value);

    bool RetardContinousAlarm(); //GE制动持续报警
    bool NoPropulsionAlarm();    //GE无推进力报警
    bool SystemFaultAlarm();     //GE系统故障报警
    bool NORetardAlarm();        //GE无制动力报警
    bool OilLevelAlarm();        //燃油位低报警
    bool DoorAlarm();            //车门未关闭报警
    bool SeatBeltAlarm();        //安全带报警
    bool ReturnFilterCloggingAlarm();        //回油过滤器堵塞报警
    bool HighPressureFilterCloggingAlarm();  //高压过滤器堵塞报警
    bool AirFilterBlockAlarm();              //空滤堵塞报警
    bool SteeringPressureLowAlarm();         //转向压力低报警

    /*spn发动机红黄白灯报警*/
    void changeLongLongToChar8(long long var,unsigned char data[8]);
    //CA
    unsigned char ch_feca[8]={0};
    //EC
    uint8_t data_EC[8]={0};
    //EB
    uint8_t data_EB[8]={0};


};

#endif // FORM_MAINLMINEW_H
