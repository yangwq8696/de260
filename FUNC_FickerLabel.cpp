#include "FUNC_FickerLabel.h"

FUNC_FickerLabel::FUNC_FickerLabel(QLabel*label,QObject *parent) :
    QObject(parent),
    m_label(label)
{
    m_b = false;
    m_Timer=0;
    m_Timer = new QTimer();
    connect(m_Timer, SIGNAL(timeout()), this,SLOT(blink()));
}
FUNC_FickerLabel::~FUNC_FickerLabel()
{
    if (m_Timer)
        delete m_Timer;
}

void FUNC_FickerLabel::setFicker(int onTime,int interval){
    this->m_onTime = onTime;
    this->m_interval = interval;
    blink(); //启动闪烁
}

void FUNC_FickerLabel::resetFicker(){
    m_Timer->stop();
    m_label->setVisible(false);
}

void FUNC_FickerLabel::blink(){
    m_Timer->stop();
    if(m_b){
        m_label->setVisible(true);
        m_Timer->start(m_onTime);
    }else{
        m_label->setVisible(false);
        m_Timer->start(m_interval);
    }
    m_b = !m_b;
}

