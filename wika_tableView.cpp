#include "wika_tableView.h"
#include <QHeaderView>
#include <QXmlStreamReader>
#include <QFile>
//#include <QtDebug>
#include <QResizeEvent>
#include <QPainter>
#include <QTimeEdit>
#include <QScrollBar>
#include <QDebug>

wika_tableView::wika_tableView(QWidget* parent):
    QTableView(parent)
{
    this->setModel(&m_model);
    m_iWidthOffset = 0;
    m_iHeightOffset= 0;

    m_delegate = new wika_ItemDelegate(this);
    this->setItemDelegate(m_delegate);

    bFrozened=false;

}

void wika_tableView::SetHeaders(Qt::Orientation orientation,const QStringList &strlistHeaderText){

    if (orientation == Qt::Horizontal)
    {
        m_strlistHorHeaderText = strlistHeaderText;
        m_model.setColumnCount(strlistHeaderText.count());
        m_model.setHorizontalHeaderLabels(strlistHeaderText);
    }
    else if (orientation == Qt::Vertical)
    {
        m_strlistVerHeaderText = strlistHeaderText;
        m_model.setRowCount(strlistHeaderText.count());
        m_model.setVerticalHeaderLabels(strlistHeaderText);
    }
    else
    {
        return;
    }
}

void wika_tableView::SetHeaderWidth(Qt::Orientation orientation,const QList<int> &listHeaderWidth){
    if (orientation == Qt::Horizontal)
    {
        m_listHorHeaderWidth.clear();
        m_listHorHeaderWidth = listHeaderWidth;

        this->ResizeColumn(this->width()- m_iWidthOffset);
    }
    else if (orientation == Qt::Vertical)
    {
        m_listVerHeaderWidth.clear();
        m_listVerHeaderWidth = listHeaderWidth;

        this->ResizeRow(this->height()-m_iHeightOffset);
    }

}

void wika_tableView::ResizeColumn(int iTableWidth){
    if (m_strlistHorHeaderText.count() == 0)
    {
        return;
    }


    int iColumnCount = m_strlistHorHeaderText.count();
    int iVisibleCount = 0;
    for (int i=0; i<iColumnCount; i++)
    {
        if (!this->horizontalHeader()->isSectionHidden(i))
        {
            ++iVisibleCount;
        }
    }


    for (int i=0; i<iColumnCount; i++)
    {
        if (this->horizontalHeader()->isSectionHidden(i))
        {
            continue;
        }

        int iRelaWidth = 0;
        int iAbsWidth = 0;

        //
        if (i < m_listHorHeaderWidth.count())
        {
            iRelaWidth = m_listHorHeaderWidth[i];
        }

        if (i < m_listHorHeaderAbsWidth.count())
        {
            iAbsWidth = m_listHorHeaderAbsWidth[i];
        }
        int iWidth = 0;
        if (iRelaWidth > 0)
        {
            iWidth = iTableWidth * iRelaWidth / 100.0;

            if (iWidth < iAbsWidth)
            {
                iWidth = iAbsWidth;
            }
        }
        else
        {
            if (iAbsWidth > 0)
            {
                iWidth = iAbsWidth;
            }
            else
            {

                iWidth = (double)iTableWidth/(double)iVisibleCount;
            }
        }

        this->setColumnWidth(i,iWidth);
    }
}

void wika_tableView::ResizeRow(int iTableHeight){

    if (m_strlistVerHeaderText.count() == 0)
    {
        return;
    }

    int iRowCount = m_strlistVerHeaderText.count();
    int iVisibleCount = 0;


    for (int i=0; i<iRowCount; i++)
    {
        if (!this->verticalHeader()->isSectionHidden(i))
        {
            ++iVisibleCount;
        }
    }

    for (int i=0; i<iRowCount; i++)
    {
        int iRelaHeight = 0;
        int iAbsHeight = 0;

        if (i < m_listVerHeaderWidth.count())
        {
            iRelaHeight = m_listVerHeaderWidth[i];
        }

        if (i < m_listVerHeaderAbsWidth.count())
        {
            iAbsHeight = m_listVerHeaderAbsWidth[i];
        }

        int iHeight = 0;
        if (iRelaHeight > 0)
        {
            iHeight = iTableHeight * iRelaHeight / 100.0;

            if (iHeight < iAbsHeight)
            {
                iHeight = iAbsHeight;
            }
        }
        else
        {
            if (iAbsHeight > 0)
            {
                iHeight = iAbsHeight;
            }
            else
            {
                iHeight = (double)iTableHeight/(double)iVisibleCount;
            }
        }

        this->setRowHeight(i,iHeight);
    }
}

