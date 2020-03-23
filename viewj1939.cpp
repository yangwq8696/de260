#include "viewj1939.h"
#include "ui_viewj1939.h"
#include "view1939erroocode.h"


static const int pageRows = 16;
static const int pageCols = 2;
static const int pageWidgets = 2;

//故障信息
//code:故障编码
//compoment:故障内容
typedef struct j1939Diag_t {
    uint32_t code;
    const char *component;
} j1939Diag_t;

//所属部件的内容
const char * j1939TypeText[5]={
    "发动机",
    "ABS",
    "变速器",
    "转向系统",
    "结构化编码错误"
};


#define ERRO_CODE_NUM_MAX       32      //最大能显示的故障数
static  j1939Diag_t diagList[ERRO_CODE_NUM_MAX] = {

};

/*
*接收到的所有故障
*len:现有故障数量
*j1939CodeStr:故障信息结构体,含编号和内容
*/
typedef struct {
    unsigned char len;
    ERROR_1939_CODE j1939CodeStr[ERRO_CODE_NUM_MAX];
}J1939ERROR_QUEEN_STR;

static J1939ERROR_QUEEN_STR  j1939ErrorQueenStr={0,};

/*
 * 本页面的监听的CAN变量列表
 */
static canVar_t varList[] = {
    { can1_r_292_b01,           0x0000ffff,          1,  0    },
    { can1_r_292_b23,           0x0000ffff,          1,  0    },
    { can1_r_292_b45,           0x0000ffff,          1,  0    },
    { can1_r_292_b67,           0x0000ffff,          1,  0    },
};

viewJ1939::viewJ1939(Page *parent) :
    Page(PageView, parent),
    ui(new Ui::viewJ1939),
    m_itemOffset(0),
    m_currentPage(0),
    m_lastRow(-1)
{
    ui->setupUi(this);

    /*
     * 使用了两个表同时显示内容
     */
    initTable(ui->tableWidgetL);
    initTable(ui->tableWidgetR);
    msgboxHide();

    // 先翻到第0页
    setPage(0);

    // 激活CAN接收处理
    canActive(varList, N(varList));
}

viewJ1939::~viewJ1939()
{
    delete ui;
}


void viewJ1939::getDiagList(void)
{
    int         i=0;
    for(i=0;i<j1939ErrorQueenStr.len;i++)
    {
	//获取故障编码
        diagList[i].code=j1939ErrorQueenStr.j1939CodeStr[i].code;
	//获取故障所在的部件
        switch(j1939ErrorQueenStr.j1939CodeStr[i].type)
        {
            case J1939_ERROR_TYPE_ENGINE:
                diagList[i].component=j1939TypeText[0];
                break;
            case J1939_ERROR_TYPE_ABS:
                diagList[i].component=j1939TypeText[1];
                break;
            case J1939_ERROR_TYPE_TRANSMISSION:
                diagList[i].component=j1939TypeText[2];
                break;
            case J1939_ERROR_TYPE_STEERING_SYSTERM:
                diagList[i].component=j1939TypeText[3];
                break;
            default:
                diagList[i].component=j1939TypeText[4];
                break;
        }
    }
}

void viewJ1939::msgboxShow(QString msg)
{
    /*
     * 通过图片、字符标签模拟对话框
     * 注意：不能使用QMessageBox，因为QMessageBox会阻塞当前进程，无法接收按键处理
     */
    ui->labelMsg->setText(msg);
    ui->iconMsgbox->show();
    ui->labelMsg->show();
}

void viewJ1939::msgboxHide()
{
    ui->iconMsgbox->hide();
    ui->labelMsg->hide();
}

bool viewJ1939::msgboxIsVisible()
{
    return ui->iconMsgbox->isVisible();
}

void viewJ1939::initTable(QTableWidget *table)
{
    /*
     * 初始化表格的样式、表头等
     */
    table->verticalHeader()->hide();
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    table->horizontalHeader()->setStyleSheet("QHeaderView::section{font:14pt;font-weight:bold; background-color: transparent; border:1px solid rgb(60, 197, 228);};");
    table->horizontalHeader()->setFixedHeight(37);
    table->verticalHeader()->setDefaultSectionSize(36);

    QStringList headers;
    headers << tr("故障代码") << tr("所属部件");
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    //设置行宽和列高
    table->setColumnWidth(0, 250);
    table->setColumnWidth(1, 393);
}

