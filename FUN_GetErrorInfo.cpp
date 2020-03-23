#include "FUN_GetErrorInfo.h"

#include "main.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QVariant>
#include <QDateTime>

FUN_GetErrorInfo::FUN_GetErrorInfo()
{
    m_localdb = 0;
    for(int i=0;i<ERRORNUMBER;i++){
        NowErrorCodeArray[i] = ""; NowErrorInfoArray[i] = "";
    }

    errorInfoInit();//故障信息初始化
    judgeNewConsole();//判断显示器是否是第一次使用
//    readRecordedData();
}

//故障信息初始化
void FUN_GetErrorInfo::errorInfoInit()
{

    for(int i=0;i<ERRORNUMBER;i++){
        InitErrorCodeArray[i] = ""; InitErrorInfoArray[i] = "";
    }

    openLocalDB();

    int i = 0;
    //添加数据
    QSqlQuery query(*m_localdb);
    QString sql = "select * from WarningInfo;";
    query.exec(sql);
    while (query.next())
    {
        QString strCode= query.value(0).toString();
        QString strName = query.value(1).toString();

        InitErrorCodeArray[i] = strCode;
        InitErrorInfoArray[i] = strName;
        i++;
    }
//    qDebug()<<InitErrorCodeArray[0]<<InitErrorCodeArray[1]<<InitErrorCodeArray[2]<<InitErrorCodeArray[3]<<InitErrorCodeArray[4];
//    qDebug()<<InitErrorInfoArray[0]<<InitErrorInfoArray[1]<<InitErrorInfoArray[2]<<InitErrorInfoArray[3]<<InitErrorInfoArray[4];

    closeDB();

/*
    InitErrorCodeArray[0] = ""; InitErrorInfoArray[0] = "发动机黄灯报警";
    InitErrorCodeArray[1] = ""; InitErrorInfoArray[1] = "发动机红灯报警";
    InitErrorCodeArray[2] = ""; InitErrorInfoArray[2] = "发动机水温高报警";
    InitErrorCodeArray[3] = ""; InitErrorInfoArray[3] = "机油压力低报警";
    InitErrorCodeArray[4] = ""; InitErrorInfoArray[4] = "水箱水位低报警";
    InitErrorCodeArray[5] = ""; InitErrorInfoArray[5] = "液压油温高报警";
    InitErrorCodeArray[6] = ""; InitErrorInfoArray[6] = "燃油油位低报警";
    InitErrorCodeArray[7] = ""; InitErrorInfoArray[7] = "驻车制动压力低报警";
    InitErrorCodeArray[8] = ""; InitErrorInfoArray[8] = "空滤堵塞报警1";
    InitErrorCodeArray[9] = ""; InitErrorInfoArray[9] = "空滤堵塞报警2";

    InitErrorCodeArray[10] = ""; InitErrorInfoArray[10] = "空滤堵塞报警3";
    InitErrorCodeArray[11] = ""; InitErrorInfoArray[11] = "空滤堵塞报警4";
    InitErrorCodeArray[12] = ""; InitErrorInfoArray[12] = "系统电压低报警";
    InitErrorCodeArray[13] = ""; InitErrorInfoArray[13] = "";
    InitErrorCodeArray[14] = ""; InitErrorInfoArray[14] = "";
    InitErrorCodeArray[15] = ""; InitErrorInfoArray[15] = "";
    InitErrorCodeArray[16] = ""; InitErrorInfoArray[16] = "总线键盘CAN故障";
    InitErrorCodeArray[17] = ""; InitErrorInfoArray[17] = "仪表CAN故障";
    InitErrorCodeArray[18] = ""; InitErrorInfoArray[18] = "发动机CAN故障";
    InitErrorCodeArray[19] = ""; InitErrorInfoArray[19] = "GPS CAN故障";

    InitErrorCodeArray[20] = ""; InitErrorInfoArray[20] = "GE CAN故障";
    InitErrorCodeArray[21] = ""; InitErrorInfoArray[21] = "";
    InitErrorCodeArray[22] = ""; InitErrorInfoArray[22] = "";
    InitErrorCodeArray[23] = ""; InitErrorInfoArray[23] = "";
    InitErrorCodeArray[24] = ""; InitErrorInfoArray[24] = "GE无制动力报警";
    InitErrorCodeArray[25] = ""; InitErrorInfoArray[25] = "GE系统故障报警";
    InitErrorCodeArray[26] = ""; InitErrorInfoArray[26] = "GE无推进力报警";
    InitErrorCodeArray[27] = ""; InitErrorInfoArray[27] = "GE制动持续报警";
    InitErrorCodeArray[28] = ""; InitErrorInfoArray[28] = "";
    InitErrorCodeArray[29] = ""; InitErrorInfoArray[29] = "";

    InitErrorCodeArray[30] = ""; InitErrorInfoArray[30] = "";
    InitErrorCodeArray[31] = ""; InitErrorInfoArray[31] = "";
    InitErrorCodeArray[32] = ""; InitErrorInfoArray[32] = "左前称重传感器错误报警";
    InitErrorCodeArray[33] = ""; InitErrorInfoArray[33] = "右前称重传感器错误报警";
    InitErrorCodeArray[34] = ""; InitErrorInfoArray[34] = "左后称重传感器错误报警";
    InitErrorCodeArray[35] = ""; InitErrorInfoArray[35] = "右后称重传感器错误报警";
    InitErrorCodeArray[36] = ""; InitErrorInfoArray[36] = "转向器压力低报警";
    InitErrorCodeArray[37] = ""; InitErrorInfoArray[37] = "后桥刹车盘温度高报警左";
    InitErrorCodeArray[38] = ""; InitErrorInfoArray[38] = "后桥刹车盘温度高报警右";
    InitErrorCodeArray[39] = ""; InitErrorInfoArray[39] = "励磁控制器参数设置错误";

    InitErrorCodeArray[40] = ""; InitErrorInfoArray[40] = "柴油机超速";
    InitErrorCodeArray[41] = ""; InitErrorInfoArray[41] = "发电机轴承温度高";
    InitErrorCodeArray[42] = ""; InitErrorInfoArray[42] = "发动机电子绕组温度高";
    InitErrorCodeArray[43] = ""; InitErrorInfoArray[43] = "方向手柄错误";
    InitErrorCodeArray[44] = ""; InitErrorInfoArray[44] = "卡车超速报警";
    InitErrorCodeArray[45] = ""; InitErrorInfoArray[45] = "ACU和DCU通信异常";
    InitErrorCodeArray[46] = ""; InitErrorInfoArray[46] = "方向手柄和运行方向不一致";
    InitErrorCodeArray[47] = ""; InitErrorInfoArray[47] = "ACU和ECU通信异常";
    InitErrorCodeArray[48] = ""; InitErrorInfoArray[48] = "左轮电机线圈温度高";
    InitErrorCodeArray[49] = ""; InitErrorInfoArray[49] = "左轮电机轴承温度高";

    InitErrorCodeArray[50] = ""; InitErrorInfoArray[50] = "右轮电机线圈温度高";
    InitErrorCodeArray[51] = ""; InitErrorInfoArray[51] = "右轮电机轴承温度高";
    InitErrorCodeArray[52] = ""; InitErrorInfoArray[52] = "左轮电机线圈温度高卸载";
    InitErrorCodeArray[53] = ""; InitErrorInfoArray[53] = "左轮电机轴承温度高卸载";
    InitErrorCodeArray[54] = ""; InitErrorInfoArray[54] = "右轮电机线圈温度高卸载";
    InitErrorCodeArray[55] = ""; InitErrorInfoArray[55] = "右轮电机轴承温度高卸载";
    InitErrorCodeArray[56] = ""; InitErrorInfoArray[56] = "直流电压饱和";
    InitErrorCodeArray[57] = ""; InitErrorInfoArray[57] = "励磁电流过流";
    InitErrorCodeArray[58] = ""; InitErrorInfoArray[58] = "主发电机过流";
    InitErrorCodeArray[59] = ""; InitErrorInfoArray[59] = "发电机轴承温度高卸载";

    InitErrorCodeArray[60] = ""; InitErrorInfoArray[60] = "发电机定子绕组温度高卸载";
    InitErrorCodeArray[61] = ""; InitErrorInfoArray[61] = "左前轮速度传感器异常";
    InitErrorCodeArray[62] = ""; InitErrorInfoArray[62] = "右前轮速度传感器异常";
    InitErrorCodeArray[63] = ""; InitErrorInfoArray[63] = "ACU和ECM通信异常";
    InitErrorCodeArray[64] = ""; InitErrorInfoArray[64] = "举升阀压力低报警";
    InitErrorCodeArray[65] = ""; InitErrorInfoArray[65] = "转向阀压力低报警";
    InitErrorCodeArray[66] = ""; InitErrorInfoArray[66] = "转向蓄能器压力低报警";
    InitErrorCodeArray[67] = ""; InitErrorInfoArray[67] = "前桥制动压力低报警";
    InitErrorCodeArray[68] = ""; InitErrorInfoArray[68] = "后桥制动压力低报警";
    InitErrorCodeArray[69] = ""; InitErrorInfoArray[69] = "前桥制动蓄能器压力低报警";
    
    InitErrorCodeArray[70] = ""; InitErrorInfoArray[70] = "后桥制动蓄能器压力低报警";
    InitErrorCodeArray[71] = ""; InitErrorInfoArray[71] = "踏板先导阀压力低报警";
    InitErrorCodeArray[72] = ""; InitErrorInfoArray[72] = "制动梭阀压力低报警";
    InitErrorCodeArray[73] = ""; InitErrorInfoArray[73] = "前继动阀先导压力低报警";
    InitErrorCodeArray[74] = ""; InitErrorInfoArray[74] = "后继动阀先导压力低报警";
    InitErrorCodeArray[75] = ""; InitErrorInfoArray[75] = "前制动冷却温度高报警";
    InitErrorCodeArray[76] = ""; InitErrorInfoArray[76] = "后制动冷却温度高报警";
    InitErrorCodeArray[77] = ""; InitErrorInfoArray[77] = "制动油温度低于0度报警";
    InitErrorCodeArray[78] = ""; InitErrorInfoArray[78] = "精滤堵塞报警";
    InitErrorCodeArray[79] = ""; InitErrorInfoArray[79] = "集中润滑报警";

    InitErrorCodeArray[80] = ""; InitErrorInfoArray[80] = "安全带未系报警";
    InitErrorCodeArray[81] = ""; InitErrorInfoArray[81] = "车门未关闭报警";
    InitErrorCodeArray[82] = ""; InitErrorInfoArray[82] = "举升回油过滤器堵塞报警0";
    InitErrorCodeArray[83] = ""; InitErrorInfoArray[83] = "缓速器油温高报警";
    InitErrorCodeArray[84] = ""; InitErrorInfoArray[84] = "转向压力低报警";
    InitErrorCodeArray[85] = ""; InitErrorInfoArray[85] = "转向回油过滤器堵塞报警";
    InitErrorCodeArray[86] = ""; InitErrorInfoArray[86] = "";
    InitErrorCodeArray[87] = ""; InitErrorInfoArray[87] = "";
    InitErrorCodeArray[88] = ""; InitErrorInfoArray[88] = "";
    InitErrorCodeArray[89] = ""; InitErrorInfoArray[89] = "";
 */

}
QString FUN_GetErrorInfo::getHistoryErrorInfo(uint16_t index)
{
    return InitErrorInfoArray[index-1];
}
/**
 * @brief FUN_GetErrorInfo::getNowErrorinfo 获取当前实时故障信息
 * @param arr CAN设备发送过来的数据
 */
