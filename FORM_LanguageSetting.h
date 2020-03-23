#ifndef FORM_LANGUAGESETTING_H
#define FORM_LANGUAGESETTING_H

#include <QWidget>
#include "page.h"
#include "FUN_ButtonBar.h"

namespace Ui {
class FORM_LanguageSetting;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_LanguageSetting : public Page
{
    Q_OBJECT

public:
    explicit FORM_LanguageSetting(Page *parent = nullptr);
    ~FORM_LanguageSetting();
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
    void selextDisplay();

private:
    Ui::FORM_LanguageSetting *ui;
    void selectDisplay();
    int m_Index=0;
    int ico_y[3] = {205,268,334};
    int ico_x=1260;
    bool m_uiInited;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_LANGUAGESETTING_H
