#ifndef FUNC_FICKERLABEL_H
#define FUNC_FICKERLABEL_H

#include <QObject>
#include <QTimer>
#include <QLabel>

class FUNC_FickerLabel : public QObject
{
    Q_OBJECT
public:
    explicit FUNC_FickerLabel(QLabel*label,QObject *parent = nullptr);
    ~FUNC_FickerLabel();

    void setFicker(int onTime,int interval);
    void resetFicker();

signals:

public slots:
    void blink();

private:
    QLabel* m_label;

    QTimer *m_Timer;
    int m_onTime;
    int m_interval;
    bool m_b;
};

#endif // FUNC_FICKERLABEL_H