void FUN_GetErrorInfo::getNowErrorinfo(uint8_t *arr)
{
    for(int i=0;i<ERRORNUMBER;i++){
        NowErrorCodeArray[i] = ""; NowErrorInfoArray[i] = "";
    }

    uint16_t i,j,m=0;
    static uint8_t arrtemp[ERRORFLAGNUMBER]; //记录上一次读取到的报警信息(值：val)
    /*********************************************************/
    //if(GetNowErrorFinisfedFalg) return;
    for(j=0; j<ERRORFLAGNUMBER; j++)//解析故障标志位
    {
        if(arrtemp[j]!=arr[j]) //本次读取到的信息和arrtemp记录的信息不一样(注意:这里读到的不一定是新的故障信息，修改故障标志位后，才能判断是否是新的故障信息)
        {
            for(i=0; i<8; i++)
            {
                //
                //解析报警信息，修改故障标志位
                //
                if(unPackValue(arr[j],i)==1) //产生故障
                    ErrorFlagArray[8*j+i] = 1;  //yes  记录报警类型 (通过索引来确定报警类型)
                else  //故障清除或者从没发生过故障
                    ErrorFlagArray[8*j+i] = 0;
            }
        }
        arrtemp[j] = arr[j]; //排除重复的报警信息
    }
    /*********************************************************/
    for(i=0; i<ERRORNUMBER; i++)//获取当前所有故障信息
    {
        //获取当前系统存在的所有故障信息
        if(ErrorFlagArray[i] > 0)
        {
            NowErrorCodeArray[m] = InitErrorCodeArray[i];
            NowErrorInfoArray[m] = InitErrorInfoArray[i];
            m++;
        }

        //存储当前系统存在的故障信息
        if(ErrorFlagArray[i] != ErrorFlagArrayTemp[i]) //有新的故障报警出现或者有故障被修复
        {
            if(ErrorFlagArray[i]==1)  //出现新故障
            {
                if(i<88||i>91){ //排除称重故障(称重故障不写入历史记录)
                    //                errorInfoRecord();//故障记录
                                    //
                                    //将故障报警信息写入数据库
                                    //
                                    QDateTime dateTime = QDateTime::currentDateTime();
                                    QString strTime = dateTime.toString("yyyy-MM-dd hh:mm");
                                    QString strCode = InitErrorCodeArray[i];
                                    QString strName = InitErrorInfoArray[i];
                                    QStringList list;
                                    list<<strTime<<strCode<<strName;
                    //                qDebug()<<i<<"\t"<<list;
                    //                qDebug()<<InitErrorCodeArray[i]<<InitErrorInfoArray[i];
                                    QVariant var = list;
                                    addErrorInfo(var);

                }



            }else{ //故障被修复
//                qDebug()<<"refused@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
            }
            ErrorFlagArrayTemp[i] = ErrorFlagArray[i];
        }
    }
    TotalErrorNumber = m;//获取当前故障信息总数
    //    GetNowErrorFinisfedFalg = 1;
    //    qDebug() << "TotalErrorNumber=" << TotalErrorNumber;
}
//故障信息存储
void FUN_GetErrorInfo::errorInfoRecord()
{
    uint8_t savedata[ERRORLENGTH];
    uint16_t res;
    uint8_t arr1[2];
    uint8_t i;
    QDateTime curDateTime = QDateTime::currentDateTime();


    savedata[0] = curDateTime.toString("yyyy").toInt() - 2000;
    savedata[1] = curDateTime.toString("MM").toInt();
    savedata[2] = curDateTime.toString("dd").toInt();
    savedata[3] = curDateTime.toString("hh").toInt();
    savedata[4] = curDateTime.toString("mm").toInt();

    savedata[5] = ErrorInfoNumber & 0xff;
    savedata[6] = ErrorInfoNumber >> 8 & 0xff;
//    qDebug() <<"save"<<savedata[0];
//    qDebug() <<"save"<<savedata[1];
//    qDebug() <<"save"<<savedata[2];
//    qDebug() <<"save"<<savedata[3];
//    qDebug() <<"save"<<savedata[4];
//    qDebug() <<"save"<<savedata[5];
//    qDebug() <<"save"<<savedata[6];
    for(i=0;i<3;i++)
    {
        res = eeprom_write(1001 + ERRORLENGTH*SavedErrorNumber, savedata, ERRORLENGTH);
//        qDebug() << "res=" << res;
        if(res==ERRORLENGTH) break;
    }

    SavedErrorNumber++;
    if(SavedErrorNumber>ERRORRECORDNUM)
    {
        if(OverMaxErrorNumFalg==0)
        {
            OverMaxErrorNumFalg = 1;
            for(i=0;i<3;i++)
            {
                res = eeprom_write(998, &OverMaxErrorNumFalg, 1);
                if(res==1) break;
            }
        }
        SavedErrorNumber = 0;
    }
    arr1[0] = SavedErrorNumber & 0xff;
    arr1[1] = SavedErrorNumber>>8 & 0xff;
    for(i=0;i<3;i++)
    {
        res = eeprom_write(999, arr1, 2);
        if(res==2) break;
    }
}
//故障信息读取
void FUN_GetErrorInfo::readRecordedData()
{
    uint8_t buff3[ERRORRECORDNUM*ERRORLENGTH];
    uint8_t i,j;
    uint16_t res;

    if(!NewConsoleFlag)
    {
        for(i=0;i<3;i++)
        {
            res = eeprom_read(1001, buff3, N(buff3));
//            qDebug() << "readbuff3" << res;
            if(res==N(buff3)) break;
        }
        for(i=0;i<3;i++)
        {
            res = eeprom_read(998, &OverMaxErrorNumFalg, 1);
            if(res==1) break;
        }
        if(OverMaxErrorNumFalg==1)//所有故障区域已经存满了
        {
//            qDebug() << "OverMaxErrorNumFalg1" << OverMaxErrorNumFalg;
            for(i=SavedErrorNumber;i<ERRORRECORDNUM;i++)
            {
                for(j=0;j<ERRORLENGTH;j++)
                    HistoryErroArray[i-SavedErrorNumber][j] = buff3[i*ERRORLENGTH+j];
            }
            for(i=0;i<SavedErrorNumber;i++)
            {
                for(j=0;j<ERRORLENGTH;j++)
                    HistoryErroArray[ERRORRECORDNUM-SavedErrorNumber+i][j] = buff3[i*ERRORLENGTH+j];
            }
        }
        else//存储区没有存满
        {
//            qDebug() << "SavedErrorNumber" << SavedErrorNumber;
            for(i=0;i<SavedErrorNumber;i++)
            {
                for(j=0;j<ERRORLENGTH;j++)
                    HistoryErroArray[i][j] = buff3[i*ERRORLENGTH+j];
            }
        }
    }
}
//判断显示器是否示第一次使用
void FUN_GetErrorInfo::judgeNewConsole()
{
    uint8_t buff2[ERRORRECORDNUM*ERRORLENGTH+1];
    uint8_t buff1[1];
    uint8_t buff[4];
    int8_t res;
    uint8_t i,j;

    for(i=0;i<3;i++)
    {
        res = eeprom_read(5,buff,4);
        if(res==4) break;
    }

    if(res==4 && buff[0]==0x11 && buff[1]==0x22 && buff[2]==0x33 && buff[3]==0x44)
    {
        NewConsoleFlag = 0;
        for(i=0;i<3;i++)
        {
            res = eeprom_read(999,buff1,1);
            if(res==1) break;
        }

        SavedErrorNumber = buff1[0];
    }
    else
    {
        NewConsoleFlag = 1;
        buff[0] = 0x11;
        buff[1] = 0x22;
        buff[2] = 0x33;
        buff[3] = 0x44;
        for(i=0;i<3;i++)
        {
            res = eeprom_write(5, buff, N(buff));
            if(res==4) break;
        }
    }
    //如果显示器示第一次使用或者数据异常，则清零所有的故障储存区
    if(SavedErrorNumber>ERRORRECORDNUM || NewConsoleFlag)
    {
        for(i=0;i<ERRORRECORDNUM*ERRORLENGTH+1;i++)
            buff2[i] = 0;
        for(i=0;i<3;i++)
        {
            res = eeprom_write(1001, buff2, N(buff2));
            if(res==N(buff2)) break;
        }
        for(i=0;i<ERRORRECORDNUM;i++)
        {
            for(j=0;j<ERRORLENGTH;j++)
                HistoryErroArray[i][j] = 0;
        }
    }
}

