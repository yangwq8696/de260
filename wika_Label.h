#ifndef WIKA_LABEL_H
#define WIKA_LABEL_H

#include <QWidget>

class
        wika_Label : public QWidget
{
    Q_OBJECT
public:
    explicit wika_Label(int width,int height, QWidget *parent = 0);

    void setAngel(qreal angle){m_angle_rotate = angle;update();}
    void setText(QString str){m_text = str;update();}
    void setFontSize(int size){m_font.setPointSize(size);update();}
    void setFont(QFont font){m_font = font;update();}
    void setColor(QColor color){m_color = color;update();}

signals:

protected:
    void paintEvent(QPaintEvent *event);      //绘图事件

    qreal m_angle_rotate;
    QString m_text;
    int m_width;
    int m_height;
    QFont m_font;
    QColor m_color;

public slots:
};

#endif // WIKA_LABEL_H
