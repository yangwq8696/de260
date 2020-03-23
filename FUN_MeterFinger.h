#ifndef FUN_MeterFinger_H
#define FUN_MeterFinger_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
/**
 * 要求：
 *      背景图片是一个正方形的表盘，图片的宽度是指针长度的二倍
 *      背景图片的宽度(像素)必须是指针长度(不是指针图片的长度)的2倍
 *      如果图片大小固定，就设置指针的长度为图片长度的一半，如果指针长度固定就设置图片的宽度为指针的二倍  图片必须是正方形
 *      指针的长度不能超过指针图片的长度
 */
class FUN_MeterFinger : public QGraphicsView
{
    Q_OBJECT

public:
    FUN_MeterFinger(QString imagePath, const QPixmap &pix, qreal x, qreal y, qreal radius, QWidget *parent = 0,qreal rLen = 0);
    ~FUN_MeterFinger();
    qreal setFingerAngle(qreal angle);

    void setText(QString str);
protected:
    QGraphicsPixmapItem *createFinger(const QPixmap &pix, qreal width, qreal height, qreal scale);
    QGraphicsPixmapItem* createBackgroundTable();
    QGraphicsTextItem* createTextItem();


    QRect scale(QRect source, QRect target,qreal& scal);

private:
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem * m_needle1;
    QGraphicsPixmapItem * m_backgroundTable;
    QGraphicsTextItem* m_textItem;

    qreal m_radius;
    qreal m_rLen;
    QString m_backGround;
};
#endif // FUN_MeterFinger_H