bool FUN_GetErrorInfo::openLocalDB()
{
    if(!m_localdb)
        m_localdb = new QSqlDatabase;
    if (!m_localdb->open()){
        *m_localdb = QSqlDatabase::addDatabase("QSQLITE", "HistoryInfo");
        m_localdb->setDatabaseName("/apps/HistoryInfo.db");
        return m_localdb->open();
    }

    return false;
}

void FUN_GetErrorInfo::closeDB()
{
    if (m_localdb->isOpen())
    {
        m_localdb->close();
        QString name = m_localdb->connectionName();
        delete m_localdb;
        m_localdb = 0;
        QSqlDatabase::removeDatabase(name);
    }
}
/**
 * @brief FUN_GetErrorInfo::getCurrentErrorInfoFromDB
 * @return 二维数组，多行两列
 */
QVariantList FUN_GetErrorInfo::getCurrentErrorInfo(){
     QList<QVariant> list;

     for(int i=0;i<ERRORNUMBER;i++){
         QList<QVariant> strList;
         if(NowErrorInfoArray[i]!=""){
             strList<<NowErrorCodeArray[i]<<NowErrorInfoArray[i];
             list.append(QVariant(strList));
         }
     }

     return list;
}

/**
 * @brief FUN_GetErrorInfo::getHistoryErrorInfoFromDB
 * @return 二维数组，多行三列
 */
