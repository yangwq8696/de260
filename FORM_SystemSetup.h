#ifndef FORM_SYSTEMSETUP_H
#define FORM_SYSTEMSETUP_H

#include <QWidget>

namespace Ui {
class FORM_SystemSetup;
}

class FORM_SystemSetup : public QWidget
{
    Q_OBJECT

public:
    explicit FORM_SystemSetup(QWidget *parent = nullptr);
    ~FORM_SystemSetup();
    void showForm();
    void closeForm();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FORM_SystemSetup *ui;
    void keyPressEvent(QKeyEvent *event);
};

#endif // FORM_SYSTEMSETUP_H
