#ifndef FUN_CANOPEN_H
#define FUN_CANOPEN_H
#include <QMatrix>
#include <QImage>
#include <QString>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPoint>
#include <QMainWindow>
#include <QPaintEvent>
#include "hsmlib.h"
#include "libmsg.h"
#define HEART_BEAT_NET_NUM_MAX 11


typedef  struct{
    quint8 lastval;
    bool flg;
    int count;
}HEART_BEAT_NET_STR;


class FUN_CanOpen
{
public:
    FUN_CanOpen();

    /*
     * CAN的相关处理函数
     */
    void canActive(const canVar_t *list, int num);
    void canDeactive();
    void canSetVars(int devId, const canVar_t *list, int num);
    int canSendPdo(int devId, int pdoNum);
public slots:
    virtual void onCanRecv(int /*code*/, msg_can_t */*m*/) {}
    static  void heartBeatTimerUpdate(void);
    static  void canReHeatBeat(int code, msg_can_t *m);
    static  void heartBeatfuc(uint32_t val,int num);
protected:
    static libMsg *m_msg;
    bool m_pageSelf=false;

    static HEART_BEAT_NET_STR heartBeatNetStr[HEART_BEAT_NET_NUM_MAX];
    static QTimer *heartBeatErrorTimer;
private:
    bool m_canActived;
    const canVar_t *m_canVarList;
    int m_canVarNum;
};



#endif // FUN_CANOPEN_H
