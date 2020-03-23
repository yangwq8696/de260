#ifndef FUNC_ERRORCODE_H
#define FUNC_ERRORCODE_H

#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QVariant>
#include <QDateTime>
#include <QList>
#include "page.h"
#include "ErrorTypedef.h"
#include <QStringList>
#include <QVariantList>
#define MaxErrorNumber 86



class FUNC_ErrorCode
{
public:
    FUNC_ErrorCode();
    /*激活故障状态*/
    void API_SetErrorStatus(ErrorTypedef *Error,bool status);
    void API_SetErrorStatus(uint16_t ErrorCode, bool status);
    /*设置故障信息*/
    void API_SetErrorCode(ErrorTypedef *Error,QString  ErrorInfor,uint16_t ErrorCode,uint8_t  ErrorClass);
    /*初始化故障信息*/
    void API_InitCode(void);
    /*通过故障码获得故障索引*/
    int API_GetErrorIndex(uint16_t code);
    /*获得当前故障码*/
    uint16_t  *API_GetCurrentError(unsigned short& count) ;
    QVariantList getCurrentErrorInfo(); //获取当前故障列表
    QVariantList getCurrentErrorInfo2(); //获取当前故障列表
    /*获得当前故障总数*/
    uint16_t   API_GetCurrentErrorNumber(void) ;

    /*获得当前故障码List*/
    QVariantList API_GetCurrentErrorList(void) ;
    /*转化工具*/
    void API_CycleToBit(long long value);
    void U64ToChar(long long var, uint8_t *data);
    /*故障码*/
    ErrorTypedef Error[MaxErrorNumber] ;
    /*实时故障*/
    uint16_t  RealTimeError[MaxErrorNumber] ;
    /*实时故障list code Name*/
    //QVariantList  RealTimeErrorList ;
    /*数据库存储*/
    bool API_OpenLocalDB(void);
    /*关闭数据库*/
    void API_CloseLocalDB(void);
    /*故障存储*/
    void SaveErrorToDB(ErrorTypedef *Error, bool en);

    // void AddErrorToDB(QList<QVariantList> var);
    void AddErrorToDB(QVariant var);
     /*获取历史故障*/
    QVariantList API_GetHistoryErrorFromDB(void);

private:
    /*数据库文件指针*/
    QSqlDatabase *M_LocalDB;
    QVariantList M_Error;
private:
    QTimer *ErrorClear;
    uint16_t QQQQ;
};

#endif // FUNC_ERRORCODE_H
