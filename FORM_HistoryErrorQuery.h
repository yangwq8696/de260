#ifndef FORM_HISTORYERRORQUERY_H
#define FORM_HISTORYERRORQUERY_H

#include <QWidget>
#include "page.h"
#include "FUN_ButtonBar.h"
#include "wika_tableView.h"

#define DISNUMPERPAGE 15

namespace Ui {
class FORM_HistoryErrorQuery;
}


class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_HistoryErrorQuery : public Page
{
    Q_OBJECT

public:
    explicit FORM_HistoryErrorQuery(Page *parent = nullptr);
    ~FORM_HistoryErrorQuery();
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
    void createFigner(); //创建指针



private:
    Ui::FORM_HistoryErrorQuery *ui;
    wika_tableView *table;
    void selextDisplay();
    void showHistoryErrorInfo(QVariantList valList);
    int menuIndex=0;
    int i=0;
    bool m_uiInited;
    void tableInfoInit(void);
    uint8_t PageIndex;
    uint8_t PageMax;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
//    QVariantList historyErrorInfoList;
    wika_GeneralWidget* m_GeneralWidget;

};

#endif // FORM_HISTORYERRORQUERY_H
