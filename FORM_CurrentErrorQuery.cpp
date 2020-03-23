#include "FORM_CurrentErrorQuery.h"
#include "ui_FORM_CurrentErrorQuery.h"
#include <QPainter>
#include "wika_GeneralWidget.h"
#include "FUN_MeterFinger.h"
#include "main.h"
#include "QScrollBar"

FORM_CurrentErrorQuery::FORM_CurrentErrorQuery(QWidget *parent) :
    Page(parent),
    m_timer(0),
    ui(new Ui::FORM_CurrentErrorQuery)
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

FORM_CurrentErrorQuery::~FORM_CurrentErrorQuery()
{
    delete ui;
}

void FORM_CurrentErrorQuery::showForm()
{
    ui->retranslateUi(this);
    if (m_timer){ delete m_timer; m_timer=0;}
    m_timer = new QTimer();
    m_timer->stop();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(currentErrorDisplay()));
    m_timer->start(1000);

    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QVariantList CurrentErrorInfoList = Func_ErrorCode->getCurrentErrorInfo2();
    tableInfoInit();
    table->SetRowCount(0);
    showCurrentErrorInfo(CurrentErrorInfoList);

    showFullScreen();
}
void FORM_CurrentErrorQuery::closeForm()
{
    this->hide();
}

void FORM_CurrentErrorQuery:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}

void FORM_CurrentErrorQuery::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}

void FORM_CurrentErrorQuery::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}

void FORM_CurrentErrorQuery::onRelease_Button2()
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
void FORM_CurrentErrorQuery::onRelease_Button3()
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
void FORM_CurrentErrorQuery::onRelease_Button4()
{
    table->SetRowCount(0);

    QVariantList CurrentErrorInfoList = Func_ErrorCode->getCurrentErrorInfo2();
    showCurrentErrorInfo(CurrentErrorInfoList);

}
void FORM_CurrentErrorQuery::onRelease_Button5()
{
    if(SysManageMenu == 0)
        SysManageMenu = new FORM_SysManageMenu;
    SysManageMenu->showForm();
    closeForm();
}

void FORM_CurrentErrorQuery::tableInfoInit(void)
{
    int8_t i;

    table = ui->tableView;
//    table->LoadHeaderFromFile(":/headers.xml");
    table->SetHeaderByIndex(Qt::Horizontal,0,QObject::tr("编号"));
    table->SetHeaderAbsWidthByIndex(Qt::Horizontal,0,100);
    table->SetHeaderByIndex(Qt::Horizontal,1,QObject::tr("当前故障"));
    table->SetHeaderAbsWidthByIndex(Qt::Horizontal,1,898);
    table->SetHeaderVisible(Qt::Vertical,false);       //隐藏行标题
//    table->SetHeaderItemVisible(Qt::Horizontal,1,false);
}

void FORM_CurrentErrorQuery::showCurrentErrorInfo(QVariantList valList){

    foreach(QVariant variant,valList){
        QList<QString> list = variant.toStringList();

        int num = table->GetRowCount();
        table->InsertRow(num);
        table->SetItemData(num,0,Qt::DisplayRole,num);
        table->SetTextAlignment(num,0,Qt::AlignCenter);
//        table->SetItemData(num,1,Qt::DisplayRole,list[0]);
        QString info = list[0];
        QString level = info.right(1);
        info.chop(1);
        table->SetItemData(num,1,Qt::DisplayRole,info);
        table->SetTextAlignment(num,1,Qt::AlignCenter);
        if(level.toInt())//黄色故障
            table->SetForeClor(num,1,Qt::yellow);
        else //红色故障
            table->SetForeClor(num,1,Qt::red);
    }

    if(!valList.size())
        table->InsertRow(0);
}

void FORM_CurrentErrorQuery::currentErrorDisplay(){
    table->SetRowCount(0);

    QVariantList CurrentErrorInfoList = Func_ErrorCode->getCurrentErrorInfo2();
    showCurrentErrorInfo(CurrentErrorInfoList);
}
