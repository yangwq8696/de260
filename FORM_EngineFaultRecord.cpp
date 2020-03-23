#include "FORM_EngineFaultRecord.h"
#include "ui_FORM_EngineFaultRecord.h"
#include "wika_GeneralWidget.h"
#include "main.h"
#include <QPixmap>
#include <QPainter>
#include <QDebug>

/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] =
{
    { C1_R18FECA00B18,   0xffffffff,     1,       0   },
    { C1_R18ECFF00B18,   0xffffffff,     1,       0   },
    { C1_R18EBFF00B18,   0xffffffff,     1,       0   },
};


FORM_EngineFaultRecord::FORM_EngineFaultRecord(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_EngineFaultRecord),
    m_uiInited(false)
{
    ui->setupUi(this);

    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);

//    canActive(CAN1_DEV, varList, N(varList));

    recordList.clear();

    ui->tableView->setEnabled(false);
    ui->tableView->LoadHeaderFromFile(":/xml/errorCode_headers.xml");
    ui->tableView->SetHeaderVisible(Qt::Vertical,false);         //隐藏行标题
    ui->tableView->SetHeaderVisible(Qt::Horizontal,false);       //
    ui->tableView->InsertRow(0);



    //EC
    data_EC[8]={0};
    m_countEC = 0;
    DM_Number = 0;//故障数量
    Pack_Number = 0;//报文数量
    _18ECFF00_BYTE5 = 0;

    //EB
    data_EB[8]={0};
    m_countEB = 0;
    DM_Data[80]={0};
    memset(data_EC_Temp,0,8);
    count = 0;
}

FORM_EngineFaultRecord::~FORM_EngineFaultRecord()
{
    delete ui;
}
/********************************************************************/
void FORM_EngineFaultRecord::showForm()
{
    ui->retranslateUi(this);
    show();
    canActive(CAN1_DEV, varList, N(varList));
}
void FORM_EngineFaultRecord::closeForm()
{
    this->hide();
//    m_GeneralWidget->ReleaseResources();
    setAttribute(Qt::WA_DeleteOnClose,1);
    canDeactive();
}
/********************************************************************/
void FORM_EngineFaultRecord::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_EngineFaultRecord::onRelease_Button2()
{
    if(!ui->tableView->GetRowCount())
        return;

//    table = ui->tableView;
    int step = ui->tableView->getCurrentRowCount();
    if(step>0)
        ui->tableView->SetRolling(false,step,Qt::Vertical);

}
void FORM_EngineFaultRecord::onRelease_Button3()
{
    if(!ui->tableView->GetRowCount())
        return;

//    table = ui->tableView;
    int step = ui->tableView->getCurrentRowCount();
    if(step>0)
        ui->tableView->SetRolling(true,step,Qt::Vertical);

}
void FORM_EngineFaultRecord::onRelease_Button4()
{

}
void FORM_EngineFaultRecord::onRelease_Button5()
{
    if(SysManageMenu == 0)
        SysManageMenu = new FORM_SysManageMenu;
    SysManageMenu->showForm();
    closeForm();
}

void FORM_EngineFaultRecord:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_EngineFaultRecord:: changeEvent(QEvent *event){
//    if(event->type() == QEvent::LanguageChange){
//        ui->retranslateUi(this);
//    }
//    QWidget::changeEvent(event);
}

void FORM_EngineFaultRecord::GetEngineFaultRecord(){
    int a = 0;
    for(int i=0;i<6;i++){
        a+=(int)ch_feca[i];
    }
    if(!a)
        return;

    int spn_singal = 0;
    uint8_t fmi_singal = 0;
    uint8_t oc_singal = 0;
    uint16_t dm_number = 0;

    if(unPackValue(ch_feca[5],7)){
        spn_singal = ch_feca[2];
        spn_singal = (spn_singal)<<8 | ch_feca[3];
        spn_singal = (spn_singal)<<3 | ch_feca[4]>>5;
    }
    else{
        spn_singal = ch_feca[2]+ch_feca[3]*0x100+((ch_feca[4])>>5)*0x10000;
    }

    fmi_singal = ch_feca[4] & 0x1F;
    oc_singal = ch_feca[5] & 0x7F;

    if(spn_singal == 0 && fmi_singal == 0)
        dm_number = 0;
    else
        dm_number = 1;

//    qDebug()<<"dm_number:   "<<dm_number;
    if(dm_number){
        engineErrorRecord record;
        record.SPN = spn_singal;
        record.FMI = fmi_singal;
        record.OC = oc_singal;
        record.CODE = Engine_Error_code(spn_singal,fmi_singal);

        recordList<<record;
    }
}

