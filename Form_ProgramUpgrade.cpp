#include "Form_ProgramUpgrade.h"
#include "ui_Form_ProgramUpgrade.h"
#include <QDir>
#include <QDebug>
#include <QPainter>
#include "wika_GeneralWidget.h"
#include "main.h"
#include <QAbstractItemView>
#include <QTimer>
#include "hsmlib.h"

/*
 * 本页面的CAN变量列表
 */
static const canVar_t varList[] =
{
};

Form_ProgramUpgrade::Form_ProgramUpgrade(QWidget *parent) :
    Page(parent),
    m_timer(0),
    m_Type(-1),
    m_usb_state(1),
    ui(new Ui::Form_ProgramUpgrade)
{
    ui->setupUi(this);
    InitData();

    m_GeneralWidget = new wika_GeneralWidget(this);
    m_GeneralWidget->move(0,0);

    m_timer = new QTimer();
    usbdisk_check();

    //
    //初始化列表
    //
    ui->tableView->setEnabled(false);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->SetHeaderByIndex(Qt::Horizontal,0,"id");
    ui->tableView->SetHeaderAbsWidthByIndex(Qt::Horizontal,0,80);
    ui->tableView->SetHeaderByIndex(Qt::Horizontal,1,tr("程序名"));
    ui->tableView->SetHeaderAbsWidthByIndex(Qt::Horizontal,1,300);
    ui->tableView->SetHeaderByIndex(Qt::Horizontal,2,tr("节点号"));
    ui->tableView->SetHeaderAbsWidthByIndex(Qt::Horizontal,2,80);
    ui->tableView->SetHeaderByIndex(Qt::Horizontal,3,tr("版本"));
    ui->tableView->SetHeaderAbsWidthByIndex(Qt::Horizontal,3,80);
    ui->tableView->SetHeaderByIndex(Qt::Horizontal,4,tr("状态"));
    ui->tableView->SetHeaderAbsWidthByIndex(Qt::Horizontal,4,150);
    ui->tableView->SetHeaderItemVisible(Qt::Horizontal,0,false);
    ui->tableView->SetHeaderVisible(Qt::Vertical,false);
//    ui->tableView->SetHeaderVisible(Qt::Horizontal,false);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(200);

    connect(m_msg, SIGNAL(sysRecvMsg(int, uint32_t)), this, SLOT(recvCanUpdateMsg(int, uint32_t)));

}

Form_ProgramUpgrade::~Form_ProgramUpgrade()
{
    delete ui;
}

void Form_ProgramUpgrade::showForm(){
    ui->retranslateUi(this);
    if (m_timer){ delete m_timer; m_timer=0;}
    m_timer = new QTimer();
    m_timer->stop();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(usbdisk_check()));
    m_timer->start(1000);

    canActive(CAN1_DEV, varList, N(varList));
    showFullScreen();

    InitView();
    ui->label_6->setVisible(false);
}

void Form_ProgramUpgrade::closeForm()
{
    m_timer->stop();
    if (m_timer){ delete m_timer; m_timer=0;}

    this->hide();
    canDeactive();
}

void Form_ProgramUpgrade::InitData(){
    m_packageNum = 0;
    m_index_selected = 0;
    m_count = 0;
    m_progressBarValue_old = 0;
    m_update_operate = 0;
    //初始化升级包状态
    int count = m_updateStates.size();
    for(int i =0;i<count;i++){
        m_updateStates[i] = "未升级1";
    }
    if(!m_updateStates.size())
        m_updateStates<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")
                  <<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")
                  <<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")
                  <<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")
                  <<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")
                  <<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1")<<tr("未升级1");
}

