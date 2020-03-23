#ifndef FORM_CONTROLPORT4_H
#define FORM_CONTROLPORT4_H

#include <QWidget>
#include "page.h"
#include "FUN_ButtonBar.h"

namespace Ui {
class FORM_ControlPort4;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_ControlPort4 : public Page
{
    Q_OBJECT

public:
    explicit FORM_ControlPort4(Page *parent = nullptr);
    ~FORM_ControlPort4();
    void showForm();
    void closeForm();
private slots:
    void onCanRecv(int code, msg_can_t *m) override;
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
    Ui::FORM_ControlPort4 *ui;
    FUN_ButtonBar *buttonBar;
    void loadFunctionKey(void);
    bool m_uiInited;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_CONTROLPORT4_H
