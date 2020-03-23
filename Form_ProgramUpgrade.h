#ifndef FORM_PROGRAMUPGRADE_H
#define FORM_PROGRAMUPGRADE_H

#include <QWidget>
#include "page.h"

namespace Ui {
class Form_ProgramUpgrade;
}

class wika_GeneralWidget;
class Form_ProgramUpgrade : public Page
{
    Q_OBJECT

public:
    explicit Form_ProgramUpgrade(QWidget *parent = 0);
    ~Form_ProgramUpgrade();
    void closeForm();
    void showForm();
private:
    void InitData();
    void InitView();
protected:
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent *event);
private slots:
    void usbdisk_check();
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();
    void recvCanUpdateMsg(int type, uint32_t code); //消息响应
    void reboot_controller();
private:
    int access_usbdisk(void);
    void showPackageInfo();
private:
    Ui::Form_ProgramUpgrade *ui;
    wika_GeneralWidget* m_GeneralWidget;

    QTimer *m_timer;

    unsigned short  m_usb_state;     //是否插入ｕ盘
    unsigned short m_packageNum;     //升级包数量
    unsigned short m_index_selected; //当前选中升级包的索引
    unsigned short m_index_update;   //当前正在升级的升级包索引
    QString m_usbPath;
    QStringList m_packageNames;
    int m_Type;

    unsigned short m_nodeid_current;
    unsigned short m_extmode_current;
    unsigned short m_type_operate;  //重启、传输
    unsigned short m_update_operate;//升级操作标志符 1--正在升级 0--当前没有升级的程序

    QStringList m_updateStates;     //升级状态

    unsigned short m_progressBarValue_old;
    unsigned short m_count;

};

#endif // FORM_PROGRAMUPGRADE_H
