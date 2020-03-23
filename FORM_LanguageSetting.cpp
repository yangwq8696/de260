#include "FORM_LanguageSetting.h"
#include "ui_FORM_LanguageSetting.h"
#include "main.h"
#include "FUN_MeterFinger.h"
#include <QPixmap>
#include <QPainter>
#include "wika_GeneralWidget.h"

FORM_LanguageSetting::FORM_LanguageSetting(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_LanguageSetting),
    m_uiInited(false)
{
    ui->setupUi(this);
//    createFigner();
    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);
}

FORM_LanguageSetting::~FORM_LanguageSetting()
{
    delete ui;
}
/********************************************************************/
void FORM_LanguageSetting::showForm()
{
    showFullScreen();
    m_Index = 0;

    uint8_t langId = LanguageZh;
    eeprom_read(EEPROM_LANGUAGE_OFFSET, &langId, EEPROM_LANGUAGE_SIZE);
    switch (langId) {
    case 0:
        m_Index = LanguageZh;
        break;
    case 1:
        m_Index = LanguageEn;
        break;
    case 2:
        m_Index = LanguageRu;
        break;
    default:
        m_Index = LanguageZh;
    }
    selectDisplay();
}
void FORM_LanguageSetting::closeForm()
{
    this->hide();
    setAttribute(Qt::WA_DeleteOnClose,1);
}
/********************************************************************/
void FORM_LanguageSetting::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_LanguageSetting::onRelease_Button2()
{
    m_Index = (m_Index + 2) % 3;
    selectDisplay();
}
void FORM_LanguageSetting::onRelease_Button3()
{
    m_Index = (m_Index + 1) % 3;
    selectDisplay();
}
void FORM_LanguageSetting::onRelease_Button4()
{
    QString strQm;
    uint8_t langId = LanguageZh;
    switch (m_Index) {
    case 0:
        langId = LanguageZh;
        eeprom_write(EEPROM_LANGUAGE_OFFSET, &langId, EEPROM_LANGUAGE_SIZE);
        m_languageId = LanguageZh;
        strQm = ":/language/LANG_zh.qm";
        break;
    case 1:
        langId = LanguageEn;
        eeprom_write(EEPROM_LANGUAGE_OFFSET, &langId, EEPROM_LANGUAGE_SIZE);
        m_languageId = LanguageEn;
        strQm = ":/language/LANG_en.qm";
        break;
    case 2:
        langId = LanguageRu;
        eeprom_write(EEPROM_LANGUAGE_OFFSET, &langId, EEPROM_LANGUAGE_SIZE);
        m_languageId = LanguageRu;
        strQm = ":/language/LANG_ru.qm";
        break;
    }
    //刷新当前页面
    m_translator->load(strQm);
    qApp->installTranslator(m_translator);
    ui->retranslateUi(this);
}
void FORM_LanguageSetting::onRelease_Button5()
{
    if(UserSettingMenu == 0)
        UserSettingMenu = new FORM_UserSettingMenu;
    UserSettingMenu->showForm();
    closeForm();
}
void FORM_LanguageSetting::selectDisplay()
{
    ui->label_3->move(ico_x,ico_y[m_Index]);

    ui->info_1->setStyleSheet("color:white");
    ui->info_2->setStyleSheet("color:white");
    ui->info_3->setStyleSheet("color:white");

    if     (m_Index==0) ui->info_1->setStyleSheet("color:red");
    else if(m_Index==1) ui->info_2->setStyleSheet("color:red");
    else if(m_Index==2) ui->info_3->setStyleSheet("color:red");
}

void FORM_LanguageSetting:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));

}
void FORM_LanguageSetting::createFigner(){
    m_finger1 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_05.png"),-54.8,734.3,428,this);
    m_finger1->raise();
    m_finger1->setFingerAngle(-50.6); //-69

    m_finger2 = new FUN_MeterFinger("",QPixmap(":/pic_de260/figner_06.png"),180.5,484.6,264,this);
    m_finger2->setFingerAngle(-32.3); //-59.7
}

void FORM_LanguageSetting::selextDisplay()
{
//    qDebug()<<"test  begin";
    ui->info_1->setStyleSheet("color:white");
    ui->info_2->setStyleSheet("color:white");
    ui->info_3->setStyleSheet("color:white");

    if     (m_Index==0) ui->info_1->setStyleSheet("color:red");
    else if(m_Index==1) ui->info_2->setStyleSheet("color:red");
    else if(m_Index==2) ui->info_3->setStyleSheet("color:red");
//    qDebug()<<"test  end";

}
