#ifndef FUN_BUTTONBAR_H
#define FUN_BUTTONBAR_H

#include <QtWidgets/QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

typedef void (*FUNC)(void);

class FUN_ButtonBar: public QGraphicsView
{
    Q_OBJECT
public:
    FUN_ButtonBar(const QPixmap &pix1,const QPixmap &pix2,const QPixmap &pix3,const QPixmap &pix4,const QPixmap &pix5,QWidget *parent);


signals:

public slots:

private:
    QGraphicsScene *m_scene;
    QGraphicsView *View;
    QGraphicsPixmapItem *item1;
    QGraphicsPixmapItem *item2;
    QGraphicsPixmapItem *item3;
    QGraphicsPixmapItem *item4;
    QGraphicsPixmapItem *item5;
    //int i=0;
    //FUNC keyRelease[5];
};

#endif // FUN_BUTTONBAR_H
