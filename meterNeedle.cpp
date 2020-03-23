#include "meterNeedle.h"
#include <QDebug>

meterNeedle::meterNeedle(const QPixmap &pix, const QPixmap &pix2, qreal x, qreal y, qreal radius, QWidget *parent):
    QGraphicsView(parent),
    m_scene(0),
    m_needle1(0),
    m_needle2(0),
    m_lastAngle1(0),
    m_lastAngle2(0),
    m_radius(0)
{
    qreal scale = radius / pix.width();
    qreal n1Height = pix.height() * scale;
    qreal n1Width = radius;
    qreal n2Height = pix2.height() * scale;
    qreal n2Width = pix2.width() * scale;

    //qDebug() << "scale=" << scale << ", Needle1=" << (int)n1Width << (int)n1Height << ", Needle2=" << (int)n2Width << (int)n2Height;
    /*
     * 将指针加入动态绘制区域
     * 默认指针放置方向是"--->"
     * 注意：这里假定 pix和pix2的宽度相同
     */
    m_radius = radius;
    m_needle1 = createNeedle(pix, n1Width, n1Height, scale);
    m_needle2 = createNeedle(pix2, n2Width * 0.8, n2Height, scale * 0.84);

    m_scene = new QGraphicsScene(this);
    m_scene->clear();
    m_scene->setSceneRect(0, 0, radius * 2, radius * 2);
    m_scene->addItem(m_needle1);
    m_scene->addItem(m_needle2);
    setScene(m_scene);

	/*
	 * 设置绘图框属性：
	 * 	透明，无边框
	 */
    setGeometry(x - radius, y - radius, radius * 2, radius * 2);
    setStyleSheet("padding:0px;border:0px");
    setAttribute(Qt::WA_TranslucentBackground, true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

meterNeedle::~meterNeedle()
{
    if (m_scene)
        delete m_scene;

    if (m_needle1)
        delete m_needle1;
    if (m_needle2)
        delete m_needle2;
}

QGraphicsPixmapItem *meterNeedle::createNeedle(const QPixmap &pix, qreal sw, qreal sh, qreal scale)
{
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pix);
    /*
     * 设置动态绘图的方法
     */
    item->setCacheMode(QGraphicsItem::NoCache);
    //item->setOpacity(0.8);
    item->setZValue(1);
    item->setTransformationMode(Qt::SmoothTransformation);

    /*
     * 初始化指针变量、大小、缩放、位置
     * //在指针90%处为原点进行旋转（10%针尾，90%针头）
     */
    item->setScale(scale);
    item->setPos(m_radius*2 - sw, m_radius - sh/2);
    item->setTransformOriginPoint(sw - m_radius, sh/2);

    return item;
}

qreal meterNeedle::setAngle1(qreal angle)
{
    //if (ang < 0 || ang > 360)
    //    return -1;

    if (qAbs(angle - m_lastAngle1) < 0.5)
        return angle;

    m_needle1->setRotation(angle);
    //m_scene->update();

    m_lastAngle1 = angle;
    return angle;
}

qreal meterNeedle::setAngle2(qreal angle)
{
    //if (ang < 0 || ang > 360)
    //    return -1;

    if (qAbs(angle - m_lastAngle2) < 0.5)
        return angle;

    m_needle2->setRotation(angle);
    //m_scene->update();

    m_lastAngle2 = angle;
    return angle;
}

#if 0
void meterNeedle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this->viewport());
    QPixmap pix(":/meter/needle.png");
    pix = pix.scaled(277, 22, Qt::KeepAspectRatio);
    qreal scale = 272 / pix.width();
    qreal len = pix.width() * scale;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);

    painter.rotate(m_ang1);
    painter.drawPixmap(0, 0, pix);

    painter.rotate(m_ang2);
    painter.drawPixmap(0, 0, pix);

    QGraphicsView::paintEvent(event);
}
#endif
