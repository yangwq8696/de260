#include "FUNC_ErrorCode.h"

FUNC_ErrorCode::FUNC_ErrorCode()
{  
    M_LocalDB = 0;
    API_InitCode();
    /*数据库指针*/
    //API_OpenLocalDB();
   QQQQ = 0;
    M_Error.clear();
    memset(RealTimeError,0,sizeof (RealTimeError));
}

void FUNC_ErrorCode::API_SetErrorCode(ErrorTypedef *Error, QString ErrorInfor, uint16_t ErrorCode, uint8_t ErrorClass)
{    
    Error->ErrorInfor = ErrorInfor;
    Error->ErrorCode  = ErrorCode;
    Error->ErrorClass = ErrorClass;
    Error->ErrorStatus = false;
}

void FUNC_ErrorCode::API_InitCode()
{
    int i = 0;
    API_SetErrorCode(&Error[i++], "空滤堵塞报警１" , 101 , 1 );
    API_SetErrorCode(&Error[i++], "空滤堵塞报警２" , 102 , 1 );
    API_SetErrorCode(&Error[i++], "空滤堵塞报警３" , 103 , 1 );
    API_SetErrorCode(&Error[i++], "空滤堵塞报警４" , 104 , 1 );
    API_SetErrorCode(&Error[i++], "空滤堵塞报警５" , 105 , 1 );
    API_SetErrorCode(&Error[i++], "空滤堵塞报警６" , 106 , 1 );
    API_SetErrorCode(&Error[i++], "燃油油位低报警" , 107 , 1 );
    API_SetErrorCode(&Error[i++], "发动机黄灯报警" , 108 , 1 );
    API_SetErrorCode(&Error[i++], "发动机水温高报警" , 151 , 0 );
    API_SetErrorCode(&Error[i++], "机油压力低报警", 152 , 0 );
    API_SetErrorCode(&Error[i++], "冷却水位低报警", 153 , 0 );
    API_SetErrorCode(&Error[i++], "发动机红灯报警", 181 , 0 );
    API_SetErrorCode(&Error[i++], "车门未关闭报警", 201 , 1 );
    API_SetErrorCode(&Error[i++], "安全带报警", 202 , 1 );
    API_SetErrorCode(&Error[i++], "前左称重传感器压力错误", 203 , 1 );
    API_SetErrorCode(&Error[i++], "前右称重传感器压力错误", 204 , 1 );
    API_SetErrorCode(&Error[i++], "后左称重传感器压力错误", 205 , 1 );
    API_SetErrorCode(&Error[i++], "后右称重传感器压力错误", 206 , 1 );
    API_SetErrorCode(&Error[i++], "称重故障", 207 , 1 );
    API_SetErrorCode(&Error[i++], "系统电压低", 208 , 1 );
    API_SetErrorCode(&Error[i++], "中车驱动黄色报警", 209 , 1 );
    API_SetErrorCode(&Error[i++], "总线键盘CAN故障", 210 , 1 );
    API_SetErrorCode(&Error[i++], "仪表CAN故障", 211 , 1 );
    API_SetErrorCode(&Error[i++], "GPS CAN故障", 212 , 1 );
    API_SetErrorCode(&Error[i++], "主控制器CAN故障", 213 , 0 );
    API_SetErrorCode(&Error[i++], "液压控制器CAN故障", 214 , 0 );
    API_SetErrorCode(&Error[i++], "辅助控制器CAN故障", 215 , 1 );
    API_SetErrorCode(&Error[i++], "称重系统CAN故障", 216 , 1 );
    API_SetErrorCode(&Error[i++], "车梯故障", 251 , 0 );
    API_SetErrorCode(&Error[i++], "发动机CAN故障", 252 , 0 );
    API_SetErrorCode(&Error[i++], "GE系统故障报警", 253 , 0 );
    API_SetErrorCode(&Error[i++], "GE制动持续报警", 254 , 0 );
    API_SetErrorCode(&Error[i++], "GE系统CAN故障", 281 , 0 );
    API_SetErrorCode(&Error[i++], "中车驱动系统红色报警", 282 , 0 );
    API_SetErrorCode(&Error[i++], "GE无制动力报警", 283 , 0 );
    API_SetErrorCode(&Error[i++], "GE无推进力报警", 284 , 0 );
    API_SetErrorCode(&Error[i++], "吸油过滤堵塞报警１", 301 , 1 );
    API_SetErrorCode(&Error[i++], "吸油过滤堵塞报警２", 302 , 1 );
    API_SetErrorCode(&Error[i++], "吸油过滤堵塞报警３", 303 , 1 );
    API_SetErrorCode(&Error[i++], "回油过滤堵塞报警１", 304 , 1 );
    API_SetErrorCode(&Error[i++], "回油过滤堵塞报警２", 305 , 1 );
    API_SetErrorCode(&Error[i++], "回油过滤堵塞报警３", 306 , 1 );
    API_SetErrorCode(&Error[i++], "高压过滤堵塞报警１", 307 , 1 );
    API_SetErrorCode(&Error[i++], "高压过滤堵塞报警２", 308 , 1 );
    API_SetErrorCode(&Error[i++], "高压过滤堵塞报警３", 309 , 1 );
    API_SetErrorCode(&Error[i++], "举升手柄故障", 310 , 1 );
    API_SetErrorCode(&Error[i++], "保持状态行车报警", 311 , 1 );
    API_SetErrorCode(&Error[i++], "集中润滑系统故障", 312 , 1 );
    API_SetErrorCode(&Error[i++], "货箱未落下行车报警", 313 , 1 );
    API_SetErrorCode(&Error[i++], "举升阀压力低报警", 314 , 1 );
    API_SetErrorCode(&Error[i++], "踏板先导阀压力低报警", 315 , 1 );
    API_SetErrorCode(&Error[i++], "前继动阀先导压力低报警", 316 , 1 );
    API_SetErrorCode(&Error[i++], "后继动阀先导压力低报警", 317 , 1 );
    API_SetErrorCode(&Error[i++], "精虑堵塞报警", 318 , 1 );
    API_SetErrorCode(&Error[i++], "举升回油过滤器堵塞报警", 319 , 1 );
    API_SetErrorCode(&Error[i++], "油水分离器报警", 320 , 1 );
    API_SetErrorCode(&Error[i++], "转向蓄能器预冲压力低报警１", 351 , 1 );
    API_SetErrorCode(&Error[i++], "转向蓄能器预冲压力低报警２", 352 , 1 );
    API_SetErrorCode(&Error[i++], "转向蓄能器预冲压力低报警３", 353 , 1 );
    API_SetErrorCode(&Error[i++], "转向蓄能器压力低报警", 354 , 0 );
    API_SetErrorCode(&Error[i++], "转向阀压力低报警", 355 , 0 );
    API_SetErrorCode(&Error[i++], "制动蓄能器（预冲）压力低报警１", 356 , 1 );
    API_SetErrorCode(&Error[i++], "制动蓄能器（预冲）压力低报警２", 357 , 1 );
    API_SetErrorCode(&Error[i++], "制动蓄能器（预冲）压力低报警３", 358 , 1 );
    API_SetErrorCode(&Error[i++], "液压油温高报警", 359 , 1 );
    API_SetErrorCode(&Error[i++], "装载制动压力低报警", 360 , 1 );
    API_SetErrorCode(&Error[i++], "前桥制动蓄能器压力低报警", 361 , 0 );
    API_SetErrorCode(&Error[i++], "后桥制动蓄能器压力低报警", 362 , 0 );
    API_SetErrorCode(&Error[i++], "前制动冷却温度低报警", 363 , 1 );
    API_SetErrorCode(&Error[i++], "后制动冷却温度低报警", 364 , 1 );
    API_SetErrorCode(&Error[i++], "前制动冷却温度高报警", 365 , 1 );
    API_SetErrorCode(&Error[i++], "后制动冷却温度高报警", 366 , 1 );
    API_SetErrorCode(&Error[i++], "转向压力低报警", 367 , 1 );
    API_SetErrorCode(&Error[i++], "制动梭阀压力低报警", 368 , 1 );
    API_SetErrorCode(&Error[i++], "制动油温低于０度报警", 369 , 1 );
    API_SetErrorCode(&Error[i++], "缓速器油温高报警", 370 , 1 );
    API_SetErrorCode(&Error[i++], "转向压力低开关报警", 371 , 1 );
    API_SetErrorCode(&Error[i++], "转向回油过滤器堵塞报警", 372 , 1 );
    API_SetErrorCode(&Error[i++], "转向油温高开关报警", 373 , 1 );
    API_SetErrorCode(&Error[i++], "湿盘制动油温高开关报警", 374 , 1 );
    API_SetErrorCode(&Error[i++], "驻车制动压力低报警", 381 , 0 );
    API_SetErrorCode(&Error[i++], "后桥刹车盘温度高报警（左）", 382 , 0 );
    API_SetErrorCode(&Error[i++], "后桥刹车盘温度高报警（右）", 383 , 0 );
    API_SetErrorCode(&Error[i++], "前桥制动压力低报警", 384 , 0 );
    API_SetErrorCode(&Error[i++], "后桥制动压力低报警", 385 , 0 );
}

