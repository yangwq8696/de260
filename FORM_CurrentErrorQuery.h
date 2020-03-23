#ifndef FORM_CURRENTERRORQUERY_H
#define FORM_CURRENTERRORQUERY_H

#include <QWidget>
#include "page.h"
#include "wika_tableView.h"

namespace Ui {
class FORM_CurrentErrorQuery;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_CurrentErrorQuery : public Page
{
    Q_OBJECT
public:
    explicit FORM_CurrentErrorQuery(QWidget *parent = nullptr);
    ~FORM_CurrentErrorQuery();

    void showForm();
    void closeForm();
protected:
    void paintEvent(QPaintEvent *event);      //绘图事件
private:
    void createFigner(); //创建指针

signals:

private slots:
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();

    void currentErrorDisplay();

private:
    Ui::FORM_CurrentErrorQuery *ui;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;

    wika_tableView *table;
    int menuIndex=0;
    int i=0;
    bool m_uiInited;
    void tableInfoInit(void);
    void showCurrentErrorInfo(QVariantList valList);
    uint8_t PageIndex;
    uint8_t PageMax;

    QTimer *m_timer;
};

#endif // FORM_CURRENTERRORQUERY_H
