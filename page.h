/*
* 监控can通讯，监控键盘事件
* 关于can信息，通过m_msg的信号将信息传出去，窗口类接收这个信号，在槽函数onCanRecv()中处理,重载onCanRecv()即可
* 关于键盘事件，通过虚函数进行处理，子类（窗口类）只需要重载这个虚函数即可
*/

#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QTranslator>
#include <QSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QMessageBox>
#include <QTableWidget>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>

#include "hsmlib.h"
#include "libmsg.h"

#define HEART_BEAT_NET_NUM_MAX 11

/*语言类型*/
enum LanguageId_t {
    LanguageNone    = -1,
    LanguageZh      = 0,
    LanguageEn      = 1,
    LanguageRu      = 2,
    LanguagePrt     = 3,
};

typedef  struct{
    quint8 lastval;
    bool flg;
    int count;
}HEART_BEAT_NET_STR_old;

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(QWidget *parent = nullptr);
    virtual ~Page() override;
    /*
     * CAN的相关处理函数
     */
    void canActive(int devid, const canVar_t *list, int num);
    void canDeactive();
    void canSetVars(int devId, const canVar_t *list, int num);
    int canSendPdo(int devId, int pdoNum);

    /**
     * 按键事件处理
     */
    virtual void onRelease_Button1();
    virtual void onRelease_Button2();
    virtual void onRelease_Button3();
    virtual void onRelease_Button4();
    virtual void onRelease_Button5();
    void keyPressEvent(QKeyEvent *event);

    void setMainCtrlState(bool bState){m_bMainCtrlState = bState;}
    bool getMainCtrlState(){return m_bMainCtrlState;}        //获取主控总线状态
    bool getHydraulicState(){return m_bHydraulicState;}      //获取液压总线状态
    bool getWeighingSystemState(){return m_bWeighingSystem;} //称重系统can通信状态


signals:

public slots:
    // 重载show，用于插入检测处理
    void show();
    virtual void updateStat() {}
    virtual void onCanRecv(int /*code*/, msg_can_t */*m*/) {}
    virtual void recvCanUpdateMsg(int type, uint32_t code){}

    static  void canReHeatBeat(int code, msg_can_t *m);

public:
    static QTranslator *m_translator;
protected:
    static libMsg *m_msg;
    bool m_pageSelf=false;
    QKeyEvent *evevt;
    static LanguageId_t m_languageId;
private:
    bool m_canActived;
    const canVar_t *m_canVarList;
    int m_canVarNum;
    int res_HeartIdCfg;

    static bool m_bMainCtrlState;  //主控总线状态
    static bool m_bHydraulicState; //液压总线状态
    static bool m_bWeighingSystem; //称重系统can通信状态
    static QElapsedTimer m_timer_TimeConsuming;


    int setLanguage(LanguageId_t lang, bool test = false);

};

#endif // PAGE_H
