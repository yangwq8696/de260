#include "FORM_ParaSetting.h"
#include "ui_FORM_ParaSetting.h"
#include "main.h"

FORM_ParaSetting::FORM_ParaSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FORM_ParaSetting)
{
    ui->setupUi(this);
    showFullScreen();
}
FORM_ParaSetting::~FORM_ParaSetting()
{
    delete ui;
}
void FORM_ParaSetting::showForm()
{
    this->show();
}
void FORM_ParaSetting::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);//销毁当前界面所有变量
}

void FORM_ParaSetting::on_pushButton_clicked()
{

}
void FORM_ParaSetting::keyPressEvent(QKeyEvent *event)
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
