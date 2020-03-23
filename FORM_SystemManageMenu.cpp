#include "FORM_SystemManageMenu.h"
#include "ui_FORM_SystemSettingMenu.h"
#include "main.h"

FORM_SystemSettingMenu::FORM_SystemSettingMenu(Page *parent) :
    Page(parent),
    ui(new Ui::FORM_SystemSettingMenu),
    m_uiInited(false)
{

}

FORM_SystemSettingMenu::~FORM_SystemSettingMenu()
{
    delete ui;
}
/********************************************************************/
void FORM_SystemSettingMenu::showForm()
{
    ui->retranslateUi(this);
    if (!m_uiInited) {
        ui->setupUi(this);
        m_uiInited = true;
    }
    this->show();
    selextDisplay();
}
void FORM_SystemSettingMenu::closeForm()
{
    this->hide();
}
/********************************************************************/
/********************************************************************/
void FORM_SystemSettingMenu::selextDisplay()
{
    ui->menu_1->setStyleSheet("color:white");
    ui->menu_2->setStyleSheet("color:white");
    ui->menu_3->setStyleSheet("color:white");
    ui->menu_4->setStyleSheet("color:white");

    if     (menuIndex==0) ui->menu_1->setStyleSheet("color:red");
    else if(menuIndex==1) ui->menu_2->setStyleSheet("color:red");
    else if(menuIndex==2) ui->menu_3->setStyleSheet("color:red");
    else if(menuIndex==3) ui->menu_4->setStyleSheet("color:red");
}
void FORM_SystemSettingMenu::onRelease_Button1()
{
    MainLmiNew->showForm();
    closeForm();
}
void FORM_SystemSettingMenu::onRelease_Button2()
{
    menuIndex = (menuIndex + 3) % 4;
    selextDisplay();
}
void FORM_SystemSettingMenu::onRelease_Button3()
{
    menuIndex = (menuIndex + 1) % 4;
    selextDisplay();
}
void FORM_SystemSettingMenu::onRelease_Button4()
{
    switch(menuIndex)
    {
        case 0: LightSetting->showForm(); break;
        case 1: LanguageSetting->showForm(); break;
        case 2: TimeDateSetting->showForm(); break;
        case 3: ConsoleSetting->showForm(); break;
    }
    closeForm();
}
void FORM_SystemSettingMenu::onRelease_Button5()
{
    Menu->showForm();
    closeForm();
}