void FORM_EngineFaultRecord::DisplayErrorInfo(){
    ui->tableView->SetRowCount(0);
//    ui->tableView->InsertRow(0);
    QString strCurrentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
//    qDebug()<<recordList.size();
    foreach(engineErrorRecord error ,recordList){
        QString  code = QString::number( error.CODE);
        QString spn = QString::number( error.SPN);
        QString fmi = QString::number(error.FMI);
        QString oc = QString::number(error.OC);
//        qDebug()<<code<<" "<<spn<<" "<<fmi<<" "<<oc;

        ui->tableView->InsertRow(0);
        ui->tableView->SetItemData(0,0,Qt::DisplayRole,code);
        ui->tableView->SetTextAlignment(0,0,Qt::AlignCenter);
        ui->tableView->SetItemData(0,1,Qt::DisplayRole,spn);
        ui->tableView->SetTextAlignment(0,1,Qt::AlignCenter);
        ui->tableView->SetItemData(0,2,Qt::DisplayRole,fmi);
        ui->tableView->SetTextAlignment(0,2,Qt::AlignCenter);
        ui->tableView->SetItemData(0,3,Qt::DisplayRole,oc);
        ui->tableView->SetTextAlignment(0,3,Qt::AlignCenter);
        ui->tableView->SetItemData(0,4,Qt::DisplayRole,strCurrentTime);
        ui->tableView->SetTextAlignment(0,4,Qt::AlignCenter);

    }
//    recordList.clear();

}

void FORM_EngineFaultRecord::onCanRecv(int code, msg_can_t *m)
{
    qreal val;
    long long value;
    QString ss;
    if (code != MSG_CAN_OD)
        return;
    for (unsigned int i=0; i<N(varList); i++)
    {
        if (varList[i].var == m->var)
        {
            val = (m->val_int32 & varList[i].mask) * varList[i].scale + varList[i].offset;

            switch (m->var)
            {
            case C1_R18FECA00B18:
//                qDebug()<<"C1_R18FECA00B18";
                value = canopen_read_var_uint64(CAN1_DEV, C1_R18FECA00B18, NULL);
                changeLongLongToChar8(value,ch_feca);
                GetEngineFaultRecord();
                DisplayErrorInfo();
                break;

            case C1_R18ECFF00B18:
                value = canopen_read_var_uint64(CAN1_DEV, C1_R18ECFF00B18, NULL);
                changeLongLongToChar8(value,data_EC);

                if(data_EC[0] == 0x20 && data_EC[5] == 0xca && data_EC[6] == 0xfe && data_EC[7] == 0x00)
                {
                    EC_valueSet();
                }
                break;

            case C1_R18EBFF00B18:
                value = canopen_read_var_uint64(CAN1_DEV, C1_R18EBFF00B18, NULL);
                changeLongLongToChar8(value,data_EB);
                _EBCount++;
                if(_EBCount>Pack_Number)
                {
                    _EBCount =Pack_Number+1 ;
                    return;
                }
                EB_valueSet();
                DisplayErrorInfo();
                break;
            }
            break;
        }
    }
}

void FORM_EngineFaultRecord::EB_valueSet(){
    m_countEB=0;

    if(_18ECFF00_BYTE5 != 0xca)
        return;
    if(DM_Number <= 1 || DM_Number>=11) return;
    for(int i = 0; i < Pack_Number; i++)
    {
        if((data_EB[0] - 1) == i)
            for(int j = 1; j < 8; j++)
                DM_Data[j - 1 + 7 * i] = data_EB[j];
    }

//    qDebug()<<"ebCount"<<QString::number(_EBCount);
//    qDebug()<<"Pack_Number"<<QString::number(Pack_Number);
    if(_EBCount<Pack_Number)
    {
//        qDebug()<<"return";
//        return;
    }
    for(int i = 0; i < 80; i++)
    {
//        qDebug()<<QString::number(i)<<" is "<<QString::number((int)DM_Data[i]);
    }
    Engine_Error();
}

uint8_t FORM_EngineFaultRecord::bitest(uint8_t byte, uint8_t n)//返回byte的第n位的值
{
    uint8_t i;
    uint8_t a = 1;
    for(i = 0; i < n; i++) a = a * 2;
    return ((byte & a) >> n);
}

