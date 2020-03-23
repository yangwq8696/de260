#ifndef FORM_LIGHTSETTING_H
#define FORM_LIGHTSETTING_H

#include <QWidget>
#include "page.h"
#include "FUN_ButtonBar.h"

namespace Ui {
class FORM_LightSetting;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_LightSetting : public Page
{
    Q_OBJECT

public:
    explicit FORM_LightSetting(Page *parent = nullptr);
    ~FORM_LightSetting();
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
    int DelayStaticstics();

private:
    Ui::FORM_LightSetting *ui;
    uint8_t lightvalue;
    bool m_uiInited;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;

    QTime t_F2_Previous;
};

#endif // FORM_LIGHTSETTING_H
