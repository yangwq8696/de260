#ifndef FORM_GPSQUERY_H
#define FORM_GPSQUERY_H

#include <QWidget>
#include "page.h"
#include "FUN_ButtonBar.h"

namespace Ui {
class FORM_GPSQuery;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_GPSQuery : public Page
{
    Q_OBJECT

public:
    explicit FORM_GPSQuery(Page *parent = nullptr);
    ~FORM_GPSQuery();
    void showForm();
    void closeForm();
private slots:
    void onCanRecv(int code, msg_can_t *m);
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();
    void GPSstateDisplay();

protected:
    void paintEvent(QPaintEvent *event);      //绘图事件
private:
    void createFigner();
    QString creatHtmlText(qreal val,char*p,int num=0);


private:
    Ui::FORM_GPSQuery *ui;
    bool m_uiInited;

    int m_powerState;
    int m_lineState;
    int m_SIMState;
    QString m_EW;
    QString m_SN;
    QString m_Altitude;
    int m_satelliteNum;
    int m_degree_jing;
    int m_min_jing;
    int m_second1_jing;
    int m_second2_jing;
    int m_degree_wei;
    int m_min_wei;
    int m_second1_wei;
    int m_second2_wei;

    bool m_bEnabled;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;

    QTimer *m_timer;
    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_GPSQUERY_H