void FUNC_ErrorCode::API_SetErrorStatus(ErrorTypedef *Error, bool status)
{

    SaveErrorToDB(Error,status);
    Error->ErrorStatus = status;

}

void FUNC_ErrorCode::API_SetErrorStatus(uint16_t ErrorCode, bool status)
{  
    SaveErrorToDB(&Error[API_GetErrorIndex(ErrorCode)],status);
    Error[API_GetErrorIndex(ErrorCode)].ErrorStatus = status;

}


void FUNC_ErrorCode::API_CycleToBit(long long var)
{
   uint8_t data[8];
   uint8_t TotalNumber,CurrentNumer;
   uint16_t CODE;
   bool Flag = false;
   static uint16_t ErrorCurrentTemp[MaxErrorNumber] ;
   U64ToChar(var,data);
//   qDebug()<<data[0]<<data[1]<<data[2]<<data[3]<<data[4]<<data[5]<<data[6]<<data[7];
   TotalNumber = data[0];
   CurrentNumer = data[1];//从1开始
   CODE = data[3] + data[4]*0x100;
//   qDebug()<<"CODE ="<<CODE;
   if(TotalNumber == 0 || TotalNumber> MaxErrorNumber)
   {
       for(int i = 0 ; i < MaxErrorNumber ; i++)
       {
            API_SetErrorStatus(&Error[i],0);
       }
       memset(ErrorCurrentTemp,0,sizeof (ErrorCurrentTemp));
   }
   else
   {
       if(CurrentNumer==0)return;
       ErrorCurrentTemp[CurrentNumer - 1 ] = CODE;
//       for(int i = 0 ; i < 5 ; i++)
//       {
//            qDebug()<<ErrorCurrentTemp[i];
//       }
       if(CurrentNumer == TotalNumber)
       {    for(int i = 0 ; i < TotalNumber;i++)
            {
              if(ErrorCurrentTemp[i] == 0 ) return;
            }
            for(int i = 0 ; i < MaxErrorNumber; i++)
            {
               Flag = false;
               for(int j = 0 ; j< TotalNumber; j++)
               {
                 if(Error[i].ErrorCode ==ErrorCurrentTemp[j])
                 {
                    Flag = true;
                    break;
                 }
               }
               API_SetErrorStatus(Error[i].ErrorCode,Flag);
             }
            memset(ErrorCurrentTemp,0,sizeof (ErrorCurrentTemp));
          }
        }
}
/*获取现行故障*/
uint16_t *FUNC_ErrorCode::API_GetCurrentError(unsigned short& count)
{
    memset(RealTimeError,0,sizeof (RealTimeError));
    int j = 0;
    for(int i = 0 ; i < MaxErrorNumber ; i++)
    {
        if(Error[i].ErrorStatus)
        {
            RealTimeError[j] = Error[i].ErrorCode;
            j++;
        }
    }
    count = j;
    return RealTimeError;
}

