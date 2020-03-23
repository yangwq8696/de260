#ifndef WIKA_TABLEVIEW_H
#define WIKA_TABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QPixmap>
#include <QAbstractItemView>

class wika_ItemDelegate;
class QScrollBar;
class wika_tableView : public QTableView
{
    Q_OBJECT
public:
    explicit wika_tableView(QWidget *parent = 0);
signals:
    void LButtonDBClicked(const QModelIndex index);

public slots:

public:
    bool LoadHeaderFromFile(const QString& strFilePath);
    void SetHeaderByIndex(Qt::Orientation orientation,int iIndex,const QString& strText);
    bool SetHeaderAbsWidthByIndex(Qt::Orientation orientation, int iIndex, int iWidth);
    bool SetHeaderWidthByIndex(Qt::Orientation orientation,int iIndex,int iWidth);


    void SetHeaders(Qt::Orientation orientation,const QStringList &strlistHeaderText);
    void SetHeaderWidth(Qt::Orientation orientation,const QList<int> &listHeaderWidth);


    void SetHeaderVisible(Qt::Orientation orientation,bool bVisible);
    void SetHeaderItemVisible(Qt::Orientation orientation,int iIndex,bool bVisible);

    void SetHeaderOffset(Qt::Orientation orientation,int iOffset);


    void SetItemData(int row, int column, Qt::ItemDataRole role, const QVariant & value);
    QVariant ItemData(int row,int column, Qt::ItemDataRole role);
    void ClearTable(int iDataCount);
    void ClearContents();


    void InsertRow(int iRow);
    void RemoveRow(int iRow);
    void SetRowCount(int iCount);
    int GetRowCount();
    void SetColumnCount(int iCount);
    int GetColumnCount();
    QStandardItemModel* GetDataModel();
    wika_ItemDelegate* GetDelegate();

    int getCurrentRowCount();
    int getCurrentColumnCount();
    void SetRolling(bool downOrRight,int step,Qt::Orientation orientation = Qt::Vertical);

    QColor GetColor(int a,int b,int c);
    void SetText(int row,int column,QString strText);
    void SetFont(int row,int column,QFont qfont);
    void SetBackClor(int row,int column,QColor qclor);
    void SetForeClor(int row,int column,QColor qclor);
    void SetTextAlignment(int row,int column,Qt::AlignmentFlag alignmentFlg);
    int GetSelectedRow();
    QModelIndex GetSelectedIndex();
    void setSelectedItems(int topLeft_x,int topLeft_y,int bottomRight_x,int bottomRight_y);

    void setStateMap(int column);


protected:
    void ResizeColumn(int iTableWidth);
    void ResizeRow(int iTableHeight);

private slots:
      void updateSectionWidth(int logicalIndex,int, int newSize);
      void updateSectionHeight(int logicalIndex, int, int newSize);


protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible);
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
//    virtual void wheelEvent(QWheelEvent *event);

public:
    void FrozenFirstColumn();
    void redo_FrozenFirstColumn();
private:
    void updateFrozenTableGeometry();

private:
    QStringList m_strlistHorHeaderText;
    QList<int> m_listHorHeaderWidth;
    QList<int> m_listHorHeaderAbsWidth;

    QStringList m_strlistVerHeaderText;
    QList<int> m_listVerHeaderWidth;
    QList<int> m_listVerHeaderAbsWidth;
    int m_iWidthOffset;
    int m_iHeightOffset;
private:
    QStandardItemModel m_model;
    wika_ItemDelegate* m_delegate;
    QTableView *frozenTableView;
    bool bFrozened;

};

class wika_ItemDelegate:public QItemDelegate
{
    Q_OBJECT
    public:
        wika_ItemDelegate(QObject * parent=0);
        virtual ~ wika_ItemDelegate(){}


        void setRowIndex(int index){m_rowIndex = index;}
        void setDurationColumn(int index){durationColumn = index;}
        void paint(QPainter * painter,
                const QStyleOptionViewItem & option,
                const QModelIndex & index) const;


        QWidget *createEditor(QWidget *parent,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;

    private slots:
        void commitAndCloseEditor();

    private:
        QPixmap favouritePixmap;
        QPixmap notFavouritePixmap;

        int m_rowIndex;
        int durationColumn;
};



#endif // WIKA_TABLEVIEW_H
