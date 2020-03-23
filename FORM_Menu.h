#ifndef FORM_MENU_H
#define FORM_MENU_H

#include <QWidget>
#include "page.h"
#include "FUN_ButtonBar.h"

namespace Ui {
class FORM_Menu;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_Menu : public Page
{
    Q_OBJECT

public:
    explicit FORM_Menu(Page *parent = nullptr);
    ~FORM_Menu() override;
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
    Ui::FORM_Menu *ui;
    FUN_ButtonBar *buttonBar;
    void selextDisplay();
    void loadFunctionKey(void);
    int menuIndex=0;
    int i=0;
    bool m_uiInited;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_MENU_H