void FORM_EngineFaultRecord::Engine_Error()
{
    uint8_t temp[4];
    uint8_t i;//U8 buff[20];//U8 j;

    uint32_t SPN;
    uint8_t FMI;
    uint8_t OC;
    uint32_t CODE;

    if(DM_Number <= 1)
        return;
//    qDebug()<<"DM_Number is "<<QString::number(DM_Number);
    for(i = 0; i < DM_Number; i++)
    {
        temp[0] = DM_Data[i * 4 + 2];
        temp[1] = DM_Data[i * 4 + 3];
        temp[2] = DM_Data[i * 4 + 4];
        temp[3] = DM_Data[i * 4 + 5];
//        qDebug()<<"I "<<QString::number(i);
//         qDebug()<<"T0 "<<QString::number(temp[0]);
//         qDebug()<<"T1 "<<QString::number(temp[1]);
//         qDebug()<<"T2 "<<QString::number(temp[2]);
//         qDebug()<<"T3 "<<QString::number(temp[3]);
        if(bitest(temp[3],7))
        {
            SPN = temp[0];
            SPN = SPN<<8 | temp[1];
            SPN = SPN<<3 | (temp[2])>>5;
//            qDebug()<<"if spn"<<QString::number(SPN);
        }
        else
        {
            SPN = temp[0] + temp[1] * 0x100 + (temp[2] >> 5) * 0x10000;
//             qDebug()<<"ELSE spn"<<QString::number(SPN);
        }
        FMI = temp[2] & 0x1F;
        OC = temp[3] & 0x7F;
        CODE = Engine_Error_code(SPN, FMI);


        engineErrorRecord record;
        record.SPN = SPN;
        record.FMI = FMI;
        record.OC = OC;
        record.CODE = CODE;

        recordList<<record;
    }
}


void FORM_EngineFaultRecord::EC_valueSet(){
    m_countEC=0;

    bool equal = true;
    for(int i =0;i<8;i++)
    {
        if(data_EC[i]!=data_EC_Temp[i])
        {
            equal = false;
            break;
        }
    }
    if(equal)return;
    for(int i =0;i<8;i++)
    {
        data_EC_Temp[i]=data_EC[i];
    }

    _18ECFF00_BYTE5 = data_EC[5];
    if(data_EC[0] == 0x20 && data_EC[5] == 0xca && data_EC[6] == 0xfe && data_EC[7] == 0x00)
    {
        DM_Number = ((data_EC[2] * 0x100 + data_EC[1]) - 2) / 4;
        Pack_Number = data_EC[3];
        DM_Number = DM_Number < 10?DM_Number:10;
        Pack_Number = Pack_Number < 6?Pack_Number:6;
        memset(DM_Data,0,80);
        _EBCount = 0;
    }

}

