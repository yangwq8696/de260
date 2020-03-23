#ifndef FORM_GE_H
#define FORM_GE_H

#include <QWidget>
#include "page.h"
#include <QPainter>

namespace Ui {
class Form_GE;
}
class wika_GeneralWidget;
class Form_GE : public Page
{
    Q_OBJECT

public:
    explicit Form_GE(QWidget *parent = nullptr);
    ~Form_GE();

    void showForm();
    void closeForm();
private slots:
    void onCanRecv(int code, msg_can_t *m);
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();

protected:
    void paintEvent(QPaintEvent *event);      //绘图事件

private:
    Ui::Form_GE *ui;
    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_GE_H
