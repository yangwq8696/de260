#include "wika_Label.h"
#include <QPainter>

wika_Label::wika_Label(int width,int height,QWidget *parent) :
    QWidget(parent),
    m_text(""),
    m_width(width),
    m_height(height)
{
    resize(m_width,m_height);

    m_angle_rotate = 0;


    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(12);
    font.setItalic(true);
    m_font = font;

    m_color = Qt::white;
}


void wika_Label:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);

    int h = height();
    int w = width();
    painter.save();
    painter.translate(QPointF(w/2,h/2));
    painter.rotate(m_angle_rotate);


//    QFont font;
//    font.setFamily("Microsoft YaHei");
//    font.setPointSize(m_fontSize);
    painter.setFont(m_font);
    QFontMetrics fm(m_font);
    QRect rec = fm.boundingRect(m_text);
    int textWidth = rec.width();
    int textHeight = rec.height();

    int x = -textWidth/2;
    int y = -textHeight/2;
    QRectF rectf = QRectF(x,y,textWidth,textHeight);


    QPen pen(m_color,2);
    painter.setPen(pen);
    painter.drawText(rectf,Qt::AlignHCenter,m_text);

    painter.restore();
}