void wika_tableView::scrollTo (const QModelIndex & index, ScrollHint hint){

    if(index.column()>0)
        QTableView::scrollTo(index, hint);
}
QModelIndex wika_tableView::moveCursor(CursorAction cursorAction,
                                          Qt::KeyboardModifiers modifiers)
{
      QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

      if(bFrozened){
          if(cursorAction == MoveLeft && current.column()>0
             && visualRect(current).topLeft().x() < frozenTableView->columnWidth(0) ){

                const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x()
                                     - frozenTableView->columnWidth(0);
                horizontalScrollBar()->setValue(newValue);
          }

          if (cursorAction == MoveUp && current.row() > 0
                      && visualRect(current).topLeft().y() < frozenTableView->rowHeight(1) ){
                    const int newValue = verticalScrollBar()->value() + visualRect(current).topLeft().y()
                                         - frozenTableView->rowHeight(0) - frozenTableView->rowHeight(1);
                    verticalScrollBar()->setValue(newValue);
              }
      }

      return current;
}

void wika_tableView::redo_FrozenFirstColumn()
{
    if(frozenTableView){
        disconnect(horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this,
                SLOT(updateSectionWidth(int,int,int)));
        disconnect(verticalHeader(),SIGNAL(sectionResized(int,int,int)), this,
                SLOT(updateSectionHeight(int,int,int)));
        disconnect(frozenTableView->verticalScrollBar(), SIGNAL(valueChanged(int)),
                verticalScrollBar(), SLOT(setValue(int)));
        disconnect(verticalScrollBar(), SIGNAL(valueChanged(int)),
                frozenTableView->verticalScrollBar(), SLOT(setValue(int)));
        delete frozenTableView;
        frozenTableView = NULL;
    }

    FrozenFirstColumn();


}

