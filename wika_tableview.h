#ifndef WIKA_TABLEVIEW_H
#define WIKA_TABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QPixmap>

class wika_ItemDelegate;
class QScrollBar;
class wika_tableview : public QTableView
{
    Q_OBJECT
public:
    explicit wika_tableview(QWidget *parent = 0);
signals:
    //左键双击信号，qt提供的双击信号不区分左右键
    void LButtonDBClicked(const QModelIndex index);

public slots:

public:
    //从配置文件中读取表头名称
    bool LoadHeaderFromFile(const QString& strFilePath);
    //根据索引设置行或列标题
    void SetHeaderByIndex(Qt::Orientation orientation,int iIndex,const QString& strText);
    //根据索引设置绝对值行高或列宽
    bool SetHeaderAbsWidthByIndex(Qt::Orientation orientation, int iIndex, int iWidth);
    //根据索引设置百分比行高或列宽   iWidth--百分比
    bool SetHeaderWidthByIndex(Qt::Orientation orientation,int iIndex,int iWidth);


    //设置横向或纵向标题
    void SetHeaders(Qt::Orientation orientation,const QStringList &strlistHeaderText);
    //设置表头每列宽度（整数百分比，比如第一列占表页50%宽度，则填50，只有在百分比模式下有效
    void SetHeaderWidth(Qt::Orientation orientation,const QList<int> &listHeaderWidth);


    //设置表头显隐
    void SetHeaderVisible(Qt::Orientation orientation,bool bVisible);
    //设置某列或末行
    void SetHeaderItemVisible(Qt::Orientation orientation,int iIndex,bool bVisible);

    //设置表头留空，有时为了显示滚动条，所以设置表头空出来一小块 默认垂直滚动条留空10像素，只对相对大小有效
    void SetHeaderOffset(Qt::Orientation orientation,int iOffset);


    //设置单元格数据
    void SetItemData(int row, int column, Qt::ItemDataRole role, const QVariant & value);
    //获取单元格数据
    QVariant ItemData(int row,int column, Qt::ItemDataRole role);
    //清除部分表数据 iDataCount：保留的行数，如果为零则为全清
    void ClearTable(int iDataCount);
    //清空单元格内容，但是不删除行
    void ClearContents();


    void InsertRow(int iRow);//在iRow位置插入新行
    void RemoveRow(int iRow);//删除索引为iRow的行
    void SetRowCount(int iCount);//设置行数
    int GetRowCount();//获取行数
    void SetColumnCount(int iCount);//设置列数
    int GetColumnCount();//获取列数
    //获得数据模型（不建议直接操作数据模型）
    QStandardItemModel* GetDataModel();
    wika_ItemDelegate* GetDelegate();
    //设置滚动条的滚动效果 down为true时，下移， step为下移的步数
    void SetRolling(bool down,int step);

    QColor GetColor(int a,int b,int c);
    void SetText(int row,int column,QString strText);   //设置单元格的显示文本
    void SetFont(int row,int column,QFont qfont);       //设置单元格的字体
    void SetBackClor(int row,int column,QColor qclor);  //设置单元格的背景颜色
    void SetForeClor(int row,int column,QColor qclor);  //设置单元格的前景色
    void SetTextAlignment(int row,int column,Qt::AlignmentFlag alignmentFlg); //设置单元格显示文本的居中方式
    int GetSelectedRow(); //设置当前选中行
    QModelIndex GetSelectedIndex(); //设置当前选中单元格索引

protected:
    void ResizeColumn(int iTableWidth); //设置列宽 iTableWidth  -- 表的宽度
    void ResizeRow(int iTableHeight);   //设置行高 iTableHeight -- 表的高度


protected:
    virtual void resizeEvent(QResizeEvent *event);           //重载resize事件
    virtual void mouseDoubleClickEvent(QMouseEvent *event);  //重载双击事件
//    virtual void wheelEvent(QWheelEvent *event);

private:
    QStringList m_strlistHorHeaderText;    //水平表头文字
    QList<int> m_listHorHeaderWidth;       //百分占比列宽（相对）
    QList<int> m_listHorHeaderAbsWidth;    //绝对列宽   （绝对）

    QStringList m_strlistVerHeaderText;    //垂直表头文字
    QList<int> m_listVerHeaderWidth;       //垂直表头宽度（相对）
    QList<int> m_listVerHeaderAbsWidth;    //垂直表头宽度（绝对）
    //偏移量(防止滚动条遮挡数据)
    int m_iWidthOffset;   //横向表头留空
    int m_iHeightOffset;  //纵向表头留空
private:
    QStandardItemModel m_model;
    wika_ItemDelegate* m_delegate;
};

class wika_ItemDelegate:public QItemDelegate
{
    Q_OBJECT
    public:
        wika_ItemDelegate(QObject * parent=0);
        virtual ~ wika_ItemDelegate(){}


        void setRowIndex(int index){m_rowIndex = index;}
        void setDurationColumn(int index){durationColumn = index;}
        //渲染数据项背景
        void paint(QPainter * painter,
                const QStyleOptionViewItem & option,
                const QModelIndex & index) const;

        //创建时间编辑器，在编辑结束时，发出信号通知视图用被编辑的数据替换已经存在的数据
        //发出信号，通知视图已经不需要这个编辑器了，这时模型将会把它删除
        //如果用户按下Esc键，视图将会简单地删除编辑器
        QWidget *createEditor(QWidget *parent,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
        //设置编辑器的值
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        //使用编辑器的值更新模型中的值
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;

    private slots:
        //自定义 提交和关闭 槽函数
        void commitAndCloseEditor();

    private:
        QPixmap favouritePixmap;
        QPixmap notFavouritePixmap;

        int m_rowIndex;
        int durationColumn;  //渲染列索引（指定那一列显示时间编辑框）

};



#endif // WIKA_TABLEVIEW_H