QVariantList FUNC_ErrorCode::getCurrentErrorInfo2(){
    QList<QVariant> list;
    unsigned short count;
    API_GetCurrentError(count);
    for(int i=0;i<MaxErrorNumber;i++){
        QList<QVariant> strList;
        if(RealTimeError[i]){
            int index = API_GetErrorIndex(RealTimeError[i]);
            QString strName;
            strName = QString("%1").arg(Error[index].ErrorInfor)+QString::number(Error[index].ErrorClass);
            strList<<strName;
            list.append(QVariant(strList));
        }
    }

    return list;
}

QVariantList FUNC_ErrorCode::getCurrentErrorInfo(){
     QList<QVariant> list;
     unsigned short count;
     API_GetCurrentError(count);
     for(int i=0;i<MaxErrorNumber;i++){
         QList<QVariant> strList;
         if(RealTimeError[i]){
             int index = API_GetErrorIndex(RealTimeError[i]);
             QString strName;
//             qDebug()<<Error[index].ErrorClass<<"\t"<<Error[index].ErrorStatus;
             if(!Error[index].ErrorClass)
                 strName = QString("<html> \
                                    <head> \
                                    <style> \
                                    font{color:red} \
                                    </style> \
                                    </head> \
                                    <body> \
                                    <font>%1</font> \
                                    </body> \
                                    </html>").arg(Error[index].ErrorInfor);
             else
                strName = QString("<html> \
                                   <head> \
                                   <style> \
                                   font{color:yellow} \
                                   </style> \
                                   </head> \
                                   <body> \
                                   <font>%1</font> \
                                   </body> \
                                   </html>").arg(Error[index].ErrorInfor);
             strList<<strName;
             list.append(QVariant(strList));
         }
     }

     return list;
}


