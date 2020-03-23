#include "FORM_PortQuery.h"
#include "ui_FORM_PortQuery.h"
#include "main.h"

FORM_PortQuery::FORM_PortQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FORM_PortQuery)
{

}
FORM_PortQuery::~FORM_PortQuery()
{
    delete ui;
}

void FORM_PortQuery::showForm()
{
    ui->setupUi(this);
    showFullScreen();
    this->show();
}
void FORM_PortQuery::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);//销毁当前界面所有变量
}
void FORM_PortQuery::on_pushButton_clicked()
{
    closeForm();
    //ParaSetting->show();//销毁当前界面所有变量
}
void FORM_PortQuery::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_F1:
        on_pushButton_clicked();
        break;
    case Qt::Key_F2:
        //on_btnJ1939_clicked();
        break;
    case Qt::Key_F3:
        //on_btnNet_clicked();
        break;
    case Qt::Key_F4:
        //on_btnSteering_clicked();
        break;
    case Qt::Key_F5:
        //on_btnMain_clicked();
        break;
    default:
        //Page::keyPressEvent(event);
        break;
    }
}