QVariantList FUN_GetErrorInfo::getHistoryErrorInfoFromDB(){
    QList<QVariant> list;

    openLocalDB();

    QSqlQuery query(*m_localdb);
    QString sql = "select a.dateTime,a.code,b.name from HistoryErrorInfo a,WarningInfo b"
                  " where a.code = b.code order by dateTime desc limit 1000";
    query.exec(sql);
    while (query.next())
    {
        QList<QVariant> strList;

        QVariant strDateTime= query.value(0);
        QVariant strCode = query.value(1);
        QVariant strName = query.value(2);

        strList<<strDateTime<<strCode<<strName;
        list.append(QVariant(strList));
    }

    closeDB();

    return list;
}

/**
 * @brief FUN_GetErrorInfo::addErrorInfo  向数据库表HistoryErrorInfo插入数据
 * @param val
 */
void FUN_GetErrorInfo::addErrorInfo(QVariant var){
    QStringList list = var.toStringList();
    if(list[0]==""||list[1]=="")
        return;

    openLocalDB();
    m_localdb->transaction();
    QSqlQuery query(*m_localdb);
    query.prepare("insert into HistoryErrorInfo (dateTime,code) values (:dateTime,:code)");
    query.bindValue(":dateTime",list[0]);
    query.bindValue(":code",list[1]);
//    query.bindValue(":name",list[2]);
    query.exec();
    m_localdb->commit();
    closeDB();

}

/**
 * @brief FUN_GetErrorInfo::deleteErrorInfo 删除数据库表HistoryErrorInfo中的指定的报警信息
 * @param str 报警信息
 */
void FUN_GetErrorInfo::deleteErrorInfo(QString str){
    openLocalDB();
    m_localdb->transaction();

    QSqlQuery query(*m_localdb);
    QString sql = "delete from HistoryErrorInfo where name = '"+str+"';";
    query.exec();

    m_localdb->commit();
    closeDB();
}




