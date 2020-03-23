#include "FORM_SystemSetup.h"
#include "ui_FORM_SystemSetup.h"
#include "main.h"

FORM_SystemSetup::FORM_SystemSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FORM_SystemSetup)
{
    ui->setupUi(this);
    showFullScreen();
}

FORM_SystemSetup::~FORM_SystemSetup()
{
    delete ui;
}
void FORM_SystemSetup::showForm()
{
    this->show();
}
void FORM_SystemSetup::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);//销毁当前界面所有变量
}

void FORM_SystemSetup::on_pushButton_clicked()
{
    closeForm();
    MainLmiNew->showForm();
}
void FORM_SystemSetup::keyPressEvent(QKeyEvent *event)
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
