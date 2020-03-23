#include "FUN_MeterFinger.h"
#include <QDebug>

/**
 * @brief FUN_MeterFinger::FUN_MeterFinger
 * @param pix  指针图片
 * @param x    指针旋转点在父窗口中位置的x轴坐标
 * @param y    指针旋转点在父窗口中位置的y轴坐标
 * @param radius 指针长度
 * @param parent 父窗口
 */
FUN_MeterFinger::FUN_MeterFinger(QString imagePath, const QPixmap &pix, qreal x, qreal y, qreal radius, QWidget *parent,qreal rLen):
    QGraphicsView(parent),
    m_backGround(imagePath),
    m_scene(0),
    m_needle1(0),
    m_radius(radius),
    m_rLen(rLen)
{
    if(!radius){
        radius = pix.width();
        m_radius = radius;
    }
    qreal scale = radius / pix.width();        //缩放比
    qreal n1Height = pix.height() * scale;     //缩放后的高度
    qreal n1Width = radius;                    //缩放后的宽度

    /*
     * 将指针加入动态绘制区域
     * 默认指针放置方向是"--->"
     */
    m_needle1 = createFinger(pix, n1Width, n1Height, scale); //创建指针图形项
    m_backgroundTable = createBackgroundTable();
    m_textItem = createTextItem();

    m_scene = new QGraphicsScene(this);
    m_scene->clear();
    m_scene->setSceneRect(0, 0, radius * 2, radius * 2);
    m_scene->addItem(m_needle1);
    m_scene->addItem(m_backgroundTable);
    m_scene->addItem(m_textItem);
//    m_scene->setBackgroundBrush(QPixmap(m_backGround));
    setScene(m_scene);
    /*
     * 设置绘图框属性：
     * 	透明，无边框
     */
    setGeometry(x - radius, y - radius, radius * 2, radius * 2);
    setStyleSheet("padding:0px;border:0px;background:transparent");
    setAttribute(Qt::WA_TranslucentBackground, true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
FUN_MeterFinger::~FUN_MeterFinger()
{
}
QGraphicsPixmapItem *FUN_MeterFinger::createFinger(const QPixmap &pix, qreal sw, qreal sh, qreal scale)
{
    //
    //创建图形项
    //

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pix);
    item->setCacheMode(QGraphicsItem::NoCache); //
    //item->setOpacity(0.8);
    item->setZValue(1);//设置层叠顺序
    item->setTransformationMode(Qt::SmoothTransformation);

    //
    //设置指针的旋转点，然后移动图形项使旋转点位于视图中心，最后按比例缩放图形项
    //

    //默认旋转点(0,0)在图形项的左上角
    //设置旋转点下移到垂直居中位置，右移m_rLen/scale
    item->setTransformOriginPoint(m_rLen/scale,sh/2/scale);
    //设置图形项的旋转点位于视图中心
    item->setPos(m_radius - m_rLen/scale, m_radius - sh/2/scale);
    //缩放图形项
    item->setScale(scale);//缩放之后，图形项的位置不变，图形项的真实大小也不变

    return item;
}

qreal FUN_MeterFinger::setFingerAngle(qreal angle)
{
    m_needle1->setRotation(angle);
    return angle;
}

///
/// \brief scale    将图像等比例缩放,使图像在目标矩形中居中显示。返回图像在目标矩形中的位置
/// \param source   图像实际大小
/// \param target   目标矩形
/// \return
///
QRect FUN_MeterFinger::scale(QRect source, QRect target,qreal& scal)
{
    int target_w = target.width();
    int target_h = target.height();

    int image_w = source.width();
    int image_h = source.height();

    if(image_w <= 0) image_w = 1;
    if(image_h <= 0) image_h = 1;

    //
    //计算缩放后的宽度和高度
    //

    //按宽度缩放
    int scaled_w = target_w;
    int scaled_h = image_h * target_w / image_w;
    scal = (float)target_w / image_w;
    //如果按宽度缩放后，超出了目标区域，则按高度缩放
    if(scaled_h > target_h)
    {
        scaled_h = target_h;
        scaled_w = image_w * target_h / image_h;
        scal = (float)target_h / image_h;
    }


    //
    //设置居中显示（计算左上角的位移）
    //
    int off_x = (target_w - scaled_w)/2;
    int off_y = (target_h - scaled_h)/2;
    return QRect(target.left() + off_x,  target.top()+ off_y, scaled_w, scaled_h);
}

QGraphicsPixmapItem* FUN_MeterFinger::createBackgroundTable(){
    QImage image;
    image.load(m_backGround);  //加载图片
    QSize imageSize = image.size();
    int imageWidth = imageSize.width();
    int imageHeight = imageSize.height();

    QPixmap backGroundTable = QPixmap(m_backGround);

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(backGroundTable);
    item->setCacheMode(QGraphicsItem::NoCache); //
    item->setZValue(0);//设置层叠顺序
    item->setTransformationMode(Qt::SmoothTransformation);

    qreal scal = 0;
    QRect rect = scale(QRect(0,0,imageWidth,imageHeight),QRect(0,0,m_radius*2,m_radius*2),scal);
//    qDebug()<<rect;
//    qDebug()<<geometry();
//    qDebug()<<scal;

    item->setScale(scal);
    item->setPos(rect.x(),rect.y());

    return item;
}

QGraphicsTextItem* FUN_MeterFinger::createTextItem(){
    QGraphicsTextItem* item = new QGraphicsTextItem;
    item->setZValue(2);
    qreal x = m_radius-25;
    qreal y = m_radius+m_radius*1/2;
//    item->setTextWidth(40);
    item->adjustSize();
    item->setPlainText("");
//    item->setHtml("<b>100<\b>");
    item->setDefaultTextColor(QColor(196, 160, 0));

    item->setPos(x,y);

    return item;
}

void FUN_MeterFinger::setText(QString str){
    m_textItem->setHtml(str);
}