void Form_ProgramUpgrade::InitView(){
//    qDebug()<<"m_languageId: "<<m_languageId;
    if(m_languageId == LanguageZh){
        this->setStyleSheet("QTableView{ \
                            color: rgb(196, 160, 0); \
                            font: 16pt \"WenQuanYi Zen Hei\"; \
                            background-color: transparent; \
                            gridline-color:rgba(73,153,216,200); \
                            selection-background-color:rgb(170,255,255); \
                            selection-color:rgb(255,0,0); \
                            } \
                            QLabel{ \
                            color: white; \
                            font: 15pt \"WenQuanYi Zen Hei\"; \
                            }");
         ui->label_5->setStyleSheet("color: rgb(252, 233, 79); \
                                    font: 30pt \"TakaoPGothic\";");
    }
    if(m_languageId == LanguageEn){
        this->setStyleSheet("QTableView{ \
                            color: rgb(196, 160, 0); \
                            font: 14pt \"WenQuanYi Zen Hei\"; \
                            background-color: transparent; \
                            gridline-color:rgba(73,153,216,200); \
                            selection-background-color:rgb(170,255,255); \
                            selection-color:rgb(255,0,0); \
                            } \
                            QLabel{ \
                            color: white; \
                            font: 12pt \"WenQuanYi Zen Hei\"; \
                            }");
         ui->label_5->setStyleSheet("color: rgb(252, 233, 79); \
                                    font: 28pt \"TakaoPGothic\";");
    }
}

void Form_ProgramUpgrade:: paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //绘制表盘背景
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pic_de260/bg_01.png"));
}
void Form_ProgramUpgrade::showEvent(QShowEvent *){
    usbdisk_check();
    if(m_usb_state==0){
        /*获取当前升级包的数量*/
        QDir usbDisk(m_usbPath);
        usbDisk.setNameFilters(QStringList("*.app"));
        m_packageNames = usbDisk.entryList();
        m_packageNum = m_packageNames.size();
//        qDebug()<<qApp->applicationDirPath();
    }

    //设置当前选中索引
    m_index_selected = 0;

    /*begin_多国语言翻译升级包的升级状态(在升级界面显示的时候)*/
    //翻译（通过关联变量来标识当前状态）
    int count = m_updateStates.size();
    int type = 0;
    if(m_languageId == LanguageZh){
        for(int i =0;i<count;i++){
            type = m_updateStates[i].right(1).toInt();
            switch (type) {
            case 1:
                m_updateStates[i] = "未升级1";
                break;
            case 2:
                m_updateStates[i] = "取消升级2";
                break;
            case 3:
                m_updateStates[i] = "升级失败3";
                break;
            case 4:
                m_updateStates[i] = "升级超时4";
                break;
            case 5:
                m_updateStates[i] = "传输完毕5";
                break;
            case 6:
                m_updateStates[i] = "升级完毕6";
                break;
            case 7:
                m_updateStates[i] = "开始传输7";
                break;
            default:
                m_updateStates[i] = "未升级1";
            }
        }
    }
    if(m_languageId == LanguageEn){
        for(int i =0;i<count;i++){
            type = m_updateStates[i].right(1).toInt();
            switch (type) {
            case 1:
                m_updateStates[i] = "Not upgraded1";
                break;
            case 2:
                m_updateStates[i] = "upgraded cacel2";
                break;
            case 3:
                m_updateStates[i] = "upgraded failed3";
                break;
            case 4:
                m_updateStates[i] = "upgraded tooLong4";
                break;
            case 5:
                m_updateStates[i] = "transmit finished5";
                break;
            case 6:
                m_updateStates[i] = "upgraded finished6";
                break;
            case 7:
                m_updateStates[i] = "transmit start7";
                break;
            default:
                m_updateStates[i] = "Not upgraded1";
            }
        }
    }
    /*end_多国语言翻译升级包的升级状态(在升级界面显示的时候)*/
}

void Form_ProgramUpgrade::onRelease_Button1()
{
    ui->label_6->setVisible(false);
    m_count = 0;
    if(m_packageNum==0)
        return;

    int ret = canopen_abort_update(CAN1_DEV);
    m_update_operate = 0;
    m_updateStates[m_index_update] = tr("取消升级2");
}
void Form_ProgramUpgrade::onRelease_Button2()
{
    if(m_packageNum==0)
        return;
//    QItemSelectionModel*selections = ui->tableView->selectionModel();
//    QModelIndex index = selections->currentIndex();
//    m_index_selected = index.row();
    m_index_selected = (m_index_selected+m_packageNum-1)%m_packageNum;
    ui->tableView->setSelectedItems(m_index_selected,1,m_index_selected,4);

//    qDebug()<<"m_index_selected:"<<m_index_selected<<"m_packageNum:"<<m_packageNum;
}
void Form_ProgramUpgrade::onRelease_Button3()
{
    if(m_packageNum==0)
        return;
//    QItemSelectionModel*selections = ui->tableView->selectionModel();
//    QModelIndex index = selections->currentIndex();
//    m_index_selected = index.row();
    m_index_selected = (m_index_selected+1)%m_packageNum;
    ui->tableView->setSelectedItems(m_index_selected,1,m_index_selected,4);

//    qDebug()<<"m_index_selected:"<<m_index_selected<<"m_packageNum:"<<m_packageNum;
}