/*获得当前故障数量*/
uint16_t FUNC_ErrorCode::API_GetCurrentErrorNumber()
{
    int j = 0;
    for(int i = 0 ; i < MaxErrorNumber ; i++)
    {
        if(Error[i].ErrorStatus)
        {
            j++;
        }
    }
    return j;
}



void FUNC_ErrorCode::U64ToChar(long long var, uint8_t *data)
{
    memset(data,0,8);
    memcpy(data,&var,sizeof(var));
}

/*打开数据库*/




int FUNC_ErrorCode::API_GetErrorIndex(uint16_t code)
{
    for(int i = 0 ; i < MaxErrorNumber ; i++)
    {
        if(Error[i].ErrorCode == code )
        {
            return i;
        }
    }
    return -1;
}



bool FUNC_ErrorCode::API_OpenLocalDB(void)
{
    bool Succeed;
//    qDebug()<<"AA";
    if(!M_LocalDB) M_LocalDB = new QSqlDatabase;
//     qDebug()<<M_LocalDB;
//     qDebug()<<"CC";
    if (!M_LocalDB->open())
    {
//        qDebug()<<"A";
        /*添加数据*/
        *M_LocalDB = QSqlDatabase::addDatabase("QSQLITE", "HistoryInfo");
//        qDebug()<<"B";
        /*关联数据库*/
        M_LocalDB->setDatabaseName("/apps/HistoryInfo.db");
//        qDebug()<<"C";
        /*开启*/
        Succeed = M_LocalDB->open();
//        qDebug()<<"D"<<Succeed;
        /*返回值*/
        return Succeed;
    }
    return false;
}


void FUNC_ErrorCode::API_CloseLocalDB(void)
{
    if (M_LocalDB->isOpen())
    {
        /*关闭*/
        M_LocalDB->close();
        QString name = M_LocalDB->connectionName();
        delete M_LocalDB;
        M_LocalDB = 0;
        QSqlDatabase::removeDatabase(name);
    }
}

