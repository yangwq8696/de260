#ifndef FUN_GETERRORINFO_H
#define FUN_GETERRORINFO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <QString>
#include <QVariantList>

#define ERRORNUMBER 96//故障总条数
#define ERRORRECORDNUM 200//故障记录总数
#define ERRORLENGTH 7//单条故障信息占的字节数
#define ERRORFLAGNUMBER ERRORNUMBER/8+1
#define ERRORNUMPERPAGE 10//每一页显示历史故障的条数

class QSqlDatabase;
class FUN_GetErrorInfo
{
public:
    FUN_GetErrorInfo();
    void getNowErrorinfo(uint8_t *arr);
    void readRecordedData();
    QString getHistoryErrorInfo(uint16_t index);

    QString NowErrorCodeArray[ERRORNUMBER];  //时时故障信息
    QString NowErrorInfoArray[ERRORNUMBER];
    uint16_t TotalErrorNumber;//当前故障信息总数
    uint8_t NewConsoleFlag;
    uint8_t HistoryErroArray[ERRORRECORDNUM][ERRORLENGTH];
    uint16_t SavedErrorNumber;//当前存储故障信息的编号，用于定位存储偏移地址
    uint8_t OverMaxErrorNumFalg;

    QVariantList getCurrentErrorInfo();
    QVariantList getHistoryErrorInfoFromDB();
private:
    bool openLocalDB();
    void closeDB();
    void addErrorInfo(QVariant var);
    void deleteErrorInfo(QString str);

private:
    void errorInfoInit();
    void errorInfoRecord();
    void judgeNewConsole();

    QString InitErrorCodeArray[ERRORNUMBER];  //报警信息code数组
    QString InitErrorInfoArray[ERRORNUMBER];  //报警信息文本数组
    uint8_t  ErrorFlagArray[ERRORNUMBER];     //记录故障标志位 (通过索引查看报警信息文本)
    uint8_t ErrorFlagArrayTemp[ERRORNUMBER];  //记录上一轮监测的故障标志位
    uint16_t ErrorInfoNumber;//故障信息编号

    QSqlDatabase* m_localdb;
};
#endif // FUN_GETERRORINFO_H
