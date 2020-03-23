#ifndef WIKA_GENERALWIDGET_H
#define WIKA_GENERALWIDGET_H

#include <QWidget>
#include "page.h"


namespace Ui {
class wika_GeneralWidget;
}

class FUN_MeterFinger;
class wika_Label;
class QTimer;
class wika_GeneralWidget : public Page
{
    Q_OBJECT

public:
    explicit wika_GeneralWidget(Page *parent = 0);
    ~wika_GeneralWidget();
    void setSpeed(QString strSpeed);
    void setRad(QString strRad);
    void setMileage(QString strMileage);
    void setAngle1(qreal angle);
    void setAngle2(qreal angle);


    void setTime(QString strTime);
    void setDate(QString strDate);
private:
    void createFinger();
protected slots:
    void onCanRecv(int code, msg_can_t *m);
    void setTimeDate();
//protected:
//    void paintEvent(QPaintEvent *event);      //绘图事件
//    void showEvent(QShowEvent *);
private:
    Ui::wika_GeneralWidget *ui;
    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_Label* m_labelSpeed;   //车速
    wika_Label* m_labelRad;     //转速
    wika_Label* m_labelTime;
    wika_Label* m_labelDate;
    wika_Label* m_mileage;      //里程
    QTimer* m_timer;
};

#endif // WIKA_GENERALWIDGET_H
