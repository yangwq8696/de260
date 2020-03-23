#ifndef ERRORTYPEDEF_H
#define ERRORTYPEDEF_H

#include <QObject>
#include <QWidget>
#include <QString>
class ErrorTypedef
{
public:
    QString  ErrorInfor;
    uint16_t ErrorCode;
    uint8_t  ErrorClass;
    bool     ErrorStatus;


};

#endif // FUNC_ERRORTYPEDEF_H
