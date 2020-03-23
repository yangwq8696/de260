/*
*   封装了can通讯，监控can信息,收到信息后通过信号将信息传出
*   静态指针，只初始化一次
*/


#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include <QTimer>
#include "defines.h"
#include "hsmlib.h"


typedef struct canConfig_t {
    int used;
    //int devId;
    int nodId;
    int bitRate;
    char edsFile[MAX_NAME_LEN];
} canConfig_t;

typedef struct canVar_t {
    int var;
    uint32_t mask;
    float scale;
    float offset;
} canVar_t;


class libMsg: public QWidget
{
    Q_OBJECT
public:
    libMsg(QWidget *parent = nullptr);
    virtual ~libMsg();
    int canOpen(int devid, int nodeid, int bitrate, const char *edsfile);
    int canSendPdo(int devId, int pdoNum);
    void canVars(int devId, const canVar_t *list, int num);

public slots:
    void monitor();

signals:
    void canRecv(int type, msg_can_t *can);  //收到can信息，类型为“CAN心跳、OD操作、SDO写、SDO读”
    void sysRecvMsg(int type, uint32_t code);    //收到can信息，类型为“升级状态”

private:
    QTimer *m_timerRecv;
    canConfig_t m_canCfg[CAN_DEV_NUM];

    uint8_t m_count;
};

#endif // MESSAGE_H
