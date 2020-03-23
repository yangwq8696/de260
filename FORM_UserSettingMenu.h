#ifndef FORM_USERSETTINGMENU_H
#define FORM_USERSETTINGMENU_H

#include <QWidget>
#include "page.h"
#include "FUN_ButtonBar.h"

namespace Ui {
class FORM_UserSettingMenu;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_UserSettingMenu : public Page
{
    Q_OBJECT

public:
    explicit FORM_UserSettingMenu(Page *parent = nullptr);
    ~FORM_UserSettingMenu();
    void showForm();
    void closeForm();
private slots:
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();

protected:
    void paintEvent(QPaintEvent *event);      //绘图事件
private:
    void createFigner();

private:
    Ui::FORM_UserSettingMenu *ui;
    void selextDisplay();
    unsigned int menuIndex=0;
    int i=0;
    bool m_uiInited;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_USERSETTINGMENU_H
