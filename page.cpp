#include "page.h"
#include <QApplication>
#include <QMatrix>
#include <QImage>
#include <QString>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPoint>
#include <QMainWindow>
#include <QPaintEvent>
#include "main.h"
#include "FUNC_ErrorCode.h"

libMsg *Page::m_msg = NULL;

static bool qchildflg=false;
QTranslator *Page::m_translator = NULL;         // 多语言翻译器
LanguageId_t Page::m_languageId = LanguageNone; // 语言种类标识
QElapsedTimer Page::m_timer_TimeConsuming;

uint8_t errorValue[12]={0,0,0,0,0,0,0,0,0,0,0,0};
const uint32_t id_time[] = {0x04FA0001,3000/*,0x04FB0001,2000,0x04FA0014,2000*/};//主控制器、液压控制器 ,称重系统
/*
 * 主程序监听的CAN变量
 */
static canVar_t varList[] =
{
    { C1_R04FA0013B18,        0xffffffff,           1,  0    },
};

bool Page::m_bMainCtrlState = 0;  //主控总线状态
bool Page::m_bHydraulicState = 0; //液压总线状态
bool Page::m_bWeighingSystem = 0;


Page::Page(QWidget *parent):
    QWidget(parent),
    m_canActived(0),
    m_canVarList(0),
    m_canVarNum(0),
    res_HeartIdCfg(-1)
{
    /*
     * 考虑所有派生类共同使用从底层上来的数据，m_msg定义为一个静态成员
     * 仅初化一次即可
     */
    if (m_msg == NULL)
    {
        m_msg = new libMsg();
        m_msg->canOpen(CAN1_DEV, CAN1_NODE, CAN1_BAUD, CAN1_EDS);
        res_HeartIdCfg = canopen_j1939_mode(CAN1_DEV,true);
        res_HeartIdCfg = canopen_heartbeat_list_set(CAN1_DEV,id_time,1);
        connect(m_msg, SIGNAL(canRecv(int, msg_can_t *)), this, SLOT(canReHeatBeat(int, msg_can_t *)));
//        connect(m_msg, SIGNAL(sysRecvMsg(int, uint32_t)), this, SLOT(recvCanUpdateMsg(int, uint32_t)));

    }
    setAttribute(Qt::WA_DeleteOnClose,1);//


    //初始化语言选择部分 ，读取存储的语言标记
    uint8_t langId = LanguageZh;
    eeprom_read(EEPROM_LANGUAGE_OFFSET, &langId, EEPROM_LANGUAGE_SIZE);
    switch (langId) {
    case 0:
        m_languageId = LanguageZh;
        break;
    case 1:
        m_languageId = LanguageEn;
        break;
    case 2:
        m_languageId = LanguageRu;
        break;
    default:
        m_languageId = LanguageZh;
    }
    setLanguage(m_languageId);


    //Time Consuming
    m_timer_TimeConsuming.start();
}

Page::~Page()
{
    /*
     * 取消本页面的CAN接收处理
     */
    canDeactive();
    //qDebug() << "page";
}
void Page::show()
{


    /*
     * 全屏显示
     */
    if (!isFullScreen())
        showFullScreen();

    QWidget::show();
#if 0
    /*
     * 安装事件监听，用于：
     *  1. 设置焦点选中时样式
     *  2. 重新定义按键映射
     */
    if(!qchildflg)
    {
        qchildflg = true;
        QObjectList list = children();

        foreach (QObject *obj, list)
        {
            obj->installEventFilter(this);
        }
    }
#endif
}

void Page::canReHeatBeat(int code, msg_can_t *m)  //m 是否是数组？？？(默认不是)
{
    uint32_t val;
    long long  value;

    switch(code)
    {
        case MSG_CAN_OD: // OD 操作
            for (unsigned int i=0; i<N(varList); i++) {
                if (varList[i].var == m->var)
                {
                    //val--CAN信息的解析结果
                    val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

                    switch (m->var) //CAN字典变量
                    {
                    case   C1_R04FA0013B18://
                        value = canopen_read_var_uint64(CAN1_DEV, C1_R04FA0013B18, NULL);
                        Func_ErrorCode->API_CycleToBit(value);
//                        qDebug()<<"C1_R04FA0013B18:"<<value;
                       break;
#if 0
                    case   C1_R18FECA00B18://
                        value = canopen_read_var_uint64(CAN1_DEV, C1_R18FECA00B18, NULL);
//                        qDebug()<<"C1_R18FECA00B18:"<<value;
                       break;
#endif


                    }
                }
            }
        break;
        case MSG_CAN_HEARTBEAT: // CAN心跳
//             if(m->var == 0x04FA0001){
//                 m_bMainCtrlState = m->val_int32;
//                 if(m_bMainCtrlState==1)
//                     m_timer_TimeConsuming.restart();
//                 else{
//                    qDebug()<<"0x04FA0001:    "
//                            <<m->val_int32<<"\t time consuming:"
//                            <<m_timer_TimeConsuming.elapsed();
//                 }

//             }
//             if(m->var == 0x04FB0001){
//                 m_bHydraulicState = m->val_int32;
//                 qDebug()<<"0x04FB0001:    "<<m->val_int32<<"\t size:"<<m->size;
//             }
//             if(m->var == 0x04FA0014){
//                 m_bWeighingSystem = m->val_int32;
//                 qDebug()<<"0x04FA0014:    "<<m->val_int32<<"\t size:"<<m->size;
//             }
        break;
    }
}