void FUNC_ErrorCode::SaveErrorToDB(ErrorTypedef *Error, bool status)
{
    if(!Error->ErrorStatus && status)
    {
        QString CodeStr = QString::number(Error->ErrorCode);
        QDateTime dateTime = QDateTime::currentDateTime();
        QString   DateTimeStr =  dateTime.toString("yyyy-MM-dd hh:mm");
        QString NameStr="";
        QStringList list;
        list<<DateTimeStr<<CodeStr<<NameStr;
        QVariant var = list;
//        qDebug()<<"list"<<list;
        AddErrorToDB(var);
        QQQQ++;
//        qDebug()<<"QQQQ"<<QQQQ;
    }
}

void FUNC_ErrorCode::AddErrorToDB(QVariant var)
{

    QStringList list = var.toStringList();
    if(list[0]==""||list[1]=="" || list.size() == 0)  return;
    /*关联数据库*/
//    qDebug()<<list;
    bool a = API_OpenLocalDB();
    if(!a) return;
//    qDebug()<<"DB_SUCCEED"<<a;
    /*数据库事物管理*/
    M_LocalDB->transaction();
    /*预处理操作*/
    QSqlQuery Query(*M_LocalDB);
    Query.prepare("insert into HistoryErrorInfo (dateTime,code) values (:dateTime,:code)");
    Query.bindValue(":dateTime",list[0]);
    Query.bindValue(":code",list[1]);

    //Query.bindValue(":name",list[2]);
    Query.exec();
    M_LocalDB->commit();
    API_CloseLocalDB();
}





QVariantList FUNC_ErrorCode::API_GetHistoryErrorFromDB(void)
{
    QList<QVariant> list;
    list.clear();
    QString sql;
    /*打开数据库*/
    API_OpenLocalDB();
    /*查询数据库*/
    QSqlQuery Query(*M_LocalDB);
    /*数据库文件*/
    sql = "select dateTime, code from HistoryErrorInfo order by dateTime desc";
     /*执行命令*/
//    qDebug()<<"AA";

    int a = Query.exec(sql);
    if( a<= 0 ) return list;
    while (Query.next())
    {
        QList<QVariant> strList;
        QVariant strName;
        QVariant strDateTime= Query.value(0);
        QVariant strCode = Query.value(1);  
        if(strCode != "")
        {
            int Index = API_GetErrorIndex(strCode.toUInt());
            if (Index == -1) strName = "";
            else{
                strName = Error[Index].ErrorInfor+QString::number(Error[Index].ErrorClass);
            }
        }
        else
        {
            strName = "";
        }

        strList<<strDateTime<<strCode<<strName;
        list.append(QVariant(strList));
    }
    API_CloseLocalDB();
    return list;
}



QVariantList FUNC_ErrorCode::API_GetCurrentErrorList()
{
    QVariantList  RealTimeErrorList ;
    RealTimeErrorList.clear();
    /*故障列表*/
    for(int i =0;i<MaxErrorNumber;i++)
    {
         if(Error[i].ErrorStatus)
         {
           QVariantList strList;
           strList<<QString::number(Error[i].ErrorCode)<<Error[i].ErrorInfor;
           RealTimeErrorList.append(QVariant(strList));
         }
    }
    return RealTimeErrorList;
}


//void AddErrorToDB(QList<QVariantList> var)
//{
//    /*关联数据库*/
//    bool a = API_OpenLocalDB();
//    /*数据库事物管理*/
//    M_LocalDB->transaction();
//    /*预处理操作*/
//    QSqlQuery Query(*M_LocalDB);
//    Query.prepare("insert into HistoryErrorInfo (dateTime,code) values (:dateTime,:code)");
//    for(int i=0;i<var.size();i++){

//        Query.bindValue(":dateTime",var[i][0]);
//        Query.bindValue(":code",var[i][1]);
//        Query.exec();
//    }

//    M_LocalDB->commit();
//    API_CloseLocalDB();
//}






