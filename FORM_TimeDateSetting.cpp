#include "FORM_TimeDateSetting.h"
#include "ui_FORM_TimeDateSetting.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"

FORM_TimeDateSetting::FORM_TimeDateSetting(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_TimeDateSetting),
    selectIndex(4),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);

//    ui->dateTimeEdit->setEnabled(false);
//    ui->dateTimeEdit_2->setEnabled(false);
}



FORM_TimeDateSetting::~FORM_TimeDateSetting()
{
    delete ui;
}
/********************************************************************/
void FORM_TimeDateSetting::showForm()
{
    ui->retranslateUi(this);
    showFullScreen();

    timeDateDisplay();
    ui->res->setText(tr(""));//clear res

    selectIndex = 4;
}
void FORM_TimeDateSetting::timeDateDisplay()
{
//    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    //qDebug() << "time";
    QDateTime curDateTime = QDateTime::currentDateTime();
    year = curDateTime.toString("yyyy").toInt();
    month = curDateTime.toString("MM").toInt()-1;
    day = curDateTime.toString("dd").toInt()-1;
    hour = curDateTime.toString("hh").toInt();
    minute = curDateTime.toString("mm").toInt();
    //qDebug() << year << "-" << month << "-" << day << "    " << hour << ":" << minute;
    dateTimeSetting();
}
void FORM_TimeDateSetting::closeForm()
{
    //delete timer01;
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);
}
/********************************************************************/
void FORM_TimeDateSetting::selectDisplay()
{
    if      (selectIndex==0){
        ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::YearSection);
        ui->dateTimeEdit_2->setSelectedSection(QDateTimeEdit::NoSection);
    }
    else if (selectIndex==1){
        ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::MonthSection);
        ui->dateTimeEdit_2->setSelectedSection(QDateTimeEdit::NoSection);
    }
    else if (selectIndex==2){
        ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::DaySection);
        ui->dateTimeEdit_2->setSelectedSection(QDateTimeEdit::NoSection);
    }
    else if (selectIndex==3){
        ui->dateTimeEdit_2->setSelectedSection(QDateTimeEdit::HourSection);
        ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::NoSection);
    }
    else if (selectIndex==4){
        ui->dateTimeEdit_2->setSelectedSection(QDateTimeEdit::MinuteSection);
        ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::NoSection);
    }
    else{
       ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::YearSection);
       ui->dateTimeEdit_2->setSelectedSection(QDateTimeEdit::NoSection);
    }
}
void FORM_TimeDateSetting::dateTimeSetting()
{
//    qDebug()<<year<<" "<<month<<" "<<day<<" "<<hour<<" "<<minute<<" ";
    QDate dateq;
    QTime timeq;
    if(month == 1 && day>=28){
        if(year%4==0)
            day=28;
        else
            day=27;
    }
    if(month==3||month==5||month==8||month==10&&day>=30)
        day=29;
//    qDebug()<<selectIndex<<"  "<<"month: "<<month+1<<"DAY:"<<day+1;


    dateq.setDate(year,month+1,day+1);
    timeq.setHMS(hour,minute,0,0);
    ui->dateTimeEdit->setDate(dateq);
    ui->dateTimeEdit_2->setTime(timeq);
    ui->res->setText(tr(""));//clear res
}
/********************************************************************/
void FORM_TimeDateSetting::onRelease_Button1()
{
    selectIndex = (selectIndex + 1) % 5;
//    qDebug()<<selectIndex;
    selectDisplay();
}
void FORM_TimeDateSetting::onRelease_Button2()
{
    if      (selectIndex==0) year++;
    else if (selectIndex==1){
        month = (month+1)%12;
    }
    else if (selectIndex==2){
        if(month ==1){
            if(year%4)
                day = (day+1)%28;
            else
                day = (day+1)%29;
        }else if(month == 0||month == 2||month == 4|| month == 6||month == 7||month == 9||month == 11)
            day = (day+1)%31;
        else
            day = (day+1)%30;
    }
    else if (selectIndex==3) hour = (hour+1)%24;
    else if (selectIndex==4) minute = (minute+1)%60;
    dateTimeSetting();

}
void FORM_TimeDateSetting::onRelease_Button3()
{
    if      (selectIndex==0) year--;
    else if (selectIndex==1){
        month = (month+11)%12;
    }
    else if (selectIndex==2){
        if(month ==1){
            if(year%4)
                day = (day+27)%28;
            else
                day = (day+28)%29;
        }else if(month == 0||month == 2||month == 4|| month == 6||month == 7||month == 9||month == 11)
            day = (day+30)%31;
        else
            day = (day+29)%30;
    }
    else if (selectIndex==3) hour = (hour+23)%24;
    else if (selectIndex==4) minute = (minute+59)%60;
    dateTimeSetting();
}
void FORM_TimeDateSetting::onRelease_Button4()
{
    int res = -1;
    QString newtime = ui->dateTimeEdit->dateTime().toString("yyyy/MM/dd")+" "+ui->dateTimeEdit_2->dateTime().toString("hh:mm");
    res = sys_set_time(newtime.toLocal8Bit().data());
    if(res==0)
    {
        ui->res->setStyleSheet("color:rgb(0,255,0)");
        ui->res->setText(tr("设置成功！"));
    }
    else
    {
        ui->res->setStyleSheet("color:rgb(255,0,0)");
        ui->res->setText(tr("设置失败！"));
    }
}
void FORM_TimeDateSetting::onRelease_Button5()
{
    if(UserSettingMenu == 0)
        UserSettingMenu = new FORM_UserSettingMenu;
    UserSettingMenu->showForm();
    closeForm();
}

void FORM_TimeDateSetting::showEvent(QShowEvent*){
    selectDisplay();
//    qDebug()<<"selectIndex:"<<selectIndex;
}

void FORM_TimeDateSetting:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01_b.png"));

}

void FORM_TimeDateSetting::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}

