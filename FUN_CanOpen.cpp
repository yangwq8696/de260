#include "FUN_CanOpen.h"
#include "page.h"

libMsg *FUN_CanOpen::m_msg = NULL;
static bool qtimerHeartflg=false;
QTimer *FUN_CanOpen::heartBeatErrorTimer=NULL;
HEART_BEAT_NET_STR FUN_CanOpen::heartBeatNetStr[]={{false,0},
                                    {0,false,0},
                                     {0,false,0},
                                     {0,false,0},
                                     {0,false,0},
                                     {0,false,0},
                                     {0,false,0},
                                     {0,false,0},
                                     {0,false,0},
                                     {0,false,0},
                                     {0,false,0}
                                    };
static canVar_t varList[] = {
    { C1R201B0,         0xff,           1,  0    },
};
/**************************************************************/
FUN_CanOpen::FUN_CanOpen()
{
    /*
     * 考虑所有派生类共同使用从底层上来的数据，m_msg定义为一个静态成员
     * 仅初化一次即可
     */
    if (m_msg == NULL) {
        m_msg = new libMsg();
        m_msg->canOpen(CAN1_DEV, CAN1_NODE, CAN1_BAUD, CAN1_EDS);
    }
    if(qtimerHeartflg==false)
    {
        qtimerHeartflg=true;
        for(int i=0;i<HEART_BEAT_NET_NUM_MAX;i++)
        {
            heartBeatNetStr->flg   =false;
            heartBeatNetStr->count =0;
        }
        heartBeatErrorTimer =new QTimer();
//        connect(heartBeatErrorTimer, SIGNAL(timeout()), this,SLOT(heartBeatTimerUpdate()));
        heartBeatErrorTimer->start(1000);
//        qDebug() << "heart Timer init";
    }
//    setAttribute(Qt::WA_DeleteOnClose,1);
}
/**************************************************************/
void FUN_CanOpen::heartBeatTimerUpdate(void)
{
    static int i=0;

    i++;
    for(int j=0;j<HEART_BEAT_NET_NUM_MAX;j++)
    {
        heartBeatNetStr[j].count++;
        if(heartBeatNetStr[j].count>=10)
        {
            heartBeatNetStr[j].count=10;
            heartBeatNetStr[j].flg = true;
        }
    }
}
/**************************************************************/
void FUN_CanOpen::canActive(const canVar_t *list, int num)
{
    canVar_t *varNew = new canVar_t[N(varList)+N(list)];
    num=N(list)+N(varList);
    for(int i=0;i<N(list);i++)
        varNew[i] = list[i];
    for(int i=0;i<N(varList);i++)
        varNew[i+N(list)] = varList[i];

    m_canActived = true;
    m_canVarList = varNew;
    m_canVarNum = N(list)+N(varList);
//    connect(m_msg, SIGNAL(canRecv(int, msg_can_t *)), this, SLOT(onCanRecv(int, msg_can_t *)));
//    connect(m_msg, SIGNAL(canRecv(int, msg_can_t *)), this, SLOT(canReHeatBeat(int, msg_can_t *)));

    canSetVars(CAN1_DEV, varNew, num);

}
/**************************************************************/
void FUN_CanOpen::canDeactive()
{
    if (!m_canActived)
        return;
    /*
     * 取消CAN的接收处理
     */
//    disconnect(m_msg, SIGNAL(canRecv(int, msg_can_t *)), this, SLOT(onCanRecv(int, msg_can_t *)));
    m_canActived = false;
    canSetVars(CAN1_DEV, NULL, 0);
}
/**************************************************************/
void FUN_CanOpen::canSetVars(int devId, const canVar_t *list, int num)
{
    /*
     * 配置监听的CAN变量
     * 这里同时缓存了CAN变量，用于基类重新激活CAN时配置使用
     */
    if (list != NULL && num > 0)
        m_msg->canVars(devId, list, num);

    int type;
    uint32_t val;
    msg_can_t msg;
    for(int i=0;i<num;i++)
    {
//        qDebug() << "444444444444444444444"<< "initmeter"  ;

        val=canopen_read_var_int(CAN_DEV1_ID,list[i].var,&type);
        memset(&msg,0,sizeof(msg));
        msg.devid       =CAN1_DEV;
        msg.var         = list[i].var;
        msg.val_int32   =val;
        onCanRecv(MSG_CAN_OD,&msg);
    }
}
/******************************发送PDO********************************/
int FUN_CanOpen::canSendPdo(int devId, int pdoNum)
{
    return m_msg->canSendPdo(devId, pdoNum);
}