void wika_tableView::FrozenFirstColumn()
{
    bFrozened = true;

    frozenTableView = new QTableView(this);
    connect(horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this,
            SLOT(updateSectionWidth(int,int,int)));
    connect(verticalHeader(),SIGNAL(sectionResized(int,int,int)), this,
            SLOT(updateSectionHeight(int,int,int)));
    connect(frozenTableView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            verticalScrollBar(), SLOT(setValue(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
            frozenTableView->verticalScrollBar(), SLOT(setValue(int)));

      frozenTableView->setModel(model());
      frozenTableView->setFocusPolicy(Qt::NoFocus);
      frozenTableView->verticalHeader()->hide();
      frozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

      viewport()->stackUnder(frozenTableView); //置顶

      frozenTableView->setStyleSheet("QTableView { border: none;"
                                     "background-color: #8EDE21;"
                                     "selection-background-color: #999}"); //for demo purposes
      frozenTableView->setSelectionModel(selectionModel());
      for(int col=1; col<model()->columnCount(); col++)
            frozenTableView->setColumnHidden(col, true);

      frozenTableView->setColumnWidth(0, columnWidth(0) );

      frozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //永远不显示水平滚动条
      frozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   //永远不显示垂直滚动条
      frozenTableView->show();

      updateFrozenTableGeometry();

      //设置滚动条一次滚动一个像素
//      setHorizontalScrollMode(ScrollPerPixel);
//      setVerticalScrollMode(ScrollPerPixel);
//      frozenTableView->setVerticalScrollMode(ScrollPerPixel);


//      frozenTableView->show();
}

void wika_tableView::updateFrozenTableGeometry()
{
//   QRect rc = frozenTableView->geometry();
//   qDebug()<<rc.x()<<rc.y();
//   QRect rc2 = QRect(verticalHeader()->width()+frameWidth(),
//                     frameWidth(), columnWidth(0),
//                     viewport()->height()+horizontalHeader()->height());
//   qDebug()<<"------:"<<rc2.x()<<rc2.y()<<verticalHeader()->width()<<frameWidth();

//    qDebug()<<frameWidth();
      frozenTableView->setGeometry( verticalHeader()->width()+frameWidth(),
                                    frameWidth(), columnWidth(0),
                                    viewport()->height()+horizontalHeader()->height());
}

 bool wika_tableView::SetHeaderWidthByIndex(Qt::Orientation orientation,int iIndex,int iWidth){
     if (orientation == Qt::Horizontal)
     {
         if (iIndex >= m_listHorHeaderWidth.count())
         {
             return false;
         }

         m_listHorHeaderWidth[iIndex] = iWidth;

         this->ResizeColumn(this->width()-m_iWidthOffset);
     }
     else if (orientation == Qt::Vertical)
     {
         if (iIndex >= m_listVerHeaderWidth.count())
         {
             return false;
         }

         m_listVerHeaderWidth[iIndex] = iWidth;

         this->ResizeRow(this->height()-m_iHeightOffset);
     }

     return true;
 }


 void wika_tableView::SetHeaderOffset(Qt::Orientation orientation, int iOffset)
 {
     if (orientation == Qt::Horizontal)
     {
         m_iWidthOffset = iOffset;
     }
     else if (orientation == Qt::Vertical)
     {
         m_iHeightOffset = iOffset;
     }
 }


 void wika_tableView::SetHeaderVisible(Qt::Orientation orientation, bool bVisible)
 {
     if (orientation == Qt::Horizontal)
     {
         this->horizontalHeader()->setVisible(bVisible);
     }
     else
     {
         this->verticalHeader()->setVisible(bVisible);
     }
 }

 void wika_tableView::SetHeaderItemVisible(Qt::Orientation orientation, int iIndex, bool bVisible)
 {
     if (orientation == Qt::Horizontal)
     {
         if (bVisible)
         {
             this->horizontalHeader()->showSection(iIndex);
         }
         else
         {
             this->horizontalHeader()->hideSection(iIndex);
         }

         int iWidth = this->width()-m_iWidthOffset;
         ResizeColumn(iWidth);
     }
     else if (orientation == Qt::Vertical)
     {
         if (bVisible)
         {
             this->verticalHeader()->showSection(iIndex);
         }
         else
         {
             this->verticalHeader()->hideSection(iIndex);
         }

         int iHeight = this->height()-m_iHeightOffset;
         ResizeRow(iHeight);
     }
 }

 void wika_tableView::SetHeaderByIndex(Qt::Orientation orientation, int iIndex, const QString &strText)
 {
     if (orientation == Qt::Horizontal)
     {
         if (m_strlistHorHeaderText.count() <= iIndex)
         {
             m_strlistHorHeaderText.append("");
             m_listHorHeaderWidth.append(0);
             m_listHorHeaderAbsWidth.append(0);
         }
         m_strlistHorHeaderText[iIndex] = strText;
         m_model.setHorizontalHeaderLabels(m_strlistHorHeaderText);
     }
     else if (orientation == Qt::Vertical)
     {
         if (m_strlistVerHeaderText.count() <= iIndex)
         {
             m_strlistVerHeaderText.append("");
             m_listVerHeaderWidth.append(0);
             m_listVerHeaderAbsWidth.append(0);
         }
         m_strlistVerHeaderText[iIndex] = strText;
         m_model.setVerticalHeaderLabels(m_strlistVerHeaderText);
         m_model.setHorizontalHeaderLabels(m_strlistHorHeaderText);
     }
     else
         return;
 }
 bool wika_tableView::SetHeaderAbsWidthByIndex(Qt::Orientation orientation, int iIndex, int iWidth)
 {
     if (orientation == Qt::Horizontal)
     {
         if (iIndex >= m_listHorHeaderAbsWidth.count())
         {
             return false;
         }
         m_listHorHeaderAbsWidth[iIndex] = iWidth;
         this->ResizeColumn(this->width()-m_iWidthOffset);
     }
     else if (orientation == Qt::Vertical)
     {
         if (iIndex >= m_listVerHeaderAbsWidth.count())
         {
             return false;
         }
         m_listVerHeaderAbsWidth[iIndex] = iWidth;
         this->ResizeRow(this->height()-m_iHeightOffset);
     }
     return true;
 }


 bool wika_tableView::LoadHeaderFromFile(const QString &strFilePath)
 {
     QFile xmlFile(strFilePath);
     if (!xmlFile.open(QFile::ReadOnly | QFile::Text))
     {
         return false;
     }

     int iHorHeaderIndex = 0;
     int iVerHeaderIndex = 0;

     QXmlStreamReader xmlReader;
     xmlReader.setDevice(&xmlFile);
     xmlReader.readNext();

     while (!xmlReader.atEnd())
     {

         if (xmlReader.isStartElement())
         {
             QString strName = xmlReader.name().toString();
             if (xmlReader.name() == "Header")
             {

                 while (!xmlReader.atEnd())
                 {
                     if (xmlReader.isStartElement())
                     {
                         if (xmlReader.name() == "Item")
                         {
                             int iType = xmlReader.attributes().value("Type").toString().toInt();
                             QString strText = xmlReader.attributes().value("Text").toString();
                             int iRelaSize = xmlReader.attributes().value("RelaSize").toString().toInt();
                             int iAbsSize = xmlReader.attributes().value("AbsSize").toString().toInt();
                             bool bVisible = xmlReader.attributes().value("Visible").toString().toInt();

                             if (strText.contains("\\n"))
                             {
                                 strText.replace("\\n","\n",Qt::CaseInsensitive);
                             }
                             Qt::Orientation orientation;
                             if (iType == 0)
                             {
                                 orientation = Qt::Horizontal;
                                 this->SetHeaderByIndex(orientation,iHorHeaderIndex,strText);
                                 this->SetHeaderWidthByIndex(orientation,iHorHeaderIndex,iRelaSize);
                                 this->SetHeaderAbsWidthByIndex(orientation,iHorHeaderIndex,iAbsSize);

                                 this->SetHeaderItemVisible(orientation,iHorHeaderIndex,bVisible);
                                 ++iHorHeaderIndex;
                             }
                             else if (iType == 1)
                             {
                                 orientation = Qt::Vertical;
                                 this->SetHeaderByIndex(orientation,iVerHeaderIndex,strText);

                                 this->SetHeaderWidthByIndex(orientation,iVerHeaderIndex,iRelaSize);
                                 this->SetHeaderAbsWidthByIndex(orientation,iVerHeaderIndex,iAbsSize);
                                 this->SetHeaderItemVisible(orientation,iVerHeaderIndex,bVisible);

                                 ++iVerHeaderIndex;
                             }
                         }
                     }
                     xmlReader.readNextStartElement();
                 }
             }
             else
             {
                 return false;
             }
         }
         else
         {
             xmlReader.readNext();
         }
     }
     xmlFile.close();
     return true;
 }

 void wika_tableView::InsertRow(int iRow)
 {
     m_model.insertRow(iRow);
 }

 void wika_tableView::RemoveRow(int iRow)
 {
     m_model.removeRow(iRow);
 }
 void wika_tableView::SetItemData(int row, int column, Qt::ItemDataRole role, const QVariant &value)
 {
     QModelIndex index = m_model.index(row,column);
     if (index.isValid())
     {
         m_model.setData(index,value,role);
     }
 }

 QVariant wika_tableView::ItemData(int row, int column, Qt::ItemDataRole role)
 {
     QModelIndex index = m_model.index(row,column);
     if (index.isValid())
     {
         return m_model.data(index,role);
     }
     else
     {
         return QVariant::Invalid;
     }
 }

 void wika_tableView::setStateMap(int column)
 {
     m_delegate->setRowIndex(column);
 }

 void wika_tableView::ClearTable(int iDataCount)
 {
     int iCurRowCount = m_model.rowCount();
     if (iCurRowCount <= iDataCount)
         return;

     m_model.removeRows(iDataCount,iCurRowCount-iDataCount);
 }

// void wika_tableView::Clear(){
//     m_model.clear();
// }
 void wika_tableView::ClearContents()
 {
     int iRowCount = m_model.rowCount();
     int iColCount = m_model.columnCount();
     for (int i=0; i<iRowCount; i++)
     {
         for (int j=0; j<iColCount; j++)
         {
             if (m_model.item(i,j))
             {
                 m_model.item(i,j)->setText("");
                 m_model.item(i,j)->setData(QVariant::Invalid,Qt::DecorationRole);
             }
         }
     }
 }


 void wika_tableView::SetRowCount(int iCount)
 {
     if(iCount == 0)
         m_strlistVerHeaderText.clear();
     m_model.setRowCount(iCount);
 }


 int wika_tableView::GetRowCount()
 {
     return m_model.rowCount();
 }

 void wika_tableView::SetColumnCount(int iCount)
 {
     if(iCount == 0)
         m_strlistHorHeaderText.clear();
     m_model.setColumnCount(iCount);
 }


 int wika_tableView::GetColumnCount()
 {
     return m_model.columnCount();
 }

 QStandardItemModel *wika_tableView::GetDataModel()
 {
     return &m_model;
 }

 wika_ItemDelegate* wika_tableView::GetDelegate()
 {
     return m_delegate;
 }

 int wika_tableView::getCurrentRowCount(){
//     if(!viewport())
//         return 0;
     return  viewport()->height()/rowHeight(0);
 }

 int wika_tableView::getCurrentColumnCount(){
//     if(!viewport())
//         return 0;
      return  viewport()->width()/columnWidth(0);
  }

 void wika_tableView::SetRolling(bool downOrRight,int step,Qt::Orientation orientation)
 {
     if(orientation == Qt::Vertical){
         QScrollBar* m_scrollBar = verticalScrollBar();
         if(!m_scrollBar)
             return;

         int counts = viewport()->height()/rowHeight(0); //每页显示行数(默认所有行相等高度)
         m_scrollBar->setMinimum(0);
         m_scrollBar->setMaximum(GetRowCount()-counts);
         int tempStep = m_scrollBar->value();
//         qDebug()<<tempStep;

         if (downOrRight)
         {
             //向下滚动
             m_scrollBar->setValue(tempStep+step);
         }
         else
         {
             //向上滚动
             m_scrollBar->setSliderPosition(tempStep-step);
         }
     }else{
         QScrollBar* m_scrollBar = horizontalScrollBar();
         if(!m_scrollBar)
             return;

         m_scrollBar->setMinimum(0);
         int counts = viewport()->width()/columnWidth(0);
         m_scrollBar->setMaximum(GetColumnCount()-counts);
         int tempStep = m_scrollBar->value();

         if (downOrRight)
         {
             //向右滚动
             m_scrollBar->setSliderPosition(tempStep+step);
         }
         else
         {
             //向左滚动
             m_scrollBar->setSliderPosition(tempStep-step);
         }
     }

 }


 QColor wika_tableView::GetColor(int a,int b,int c){
     return QColor::fromRgb(qRgb(a,b,c));
 }

 void wika_tableView::SetText(int row,int column,QString strText){
     QStandardItem* pTableItem = m_model.item(row,column);
     if (!pTableItem){
         pTableItem = new QStandardItem(strText);
         m_model.setItem(row,column,pTableItem);
     }else{
         pTableItem->setText(strText);
     }

 }

 void wika_tableView::SetFont(int row,int column,QFont qfont){
     QStandardItem* pTableItem = m_model.item(row,column);
     if (pTableItem){
         pTableItem->setFont(qfont);
     }
 }

 void wika_tableView::SetBackClor(int row,int column,QColor qclor){
     QStandardItem* pTableItem = m_model.item(row,column);
     if (pTableItem){
          if (qclor.isValid())
              // pTableItem->setBackground(QBrush(QColor(0,77,128,255)));
              pTableItem->setData(QBrush(qclor),Qt::BackgroundRole);
      }
 }

 void wika_tableView::SetForeClor(int row,int column,QColor qclor){
     QStandardItem* pTableItem = m_model.item(row,column);
     if (pTableItem){
         if (qclor.isValid())
             pTableItem->setData(QBrush(qclor),Qt::ForegroundRole);
     }
 }

 int wika_tableView::GetSelectedRow(){
     if (currentIndex().isValid())
     {
         return currentIndex().row();
     }
     return -1;
 }

 QModelIndex wika_tableView::GetSelectedIndex(){
     return currentIndex();
 }

 void wika_tableView::setSelectedItems(int topLeft_x,int topLeft_y,int bottomRight_x,int bottomRight_y){
     QItemSelectionModel* selectionModel = this->selectionModel();
     QModelIndex topLeft;
     QModelIndex bottomRight;
     topLeft = m_model.index(topLeft_x,topLeft_y,QModelIndex());
     bottomRight = m_model.index(bottomRight_x,bottomRight_y,QModelIndex());
     QItemSelection selection(topLeft,bottomRight);
     selectionModel->clear();
     selectionModel->select(selection,QItemSelectionModel::Select);

 }

 void wika_tableView::SetTextAlignment(int row,int column,Qt::AlignmentFlag alignmentFlg){
     QStandardItem* pTableItem = m_model.item(row,column);
     if (pTableItem){
             pTableItem->setTextAlignment(alignmentFlg);
     }
 }

 void wika_tableView::resizeEvent(QResizeEvent *event)
 {
     QSize oldSize = event->oldSize();
     QSize size = event->size();
     if (size.width() == oldSize.width())
         return;

     int iWidth = size.width()-m_iWidthOffset;
     int iHeight = size.height()-m_iHeightOffset;
     ResizeColumn(iWidth);
     ResizeRow(iHeight);

     if(bFrozened)
         updateFrozenTableGeometry();
     QTableView::resizeEvent(event);
 }

 void wika_tableView::updateSectionWidth(int logicalIndex, int, int newSize)
 {
       if(logicalIndex==0){
             frozenTableView->setColumnWidth(0,newSize);
             updateFrozenTableGeometry();
       }
 }

 void wika_tableView::updateSectionHeight(int logicalIndex, int, int newSize)
 {
       frozenTableView->setRowHeight(logicalIndex, newSize);
 }

 void wika_tableView::mouseDoubleClickEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton)
     {
         QModelIndex index = this->currentIndex();
         if (index.isValid())
         {
             emit LButtonDBClicked(index);
         }
     }
     QTableView::mouseDoubleClickEvent(event);
 }

 wika_ItemDelegate::wika_ItemDelegate(QObject * parent)
     : QItemDelegate(parent)
 {
     favouritePixmap=QPixmap(":/image/true.png");
     notFavouritePixmap=QPixmap(":/image/false.png");
     durationColumn = 10000;
 }

 void wika_ItemDelegate::paint(QPainter * painter,
         const QStyleOptionViewItem & option,
         const QModelIndex & index) const
 {
     if(index.column() == m_rowIndex){
         const QAbstractItemModel * model=index.model();
         QVariant var=model->data(index,Qt::CheckStateRole);
         if(var.isNull()) var=false;
         const QPixmap & star=var.toBool()?
                              favouritePixmap:notFavouritePixmap;


         int width=star.width();
         int height=star.height();
         QRect rect=option.rect;
         int x=rect.x()+rect.width()/2-width/2;
         int y=rect.y()+rect.height()/2-height/2;

         painter->drawPixmap(x,y,star);

     }else if (index.column() == durationColumn) {
         int secs = index.model()->data(index, Qt::DisplayRole).toInt();
         QString text = QString("%1:%2")
                        .arg(secs / 60, 2, 10, QChar('0'))
                        .arg(secs % 60, 2, 10, QChar('0'));

         QStyleOptionViewItem myOption = option;
         myOption.displayAlignment = Qt::AlignLeft  | Qt::AlignVCenter; //Qt::AlignRight
         drawDisplay(painter, myOption, myOption.rect, text);
         drawFocus(painter, myOption, myOption.rect);
     } else{
         QItemDelegate::paint(painter,option,index);
         return;
     }

 }

 QWidget *wika_ItemDelegate::createEditor(QWidget *parent,
         const QStyleOptionViewItem &option,
         const QModelIndex &index) const
 {
     if (index.column() == durationColumn) {
         QTimeEdit *timeEdit = new QTimeEdit(parent);
         timeEdit->setDisplayFormat("mm:ss"); //hh:mm:ss
         connect(timeEdit, SIGNAL(editingFinished()),
                 this, SLOT(commitAndCloseEditor()));
         return timeEdit;
     } else {
         return QItemDelegate::createEditor(parent, option, index);
     }
 }

 void wika_ItemDelegate::setEditorData(QWidget *editor,
                                   const QModelIndex &index) const
 {

     if (index.column() == durationColumn) {

         int secs = index.model()->data(index, Qt::DisplayRole).toInt();

         QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor);
         timeEdit->setTime(QTime(0, secs / 60, secs % 60));
     } else {
         QItemDelegate::setEditorData(editor, index);
     }
 }

 void wika_ItemDelegate::setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const
 {

     if (index.column() == durationColumn) {

         QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor);
         QTime time = timeEdit->time();
         int secs = (time.minute() * 60) + time.second();

         model->setData(index, secs);
     } else {

         QItemDelegate::setModelData(editor, model, index);
     }
 }

 void wika_ItemDelegate::commitAndCloseEditor()
 {
     QTimeEdit *editor = qobject_cast<QTimeEdit *>(sender());

     emit commitData(editor);
     emit closeEditor(editor);
 }
