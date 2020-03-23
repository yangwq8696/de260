#ifndef FORM_CANBUSQUERY_H
#define FORM_CANBUSQUERY_H

#include <QWidget>
#include "page.h"

namespace Ui {
class FORM_CanBusQuery;
}

class FUN_MeterFinger;
class wika_GeneralWidget;
class FORM_CanBusQuery : public Page
{
    Q_OBJECT

public:
    explicit FORM_CanBusQuery(Page *parent = nullptr);
    ~FORM_CanBusQuery();
    void showForm();
    void closeForm();
    void initView();
private slots:
    void onCanRecv(int code, msg_can_t *m) override;
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();
    void canBusStateDisplay();

protected:
    void paintEvent(QPaintEvent *event);      //绘图事件
private:
    void createFigner();

    bool getMeterCanState();       //获取仪表ｃａｎ状态
    bool getButtonPanelState();    //按钮板状态
    bool getEngineState();         //发动机状态
    bool getGECanState();          //GE can 状态
    bool getWeighingSystemState(); //称重系统CAN状态
    bool getHydraulicControllerState();  //液压控制器CAN状态
    bool getAuxiliaryContrullerState();  //辅助控制器CAN状态

    void setCanState();

private:
    Ui::FORM_CanBusQuery *ui;
    bool m_uiInited;

    QTimer *m_timer;

    QStringList currentWaringList;
    uint32_t m_canBusDataOld;
    uint32_t m_canBusData;
    uint8_t m_count_canBas;
    bool m_canBusState;
    uint32_t m_count;

    FUN_MeterFinger* m_finger1;
    FUN_MeterFinger* m_finger2;
    wika_GeneralWidget* m_GeneralWidget;
};

#endif // FORM_CANBUSQUERY_H
