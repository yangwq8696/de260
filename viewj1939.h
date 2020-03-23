#ifndef VIEWJ1939_H
#define VIEWJ1939_H

#include <QWidget>
#include "page.h"

namespace Ui {
class viewJ1939;
}

class viewJ1939 : public Page
{
    Q_OBJECT

public:
    explicit viewJ1939(Page *parent = nullptr);
    ~viewJ1939() override;
    void addItem(QTableWidget *table, int line,int code, const char *component);
    void setPage(QTableWidget *table);
    void setPage(int page);
    void j1939Inquire(int code);
    void msgboxShow(QString msg);
    //隐藏对话框
    void msgboxHide();
    //显示对话框
    bool msgboxIsVisible();
    void getDiagList(void);


public slots:
    void onCanRecv(int code, msg_can_t *m) override;

private:
    Ui::viewJ1939 *ui;
    int m_itemOffset;
    int m_currentPage;
    int m_lastRow;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void initTable(QTableWidget *table);
};

#endif // VIEWJ1939_H
