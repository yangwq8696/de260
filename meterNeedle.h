#ifndef METERNEEDLE_H
#define METERNEEDLE_H

#include <QtWidgets/QGraphicsView>
#include <QGraphicsPixmapItem>

class meterNeedle : public QGraphicsView
{
    Q_OBJECT

public:
    meterNeedle(const QPixmap &pix, const QPixmap &pix2, qreal x, qreal y, qreal radius, QWidget *parent = nullptr);
    ~meterNeedle() override;
    qreal setAngle1(qreal angle);
    qreal setAngle2(qreal angle);

protected:
    //void paintEvent(QPaintEvent *event) override;
    QGraphicsPixmapItem *createNeedle(const QPixmap &pix, qreal width, qreal height, qreal scale);

private:
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_needle1;
    QGraphicsPixmapItem *m_needle2;

    qreal m_lastAngle1;
    qreal m_lastAngle2;
    qreal m_radius;
};

#endif // METERNEEDLE_H