uint16_t FORM_EngineFaultRecord::Engine_Error_code(uint32_t spn, uint8_t fmi)
{
    uint16_t code;
    switch(fmi)
    {
        case 0:
            switch(spn)
            {
                case 52: code = 2114; break;
                case 91: code = 148; break;
                case 97: code = 4642; break;//新增油水分离器CODE
                case 105: code = 155; break;
                case 110: code = 151; break;
                case 157: code = 449; break;
            //	case 157: code = 553; break;
            //	case 157: code = 1911; break;
                case 168: code = 442; break;
                case 174: code = 261; break;
                case 175: code = 214; break;
                case 190: code = 234; break;
                case 191: code = 349; break;
                case 1180: code = 777; break;
                case 1264: code = 555; break;
                case 1378: code = 649; break;
                default: code = 0; break;
            }
            break;
        case 1:
            switch(spn)
            {
                case 91: code = 147; break;
                case 94: code = 2216; break;
                case 98: code = 253; break;
                //case 98: code = 471; break;
                case 100: code = 143; break;
                //case 100: code = 415; break;
                case 109: code = 233; break;
                case 111: code = 235; break;
                case 157: code = 2249; break;
                case 167: code = 598; break;
                case 168: code = 441; break;
                case 191: code = 489; break;
                case 626: code = 487; break;
                case 1380: code = 219; break;
                default: code = 0; break;
            }
            break;
        case 2:
            switch(spn)
            {
                case 84: code = 241; break;
                case 91: code = 431; break;
                //case 91: code = 1242; break;
                case 93: code = 528; break;
                case 94: code = 268; break;
                case 98: code = 252; break;
                case 100: code = 435; break;
                case 102: code = 2973; break;
                case 108: code = 295; break;
                case 110: code = 334; break;
                case 111: code = 422; break;
                case 113: code = 524; break;
                case 156: code = 423; break;
                case 157: code = 554; break;
                case 166: code = 951; break;
                case 175: code = 425; break;
                case 190: code = 115; break;
                //case 190: code = 689; break;
                //case 190: code = 2321; break;
                case 251: code = 319; break;
                case 558: code = 431; break;
                case 609: code = 184; break;
                case 611: code = 523; break;
                case 612: code = 115; break;
                case 627: code = 434; break;
                //case 627: code = 1117; break;
                case 629: code = 375; break;
                case 630: code = 341; break;
                case 633: code = 468; break;
                case 635: code = 467; break;
                case 639: code = 185; break;
                //case 639: code = 426; break;
                case 644: code = 237; break;
                case 652: code = 315; break;
                case 723: code = 753; break;
                //case 723: code = 778; break;
                //case 723: code = 2322; break;
                case 1077: code = 366; break;
                case 1078: code = 369; break;
                case 1319: code = 419; break;
                case 1377: code = 497; break;
                case 1380: code = 472; break;
            //	case 1380: code = 473; break;
                case 1563: code = 1256; break;
            //	case 1563: code = 1257;	 break;
                default: code = 0; break;
            }
            break;
        case 3:
            switch(spn)
            {
                case 22: code = 719; break;
                case 29: code = 133; break;
                case 52: code = 2111; break;
                case 91: code = 131; break;
                //case 91: code = 515; break;
                case 94: code = 546; break;
                case 97: code = 428; break;
                case 100: code = 135; break;
                case 102: code = 122; break;
                case 105: code = 153; break;
                case 108: code = 221; break;
                case 109: code = 231; break;
                case 110: code = 144; break;
                case 111: code = 195; break;
                case 135: code = 118; break;
                case 156: code = 116; break;
                case 157: code = 451; break;
                case 171: code = 249; break;
                case 174: code = 263; break;
                case 175: code = 212; break;
                case 412: code = 2375; break;
                case 441: code = 293; break;
                case 620: code = 227; break;
                //case 620: code = 385; break;
                case 632: code = 255; break;
                case 633: code = 276; break;
                //case 633: code = 455; break;
                case 635: code = 113; break;
                case 647: code = 2377; break;
                case 677: code = 584; break;
                case 697: code = 2557; break;
                case 702: code = 527; break;
                case 703: code = 529; break;
                case 729: code = 2555; break;
                case 730: code = 2426; break;
                case 733: code = 166; break;
                case 931: code = 316; break;
                case 974: code = 133; break;
                case 1043: code = 283; break;
                //case 1043: code = 387; break;
                case 1072: code = 2182; break;
                case 1073: code = 2367; break;
                case 1075: code = 2265; break;
                case 1076: code = 361; break;
                case 1077: code = 373; break;
                case 1079: code = 386; break;
                case 1083: code = 293; break;
                case 1084: code = 297; break;
                case 1112: code = 2368; break;
                case 1129: code = 128; break;
                case 1136: code = 697; break;
                case 1172: code = 691; break;
                case 1209: code = 2373; break;
                case 1264: code = 719; break;
                case 1267: code = 338; break;
                case 1347: code = 272; break;
                case 1348: code = 274; break;
                case 1349: code = 483; break;
                case 1381: code = 581; break;
                case 1387: code = 297; break;
                case 1388: code = 297; break;
                case 2623: code = 1239; break;
                case 2981: code = 2115; break;
                case 3509: code = 386; break;
                case 3510: code = 227; break;
                case 3511: code = 239; break;
                case 3512: code = 2185; break;
                case 520199: code = 193; break;
                default: code = 0; break;
            }
            break;
        case 4:
            switch(spn)
            {
                case 22: code = 729; break;
                case 29: code = 134; break;
                case 52: code = 2112; break;
                case 91: code = 132; break;
            //	case 91: code = 516; break;
            //	case 91: code = 551; break;
                case 94: code = 547; break;
                case 97: code = 429; break;
                case 100: code = 141; break;
                case 102: code = 123; break;
                case 105: code = 154; break;
                case 108: code = 222; break;
                case 109: code = 232; break;
                case 110: code = 145; break;
                case 111: code = 196; break;
                case 135: code = 119; break;
                case 156: code = 117; break;
                case 157: code = 452; break;
                case 171: code = 256; break;
                case 174: code = 265; break;
                case 175: code = 213; break;
                case 412: code = 2376; break;
                case 441: code = 294; break;
                case 513: code = 243; break;
                case 558: code = 551; break;
                case 620: code = 187; break;
                case 623: code = 244; break;
                case 632: code = 254; break;
                case 633: code = 279; break;
                case 635: code = 114; break;
                case 647: code = 245; break;
                case 677: code = 585; break;
                case 697: code = 2558; break;
                case 729: code = 2556; break;
                case 730: code = 2425; break;
                case 974: code = 134; break;
                case 1043: code = 284; break;
                //case 1043: code = 443; break;
                case 1072: code = 2183; break;
                case 1073: code = 2363; break;
                case 1075: code = 2266; break;
                case 1076: code = 362; break;
                case 1077: code = 365; break;
                case 1079: code = 352; break;
                case 1083: code = 294; break;
                case 1084: code = 298; break;
                case 1112: code = 2365; break;
                case 1129: code = 129; break;
                case 1136: code = 698; break;
                case 1172: code = 692; break;
                case 1188: code = 466; break;
                case 1209: code = 2374; break;
                case 1264: code = 729; break;
                case 1265: code = 223; break;
                case 1266: code = 225; break;
                case 1267: code = 339; break;
                case 1347: code = 271; break;
                case 1348: code = 273; break;
                case 1349: code = 484; break;
                case 1381: code = 582; break;
                case 1387: code = 298; break;
                case 1388: code = 298; break;
                case 1661: code = 199; break;
                case 2623: code = 1241; break;
                case 2981: code = 2116; break;
                case 3509: code = 352; break;
                case 3510: code = 187; break;
                case 3511: code = 238; break;
                case 3512: code = 2186; break;
                case 520199: code = 194; break;
                default: code = 0; break;
            }
            break;
        case 5:
            switch(spn)
            {
                case 633: code = 378; break;
                case 635: code = 394; break;
                case 651: code = 322; break;
                case 652: code = 331; break;
                case 653: code = 324; break;
                case 654: code = 332; break;
                case 655: code = 323; break;
                case 656: code = 325; break;
                case 1244: code = 396; break;
                case 1245: code = 398; break;
                default: code = 0; break;
            }
            break;
        case 6:
            switch(spn)
            {
                case 633: code = 379; break;
                case 635: code = 395; break;
                case 638: code = 172; break;
                case 651: code = 311; break;
                case 653: code = 313; break;
                case 654: code = 321; break;
                case 655: code = 312; break;
                case 656: code = 314; break;
                case 1244: code = 397; break;
                case 1245: code = 399; break;
                default: code = 0; break;
            }
            break;
        case 7:
            switch(spn)
            {
                case 157: code = 755; break;
                case 632: code = 259; break;
                //case 633: code = 277; break;
                case 633: code = 514; break;
                case 635: code = 112; break;
                case 638: code = 173; break;
                case 651: code = 1139; break;
                case 652: code = 1141; break;
                case 653: code = 1142; break;
                case 654: code = 1143; break;
                case 655: code = 1144; break;
                case 656: code = 1145; break;
                case 723: code = 731; break;
                case 931: code = 318; break;
                case 1076: code = 363; break;
                case 1077: code = 377; break;
                case 1347: code = 275; break;
                //case 1347: code = 281; break;
                case 1348: code = 282; break;
                //case 1348: code = 328; break;
                case 1349: code = 758; break;
                default: code = 0; break;
            }
            break;
        case 8:
            switch(spn)
            {
                case 91: code = 147; break;
            //	case 91: code = 148; break;
                case 1078: code = 368; break;
                default: code = 0; break;
            }
            break;
        case 9:
            switch(spn)
            {
                case 639: code = 285; break;
                //case 639: code = 427; break;
                case 1077: code = 364; break;
                default: code = 0; break;
            }
            break;
        case 10:
            switch(spn)
            {
                case 84: code = 242; break;
                case 103: code = 2345; break;
                case 190: code = 121; break;
                default: code = 0; break;
            }
            break;
        case 11:
            switch(spn)
            {
                case 174: code = 264; break;
                case 626: code = 381; break;
                //case 626: code = 382; break;
                //case 626: code = 384; break;
                case 632: code = 391; break;
                case 633: code = 539; break;
                case 647: code = 389; break;
                case 703: code = 779; break;
                case 923: code = 768; break;
                case 1043: code = 496; break;
                case 1075: code = 278; break;
                case 1078: code = 367; break;
                case 1387: code = 2194; break;
                default: code = 0; break;
            }
            break;
        case 12:
            switch(spn)
            {
                case 627: code = 351; break;
                case 629: code = 111; break;
                //case 629: code = 343; break;
                case 630: code = 346; break;
                case 1076: code = 517; break;
                case 1077: code = 374; break;
                default: code = 0; break;
            }
            break;
        case 13:
            switch(spn)
            {
                case 91: code = 432; break;
                case 558: code = 432; break;
                case 630: code = 342; break;
                case 639: code = 286; break;
                case 1076: code = 493; break;
                case 1077: code = 376; break;
                default: code = 0; break;
            }
            break;
        case 14:
            switch(spn)
            {
                case 441: code = 292; break;
                case 640: code = 599; break;
                case 703: code = 2195; break;
                case 1077: code = 329; break;
                //case 1077: code = 372; break;
                case 1083: code = 292; break;
                case 1084: code = 296; break;
                case 1387: code = 296; break;
                case 1388: code = 296; break;
                default: code = 0; break;
            }
            break;
        case 15:
            switch(spn)
            {
                case 94: code = 2261; break;
                case 97: code = 418; break;
                case 110: code = 2963; break;
                case 2789: code = 2346; break;
                case 2790: code = 2347; break;
                default: code = 0; break;
            }
            break;
        case 16:
            switch(spn)
            {
                case 52: code = 2113; break;
                case 94: code = 449; break;
                case 95: code = 2372; break;
                case 97: code = 1852; break;
                case 102: code = 124; break;
                case 103: code = 595; break;
                case 105: code = 488; break;
                case 110: code = 146; break;
                case 157: code = 553; break;
                case 167: code = 596; break;
                case 168: code = 442; break;
                case 174: code = 261; break;
                case 191: code = 349; break;
                case 611: code = 2292; break;
                case 1129: code = 126; break;
                case 1264: code = 555; break;
                case 1349: code = 485; break;
                case 1800: code = 2263; break;
                default: code = 0; break;
            }
            break;
        case 17:
            switch(spn)
            {
                case 94: code = 2262; break;
                case 1380: code = 219; break;
                default: code = 0; break;
            }
            break;
        case 18:
            switch(spn)
            {
                case 94: code = 482; break;
                //case 94: code = 2215; break;
                case 100: code = 143; break;
                case 102: code = 125; break;
                case 103: code = 687; break;
                case 109: code = 233; break;
                case 111: code = 197; break;
                case 157: code = 559; break;
                case 167: code = 597; break;
                case 168: code = 441; break;
                case 191: code = 489; break;
                case 611: code = 2293; break;
                case 620: code = 444; break;
                case 1129: code = 127; break;
                case 1349: code = 486; break;
                case 1381: code = 583; break;
                case 1800: code = 2264; break;
                case 2981: code = 2117; break;
                default: code = 0; break;
            }
            break;
        case 19:
            switch(spn)
            {
                case 91: code = 287; break;
                case 974: code = 288; break;
                default: code = 0; break;
            }
            break;
        case 31:
            switch(spn)
            {
                case 630: code = 2217; break;
                case 633: code = 2311; break;
                case 1378: code = 649; break;
                case 1383: code = 611; break;
                case 1384: code = 299; break;
                case 1484: code = 211; break;
                case 2802: code = 757; break;
                default: code = 0; break;
            }
            break;
        default:
            code = 0;
            break;
    }
    return code;
}

void FORM_EngineFaultRecord::changeLongLongToChar8(long long var,unsigned char data[8]){
    memset(data,0,8);
    memcpy(data,&var,sizeof(var));
#if 0
    qDebug()<<QString::number(data[0],16)+" "
            <<QString::number(data[1],16)+" "
            <<QString::number(data[2],16)+" "
            <<QString::number(data[3],16)+" "
            <<QString::number(data[4],16)+" "
            <<QString::number(data[5],16)+" "
            <<QString::number(data[6],16)+" "
            <<QString::number(data[7],16)+" ";
#endif
}
