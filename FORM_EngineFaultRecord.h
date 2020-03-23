#ifndef FORM_ENGINEFAULTRECORD_H
#define FORM_ENGINEFAULTRECORD_H

#include <QWidget>
#include "page.h"

typedef struct engineErrorRecord {
    uint32_t SPN;
    uint8_t FMI;
    uint8_t OC;
    uint16_t CODE;
} engineErrorRecord;

namespace Ui {
class FORM_EngineFaultRecord;
}

class wika_GeneralWidget;
class FORM_EngineFaultRecord : public Page
{
    Q_OBJECT

public:
    explicit FORM_EngineFaultRecord(Page *parent = nullptr);
    ~FORM_EngineFaultRecord();
    void showForm();
    void closeForm();
private slots:
    void onRelease_Button1();
    void onRelease_Button2();
    void onRelease_Button3();
    void onRelease_Button4();
    void onRelease_Button5();
    void onCanRecv(int code, msg_can_t *m) override;
    void DisplayErrorInfo();

protected:
    void paintEvent(QPaintEvent *event);      //绘图事件
    void changeEvent(QEvent *event);

private:
    void GetEngineFaultRecord();
    uint16_t Engine_Error_code(uint32_t spn, uint8_t fmi);
    int count;
    uint8_t bitest(uint8_t byte, uint8_t n);
    void Engine_Error();

    void EC_valueSet();
    void EB_valueSet();

    void changeLongLongToChar8(long long var,unsigned char data[8]);

private:
    Ui::FORM_EngineFaultRecord *ui;
    bool m_uiInited;

    int _EBCount = 0;
    //CA
    unsigned char ch_feca[8]={0};
    QList<engineErrorRecord> recordList;

    //EC
    uint8_t data_EC[8]={0};
     uint8_t data_EC_Temp[8]={0};
    int m_countEC;
    uint16_t DM_Number;   //故障数量
    uint8_t Pack_Number;  //报文数量
    uint8_t _18ECFF00_BYTE5;

    //EB
    uint8_t data_EB[8]={0};
    int m_countEB;
    uint8_t DM_Data[80];
    wika_GeneralWidget* m_GeneralWidget;

};

#endif // FORM_ENGINEFAULTRECORD_H