/**
 * @brief Page::canActive  激活CAN监听，添加监听的can变量
 * @param devid     CAN接口ID
 * @param list      本页面要监听的CAN变量
 * @param num
 */
void Page::canActive(int devid, const canVar_t *list, int num)
{
    int numtotal;
    canVar_t *varNew = new canVar_t[N(varList) + num];
    numtotal = num + N(varList);
//    qDebug() << __func__ << __LINE__ << N(list) << " ertger" << N(varList);

    for(int i=0;i<num;i++)
        varNew[i] = list[i];
    for(int i=0;i<N(varList);i++)
        varNew[i+num] = varList[i];

    m_canActived = true;
    m_canVarList = varNew;
    m_canVarNum = numtotal;
    connect(m_msg, SIGNAL(canRecv(int, msg_can_t *)), this, SLOT(onCanRecv(int, msg_can_t *)));
//    connect(m_msg, SIGNAL(canRecv(int, msg_can_t *)), this, SLOT(canReHeatBeat(int, msg_can_t *)));

    canSetVars(devid, varNew, numtotal); //配置监听CAN变量
}

void Page::canDeactive()
{
    if (!m_canActived)
        return;
    /*
     * 取消CAN的接收处理
     */
    disconnect(m_msg, SIGNAL(canRecv(int, msg_can_t *)), this, SLOT(onCanRecv(int, msg_can_t *)));
    m_canActived = false;
    canSetVars(CAN1_DEV, NULL, 0);
    canSetVars(CAN2_DEV, NULL, 0);
}

void Page::canSetVars(int devId, const canVar_t *list, int num)
{
//    qDebug() << __func__ << __LINE__ << num;
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
        val = canopen_read_var_int(CAN_DEV1_ID,list[i].var,&type);
        memset(&msg,0,sizeof(msg));
        msg.devid       = devId;
        msg.var         = list[i].var;
        msg.val_int32   = val;
        onCanRecv(MSG_CAN_OD,&msg);
    }
}

int Page::canSendPdo(int devId, int pdoNum)
{
    return m_msg->canSendPdo(devId, pdoNum);
}
/*************************************************************************************/
void Page::onRelease_Button1()
{

}
void Page::onRelease_Button2()
{

}
void Page::onRelease_Button3()
{

}
void Page::onRelease_Button4()
{

}
void Page::onRelease_Button5()
{

}

void Page::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_F1:
//        qDebug() << "key1";
        onRelease_Button1();
        break;
    case Qt::Key_F2:
//        qDebug() << "key2";
        onRelease_Button2();
        break;
    case Qt::Key_F3:
//        qDebug() << "key3";
        onRelease_Button3();
        break;
    case Qt::Key_F4:
//        qDebug() << "key4";
        onRelease_Button4();
        break;
    case Qt::Key_F5:
//        qDebug() << "key5";
        onRelease_Button5();
        break;
    default:
        break;
    }

    QWidget::keyPressEvent(event);
}
/*************************************************************************************/
/******************************************************************************
 * 函数名  : setLanguage
 * 函数参数: lang: 语言ID
 * 函数功能: 配置当前语言
 *****************************************************************************/
int Page::setLanguage(LanguageId_t lang, bool test)
{
    m_languageId = lang;

    QString strLanguage;
    if (m_translator == NULL) {
        m_translator = new QTranslator;
    }
    switch (lang) {
        case 0:
            strLanguage = ":/language/LANG_zh.qm";
            break;
        case 2:
            strLanguage = ":/language/LANG_ru.qm";
            break;
        case 1:
            strLanguage = ":/language/LANG_en.qm";
            break;
    default:
        strLanguage = ":/language/LANG_zh.qm";
        }
    m_translator->load(strLanguage);
    qApp->installTranslator(m_translator);
    return 0;
}
