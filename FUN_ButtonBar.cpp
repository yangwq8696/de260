#include "FUN_ButtonBar.h"
#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>
#include "main.h"

FUN_ButtonBar::FUN_ButtonBar(const QPixmap &pix1,const QPixmap &pix2,const QPixmap &pix3,const QPixmap &pix4,const QPixmap &pix5,QWidget *parent):
    QGraphicsView(parent),
    m_scene(0)
{
    delete item1;
    delete item2;
    delete item3;
    delete item4;
    delete item5;
    delete m_scene;
    delete View;
    QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem(pix1);
    QGraphicsPixmapItem *item2 = new QGraphicsPixmapItem(pix2);
    QGraphicsPixmapItem *item3 = new QGraphicsPixmapItem(pix3);
    QGraphicsPixmapItem *item4 = new QGraphicsPixmapItem(pix4);
    QGraphicsPixmapItem *item5 = new QGraphicsPixmapItem(pix5);

    m_scene = new QGraphicsScene(this);
    m_scene->clear();

    item1->setPos(0,-720);
    item2->setPos(0,-600);
    item3->setPos(0,-480);
    item4->setPos(0,-360);
    item5->setPos(0,-240);
    m_scene->addItem(item1);
    m_scene->addItem(item2);
    m_scene->addItem(item3);
    m_scene->addItem(item4);
    m_scene->addItem(item5);

    setScene(m_scene);
    setGeometry(1820,0,100,720);
    setStyleSheet("padding:0px;border:0px;background:transparent");
    setAttribute(Qt::WA_TranslucentBackground, true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
