#ifndef FORM_WEIGHQUERY_H
#define FORM_WEIGHQUERY_H
#include <QWidget>
#include "page.h"

namespace Ui {
class FORM_WeighQuery;
}

class wika_GeneralWidget;
class FORM_WeighQuery : public Page
{
    Q_OBJECT

public:
    explicit FORM_WeighQuery(Page *parent = nullptr);
    ~FORM_WeighQuery();

    void closeForm();
    void showForm();
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
    Ui::FORM_WeighQuery *ui;
    bool m_uiInited;

    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_WEIGHQUERY_H
