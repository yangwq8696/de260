#include "libmsg.h"
#include <QDebug>

/*
 * 消息处理中心
 *  包括消息机制、CAN通信
 */

static const int monitorIntval = 10;

libMsg::libMsg(QWidget *parent):
    QWidget(parent),
    m_timerRecv(0)
{
    memset(m_canCfg, 0, CAN_DEV_NUM);

    m_timerRecv = new QTimer(this);
    // 定时检查消息接收
    connect(m_timerRecv, SIGNAL(timeout()), this, SLOT(monitor()));

    m_count = 0;
}

/**
 * @brief libMsg::~libMsg   关闭时钟，关闭can通讯
 */
libMsg::~libMsg()
{
    if (m_timerRecv != NULL){
        m_timerRecv->stop();
        delete m_timerRecv;
    }

    for (int i=0; i<CAN_DEV_NUM; i++) {
        if (m_canCfg[i].used) {
            canopen_stop(i);
        }
    }
}
/**
 * @brief libMsg::canOpen 打开can通讯，启动定时器读取can信息
 * @param devId     CAN设备号
 * @param nodeId    CAN节点号
 * @param bitRate   波特率
 * @param edsFile   CAN字典文件名
 * @return
 */
int libMsg::canOpen(int devId, int nodeId, int bitRate, const char *edsFile)
{
    if (devId < 0 || devId >= CAN_DEV_NUM)
        return -1;

    /*
     * 初始化CAN信息
     */
    m_canCfg[devId].used = 1;
    m_canCfg[devId].nodId = nodeId;
    m_canCfg[devId].bitRate = bitRate;
    strncpy(m_canCfg[devId].edsFile, edsFile, sizeof(m_canCfg[devId].edsFile));
    m_canCfg[devId].edsFile[sizeof(m_canCfg[devId].edsFile) - 1] = '\0';

    canopen_baud_rate(devId, bitRate);
    canopen_start(devId, nodeId, edsFile);
    canopen_start_node(devId, nodeId);
    //canopen_j1939_mode(devId, 1);

    m_timerRecv->stop();
    m_timerRecv->start(monitorIntval);

    return 0;
}
/**
 * @brief libMsg::canSendPdo    PDO写请求
 * @param devId     CAN设备号
 * @param pdoNum    PDO数量
 * @return
 */
int libMsg::canSendPdo(int devId, int pdoNum)
{
    return canopen_send_pdo(devId, pdoNum);
}

/**
 * @brief libMsg::canVars   配置监控节点列表
 * @param devid     CAN设备号
 * @param list      节点名列表
 * @param num       节点数量
 */
void libMsg::canVars(int devId, const canVar_t *list, int num)
{
    uint32_t *names = new uint32_t[num];

    for (int i=0; i<num; i++)
        names[i] = list[i].var;
  //  qDebug() << Q_FUNC_INFO << __LINE__ << num;

    canopen_monitor_vars(devId, names, num);
    delete names;
}

/**
 * @brief libMsg::monitor   获取can信息，然后进行处理
 */
void libMsg::monitor()
{
    int ret = 0;
    uint8_t buf[MSG_DATA_SIZE];
    msg_data_t *msg = (msg_data_t *)buf;

    do {
        memset(buf, 0, sizeof(buf));

        ret = msg_local_recv(msg);
        if (ret >= 0) {
//            qDebug() << "---------------------------------------------";
            switch (msg->type) {
            case MSG_CAN_HEARTBEAT:
//                qDebug() << "msg->type" << msg->type << msg->code;
            case MSG_CAN_OD:
            case MSG_CAN_SDO_READ:
            case MSG_CAN_SDO_WRITE:
#warning: TBD maybe error with memory pointer
#if 0
                msg_can_t *canMsg = (msg_can_t *)msg->pad;

//                qDebug() << __func__ << __LINE__
                         << "code:" << msg->type << "dev:" << canMsg->devid << "cobid:" << canMsg->cobid
                         << "idx:" << canMsg->index << "subidx:" << canMsg->subindex
                         << "type:" << canMsg->type << "val:" << canMsg->val_int32 << "size:" << canMsg->size;
#endif
                //发出CAN接收信号
//                qDebug() << "msg" << msg->type << msg->len;
                emit canRecv(msg->type, (msg_can_t *)msg->pad);
                break;

            case MSG_CPU_TEMP_ALARM:
            case MSG_EEPROM_ERROR:
            case MSG_UPGRADE_STATE:
                emit sysRecvMsg(msg->type, msg->code);
//                qDebug()<< "MSG_UPGRADE_STATE:msg->type" << msg->type << msg->code;
                break;

            default:
                break;
            }
        }
    } while (ret >= 0);
    /*发送pdo*/
//    if(m_count == 20){
//        canopen_write_var_int(CAN1_DEV,C1_T04FB0012B12,100);
//        canSendPdo(CAN1_DEV,1);
//        m_count=0;
//    }
//    m_count++;
}