void viewJ1939::keyPressEvent(QKeyEvent *event)
{
    QTableWidget *table = ui->tableWidgetR->hasFocus() ? ui->tableWidgetR : ui->tableWidgetL;
    QTableWidget *tableAlt = ui->tableWidgetR->hasFocus() ? ui->tableWidgetL : ui->tableWidgetR;
    int currentRow = table->currentRow();

    int offset = 0;

    if(table==ui->tableWidgetL)
        offset = currentRow;
    else
        offset = currentRow+pageRows;




    /*
     * 设置菜单快捷键
     */
    switch (event->key()) {
#if 0
    case Qt::Key_F1:
        m_currentPage = (pageNum + m_currentPage - 1) % pageNum;
        setPage(m_currentPage);
        break;
    case Qt::Key_F2:
        m_currentPage = (m_currentPage + 1) % pageNum;
        setPage(m_currentPage);
        break;
#else
    case Qt::Key_F1:
            /*
             * 连续两次都在第0行时，说明已经到行首了，切换到另一张表的行尾
             */
            if (table->hasFocus() && currentRow == m_lastRow && currentRow == 0) {
                table->clearSelection();
                tableAlt->setFocus();
                tableAlt->selectRow(tableAlt->rowCount() - 1);
                table = tableAlt;
            }
	    //判断当前行有无内容,有则显示对号,无则不显示
            if(table->item(currentRow,0)==NULL)
                ui->btnEnter->hide();
            else
                ui->btnEnter->show();

        Page::keyPressEvent(event);
        break;
    case Qt::Key_F2:
            /*
             * 连续两次都在最后一行时，说明已经到行尾了，切换到另一张表的首行
             */
            if (table->hasFocus() && currentRow == m_lastRow && currentRow == table->rowCount() - 1) {
                table->clearSelection();
                tableAlt->setFocus();
                tableAlt->selectRow(0);
                table = tableAlt;
            }
	    //判断当前行有无内容,有则显示对号,无则不显示
            if(table->item(currentRow,0)==NULL)
                ui->btnEnter->hide();
            else
                ui->btnEnter->show();

        Page::keyPressEvent(event);
        break;
    case Qt::Key_F3:
        /*
         * 选中时提示信息，再次按下此键时关闭对
         */
        if (msgboxIsVisible()) {
            msgboxHide();
        } else {
             if(table->item(currentRow,0)!=NULL)
                msgboxShow("故障编码: "+table->item(currentRow,0)->text() +"\n故障内容: "+ j1939ErrorQueenStr.j1939CodeStr[offset].textZh );


        }
        break;
#endif
    default:
        Page::keyPressEvent(event);
        break;
    }

    m_lastRow = table->currentRow();
}

void viewJ1939::setPage(QTableWidget *table)
{
//    int count = table->rowCount();

    int count =0;
    /*
     * 清空表中原有内容
     */
    for(int i=0;i<pageRows;i++)
        table->removeRow(i);
    table->setRowCount(pageRows);
    table->clearContents();

    /*
     * 向表中填入指定行数内容
     */
    for (unsigned int i=m_itemOffset, count=0;
         i < j1939ErrorQueenStr.len && count < pageRows;
         i++, count++) {
        addItem(table, count,diagList[i].code, diagList[i].component);
    }

}

void viewJ1939::setPage(int page)
{
    QTableWidget *tableL = ui->tableWidgetL;
    QTableWidget *tableR = ui->tableWidgetR;

    m_currentPage = page;
    m_itemOffset = m_currentPage * (pageRows * pageWidgets);
    //qDebug() << __func__ << __LINE__ << "page=" << m_currentPage << "offset=" << m_itemOffset;

    /*
     * 向两张表中分别填入内容
     * 注意：第二张表内容接着第一张表
     */
    setPage(tableL);
    setPage(tableR);


    /*
     * 默认选中第一张表的第一行
     */
    tableL->setFocus();
    tableL->selectRow(0);

    if(j1939ErrorQueenStr.len==0)
        ui->btnEnter->hide();

}