void Form_ProgramUpgrade::onRelease_Button4()
{
    if(m_update_operate == 0){
        m_update_operate = 1;
        m_updateStates[m_index_selected] = tr("开始传输7");
        m_index_update = m_index_selected;
    }else
        return;

    m_Type = 1;
    QVariant variant_Name = ui->tableView->ItemData(m_index_update,1,Qt::DisplayRole);
//    QVariant variant_Status = ui->tableView->ItemData(m_index_selected,2,Qt::DisplayRole);


    QString strName = variant_Name.toString();
    QString appPath = m_usbPath + strName;
    int index = strName.lastIndexOf("_");
    QString vertion = strName.mid(index+1,6);
    strName.chop(11);
    index = strName.lastIndexOf("_");
    int nodeid = strName.mid(index+1,strName.length()).toInt(); //节点

    m_Type = 1;
    char chName[256]={0};
    strncpy(chName,appPath.toStdString().c_str(),appPath.toLocal8Bit().length());
//    qDebug()<<"chName:"<<chName<<" nodeid:"<<nodeid<<" extmode:"<<extmode;
    m_nodeid_current = nodeid;
    m_extmode_current = 0;
    int ret = canopen_update_controller(CAN1_DEV, nodeid, 1, chName);
    m_type_operate = 0;  //传输
//    ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,tr("开始传输"));

    ui->progressBar->setValue(0);
    qApp->processEvents();

//    if (ret == 0)
//        ui->txtState->appendPlainText("已启动传输命令...");
//    else
//        ui->txtState->appendPlainText("启动传输命令失败。");
}

void Form_ProgramUpgrade::onRelease_Button5()
{
    if(UserSettingMenu == 0)
        UserSettingMenu = new FORM_UserSettingMenu;
    UserSettingMenu->showForm();
    closeForm();
}

void Form_ProgramUpgrade::reboot_controller(){
    m_type_operate = 1;//重启
    int ret = canopen_reboot_controller(CAN1_DEV, m_nodeid_current, m_extmode_current);
//    qDebug()<<"canopen_reboot_controller";
}

void Form_ProgramUpgrade::recvCanUpdateMsg(int type, uint32_t code)
{
    QString mode = (m_Type==1) ? tr("传输") : tr("重启");
//    qDebug() << __func__ << __LINE__ << type << (code & 0xffff) << (code>>16);
    if (type == MSG_UPGRADE_STATE) {
        switch (code & 0xffff) {
        case UPD_CODE_OK:
            m_Type = -1;
//            ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("成功"));
            ui->tableView->SetForeClor(m_index_selected,4,QColor(115, 210, 22));
            break;
        case UPD_CODE_FAIL:
            m_Type = -1;
//            ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("失败"));
            m_updateStates[m_index_update] = tr("升级失败3");
            m_update_operate = 0;
            break;
        case UPD_CODE_TIMEOUT:
//            ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("超时"));
            m_updateStates[m_index_update] = tr("升级超时4");
            m_update_operate = 0;
            break;
        case UPD_CODE_UNKNOWN_ERR:
//            ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("未知错误"));
            break;
        case UPD_CODE_ARGS_ERR:
//            ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("参数错误"));
            break;
        case UPD_CODE_NOT_ALLOWED:
//            ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("不允许"));
            break;
        case UPD_CODE_DOING:
//            ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("已经在进行"));
            break;
        case UPD_CODE_PROGRESS:
            if((code>>16)!=0){
                ui->progressBar->setValue((code >> 16)*1.6);
                qApp->processEvents();
            }
//            qDebug()<<"UPD_CODE_PROGRESS value:"<<(code >> 16);
            if((code>>16)==100){
                if(m_type_operate ==0){
                    QTimer::singleShot(7000,this,SLOT(reboot_controller()));
                     m_updateStates[m_index_update] = tr("传输完毕5");
//                    ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("传输完毕"));
                }else{
                    ui->progressBar->setValue((code >> 16)*0.4+160);
                    qApp->processEvents();
                    m_updateStates[m_index_update] = tr("升级完毕6");
                    m_update_operate = 0;
//                    ui->tableView->SetItemData(m_index_selected,4,Qt::DisplayRole,mode + tr("重启完毕"));
                }
            }
            break;
        }
    }
}

