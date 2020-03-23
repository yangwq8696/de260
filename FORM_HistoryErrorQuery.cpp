#include "FORM_HistoryErrorQuery.h"
#include "ui_FORM_HistoryErrorQuery.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"
#include <QScrollBar>



FORM_HistoryErrorQuery::FORM_HistoryErrorQuery(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_HistoryErrorQuery),
    m_uiInited(false),
    PageIndex(0),
    PageMax(0)
{

    ui->setupUi(this);

    ui->tableView->setEnabled(false);
    ui->tableView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
                                                      "background:#38a0d8;"
                                                      "padding-top:20px;"
                                                      "padding-bottom:20px;"
                                                      "padding-left:3px;"
                                                      "padding-right:3px;"
                                                      "border-left:1px solid #d7d7d7;}"
                                                      "QScrollBar::handle:vertical{"
                                                      "background:#550000;"
                                                      "border-radius:6px;"
                                                      "min-height:80px;}"
                                                      "QScrollBar::handle:vertical:hover{"
                                                      "background:#d0d0d0;}"
                                                      );

    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_HistoryErrorQuery::~FORM_HistoryErrorQuery()
{
    delete ui;
}

void FORM_HistoryErrorQuery::showForm()
{
    ui->retranslateUi(this);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QVariantList historyErrorInfoList = Func_ErrorCode->API_GetHistoryErrorFromDB();
    tableInfoInit();
    table->SetRowCount(0);
    showHistoryErrorInfo(historyErrorInfoList);

    showFullScreen();
}
void FORM_HistoryErrorQuery::closeForm()
{
    this->hide();
}
/********************************************************************/
void FORM_HistoryErrorQuery::tableInfoInit(void)
{
    int8_t i;

    table = ui->tableView;
//    table->LoadHeaderFromFile(":/xml/headers.xml");
    table->SetHeaderByIndex(Qt::Horizontal,0,"");
    table->SetHeaderAbsWidthByIndex(Qt::Horizontal,0,2);
    table->SetHeaderByIndex(Qt::Horizontal,1,QObject::tr("编号"));
    table->SetHeaderAbsWidthByIndex(Qt::Horizontal,1,80);
    table->SetHeaderByIndex(Qt::Horizontal,2,QObject::tr("故障时间"));
    table->SetHeaderAbsWidthByIndex(Qt::Horizontal,2,250);
    table->SetHeaderByIndex(Qt::Horizontal,3,QObject::tr("故障信息"));
    table->SetHeaderAbsWidthByIndex(Qt::Horizontal,3,668);

    table->SetHeaderVisible(Qt::Vertical,false);       //隐藏行标题
    table->SetHeaderItemVisible(Qt::Horizontal,0,false);
}
/********************************************************************/
void FORM_HistoryErrorQuery::showHistoryErrorInfo(QVariantList valList){

    foreach(QVariant variant,valList){
        QList<QString> list = variant.toStringList();

        int num = table->GetRowCount();
        table->InsertRow(num);
        table->SetItemData(num,1,Qt::DisplayRole,list[1]);
//        qDebug()<<__func__<<__LINE__<<list[1];
        table->SetItemData(num,2,Qt::DisplayRole,list[0]);
        QString info = list[2];
        QString level = info.right(1);
        info.chop(1);
        table->SetItemData(num,3,Qt::DisplayRole,info);
        if(level.toInt())//黄色故障
            table->SetForeClor(num,3,Qt::yellow);
        else //红色故障
            table->SetForeClor(num,3,Qt::red);
    }

    if(!valList.size())
        table->InsertRow(0);
}

void FORM_HistoryErrorQuery::selextDisplay()
{

}
void FORM_HistoryErrorQuery::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_HistoryErrorQuery::onRelease_Button2()
{
//    PageIndex = (PageIndex+(PageMax-1)) % PageMax;
//    selextDisplay();
    if(!table->GetRowCount())
        return;

    table = ui->tableView;
    int step = table->getCurrentRowCount();
//    qDebug()<<"F2"<<step;
    if(step>0)
        table->SetRolling(false,step,Qt::Vertical);
}
void FORM_HistoryErrorQuery::onRelease_Button3()
{
//    PageIndex = (PageIndex+1) % PageMax;
//    selextDisplay();
    if(!table->GetRowCount())
        return;

    table = ui->tableView;
    int step = table->getCurrentRowCount();
//    qDebug()<<"F3"<<step;
    if(step>0)
        table->SetRolling(true,step,Qt::Vertical);
}
void FORM_HistoryErrorQuery::onRelease_Button4()
{
    table->SetRowCount(0);
//    QVariantList historyErrorInfoList = GetErrorInfo->getHistoryErrorInfoFromDB();
    QVariantList historyErrorInfoList = Func_ErrorCode->API_GetHistoryErrorFromDB();
    showHistoryErrorInfo(historyErrorInfoList);

//    QVariantList listf = GetErrorInfo->getCurrentErrorInfo();
//    qDebug()<<listf.size();
}
void FORM_HistoryErrorQuery::onRelease_Button5()
{
    if(SysManageMenu == 0)
        SysManageMenu = new FORM_SysManageMenu;
    SysManageMenu->showForm();
    closeForm();
}

void FORM_HistoryErrorQuery:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_HistoryErrorQuery::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}