void viewJ1939::addItem(QTableWidget *table, int line,int code, const char *component)
{
   // QTableWidgetItem *itemCode  = new QTableWidgetItem(QString("0x%1").arg(code, 0, 16));
    QTableWidgetItem *itemCode  = new QTableWidgetItem(QString::number(code));
    itemCode->setTextAlignment(Qt::AlignCenter);
    table->setItem(line, 0, itemCode);

    QTableWidgetItem *itemComponent  = new QTableWidgetItem(QString(component));
    itemComponent->setTextAlignment(Qt::AlignCenter);
    table->setItem(line, 1, itemComponent);

    /*
     * 通过记录条数偏移来定位
     */
    m_itemOffset++;
}

void viewJ1939::j1939Inquire(int /*code*/)
{
    canSendPdo(0, CanPdoInquireError);
}



void viewJ1939::onCanRecv(int code, msg_can_t *m)
{
    uint32_t val = 0, spn = 0, fmi = 0,  num=0;

    int i=0,j=0;
    //是否需要更新表格标志位,false:不,true:是
    bool updateUi = false;
    //当前错误代码是否已存在标志位  false:不,true: 是
    bool existFlag = false;

    const ERROR_1939_CODE * errorP   = J1939ErrorEngineStr;
    uint8_t type                     = J1939_ERROR_TYPE_ENGINE;

//    qDebug() << "Meter" << __func__ << __LINE__ << "var:" << m->var << "val:" << m->val_int32;

    if (code != MSG_CAN_OD)
        return;

    for (unsigned int i=0; i<N(varList); i++) {
        if (varList[i].var == m->var) {
            val = (m->val_int32 & varList[i].mask)%10000;

            num = val%1000;

            int i=0,j=0;
            switch(m->var)
            {
                case can1_r_292_b01:
                    errorP = J1939ErrorEngineStr;
                    type   = J1939_ERROR_TYPE_ENGINE;
                    break;
                case can1_r_292_b23:
                    errorP = J1939ErrorAbsStr;
                    type   = J1939_ERROR_TYPE_ABS;
                    break;
                case can1_r_292_b45:
                    errorP = J1939ErrorTransmissionStr;
                    type   = J1939_ERROR_TYPE_TRANSMISSION;
                    break;
                case can1_r_292_b67:
                    errorP = J1939ErrorSteeringSystemStr;
                    type   = J1939_ERROR_TYPE_STEERING_SYSTERM;
                    break;
            }

            existFlag = false;
            for(i=0;i<j1939ErrorQueenStr.len;i++)
            {
                if((j1939ErrorQueenStr.j1939CodeStr[i].type==type)&&\
                        (j1939ErrorQueenStr.j1939CodeStr[i].num==num))
                {
                    existFlag=true;
                    break;
                }
            }
            if(val>=1000)
            {
                if((j1939ErrorQueenStr.len<ERRO_CODE_NUM_MAX)&&(existFlag==false))
                {
		   //故障总数小于32并且当前故障表中不存在此故障,则添加上此故障
                   updateUi = true;
                   j1939ErrorQueenStr.len++;
                   j1939ErrorQueenStr.j1939CodeStr[j1939ErrorQueenStr.len-1]=errorP[num];
                }
            }
            else if(val<1000)
            {
                if(existFlag==true)
                {
		    //若已存在,且小于1000则消除次代码
                    for(j=0;j<(j1939ErrorQueenStr.len-1-i);j++)
                    {
                        j1939ErrorQueenStr.j1939CodeStr[i+j]=j1939ErrorQueenStr.j1939CodeStr[i+j+1];
                    }
		    //数量减1
                    j1939ErrorQueenStr.len--;
                    updateUi = true;
                }
            }

            break;
        }
    }
    //更新表格
    if(updateUi==true)
    {
	
	//记住更新前的表格和行数,后面更新后再次选中此行
        QTableWidget *table = ui->tableWidgetR->hasFocus() ? ui->tableWidgetR : ui->tableWidgetL;
        int currentRow = table->currentRow();
	//更新所有故障的编码和内容
        getDiagList();
        m_itemOffset=0;
	//更新表格
        setPage(ui->tableWidgetL);
        setPage(ui->tableWidgetR);

	//选中更新前的表格行
        table->selectRow(currentRow);
	//若无故障则隐藏对号按钮
        if(j1939ErrorQueenStr.len==0)
            ui->btnEnter->hide();
        else
            ui->btnEnter->show();
	//若当前行为空则隐藏对号按钮
        if(table->item(currentRow,0)==NULL)
            ui->btnEnter->hide();
        else
            ui->btnEnter->show();
    }
}