void Form_ProgramUpgrade::usbdisk_check(){

    m_usb_state = access_usbdisk();
    if(m_usb_state == 0)//有ｕ盘接入
    {
        ui->label_3->setText(tr("u盘接入"));
        ui->label_3->setStyleSheet("color: rgb(115, 210, 22);");

        /*获取当前升级包*/
        QDir usbDisk(m_usbPath);
        usbDisk.setNameFilters(QStringList("*.app"));
        m_packageNames = usbDisk.entryList();
        m_packageNum = m_packageNames.size();        
        showPackageInfo();
        ui->tableView->setSelectedItems(m_index_selected,1,m_index_selected,4);
    }else {
        ui->label_3->setText(tr("u盘未接入"));
        ui->label_3->setStyleSheet("color:red");
        m_packageNames.clear();
        m_packageNum=0;
        InitData();
        showPackageInfo();
    }

    //
    //进度条监控
    //
    if(m_update_operate){
        if(0 == ui->progressBar->value()){
            m_count++;
        }else{
            m_count = 0;
        }
        m_progressBarValue_old = ui->progressBar->value();

        if(m_count>=30)
            ui->label_6->setVisible(true);
        else
            ui->label_6->setVisible(false);
    }
}

void Form_ProgramUpgrade::showPackageInfo(){
    QString strName;
    ui->tableView->SetRowCount(0);
    int i = 0;
    foreach(QString str,m_packageNames){
        if(str == "ICM7100.app"){
            m_packageNum --;
            continue;
        }

        strName = str;
        int index = str.lastIndexOf("_");
        QString vertion = str.mid(index+1,6);  //扩展贞模式
        str.chop(11);
        index = str.lastIndexOf("_");
        int nodeid = str.mid(index+1,str.length()).toInt(); //节点

        int num = ui->tableView->GetRowCount();
        ui->tableView->InsertRow(num);
        ui->tableView->SetItemData(num,1,Qt::DisplayRole,strName);
        ui->tableView->SetTextAlignment(num,1,Qt::AlignCenter);
        ui->tableView->SetForeClor(num,1,Qt::white);

        ui->tableView->SetItemData(num,2,Qt::DisplayRole,nodeid);
        ui->tableView->SetTextAlignment(num,2,Qt::AlignCenter);
        ui->tableView->SetForeClor(num,2,Qt::white);

        ui->tableView->SetItemData(num,3,Qt::DisplayRole,vertion);
        ui->tableView->SetTextAlignment(num,3,Qt::AlignCenter);
        ui->tableView->SetForeClor(num,3,Qt::white);

        QString strState = m_updateStates[i];
        strState.chop(1);
        ui->tableView->SetItemData(num,4,Qt::DisplayRole,strState);
        ui->tableView->SetTextAlignment(num,4,Qt::AlignCenter);
        ui->tableView->SetForeClor(num,4,Qt::white);

        i++;
    }
}

int Form_ProgramUpgrade::access_usbdisk(void)
{
    char fname[MAX_NAME_LEN], cmdbuf[MAX_LINE_SIZE];
    int i, d;

    if (!sys_mounted_usbdisk())
        return -1;

    for (d='a'; d<'z'; d++)
    for (i=0; i<16; i++) {
        snprintf(cmdbuf, sizeof(cmdbuf), WIKA_USB_DEV"%d", i);
        if (is_mounted(cmdbuf)) {
            snprintf(fname, sizeof(fname), "%s/sd%c%d/", WIKA_MEDIA_PATH, d, i);
            m_usbPath = QString(fname);
            return 0;
        }
    }

    return -1;
}
