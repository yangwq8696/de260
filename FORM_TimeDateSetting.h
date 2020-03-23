#ifndef FORM_TIMEDATESETTING_H
#define FORM_TIMEDATESETTING_H

#include <QWidget>
#include <QDateTimeEdit>
#include "page.h"
#include "FUN_ButtonBar.h"

#define SEC_DT QDateTimeEdit::Sections

namespace Ui {
class FORM_TimeDateSetting;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_TimeDateSetting : public Page
{
    Q_OBJECT

public:
    explicit FORM_TimeDateSetting(Page *parent = nullptr);
    ~FORM_TimeDateSetting();
    void showForm();
    void closeForm();
private slots:
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();
    void timeDateDisplay();

protected:
    void paintEvent(QPaintEvent *event);      //绘图事件
    void showEvent(QShowEvent*);
private:
    void createFigner();

private:
    Ui::FORM_TimeDateSetting *ui;
    QTimer *timer01;
    int selectIndex;
    unsigned int year,month,day,hour,minute;
    void dateTimeSetting();
    bool m_uiInited;
    void selectDisplay();

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;

};

#endif // FORM_TIMEDATESETTING_H
