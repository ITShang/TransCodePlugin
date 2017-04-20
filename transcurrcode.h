#ifndef TRANSCURRCODE_H
#define TRANSCURRCODE_H

#include <QString>

class TransCurrCode
{
public:
    TransCurrCode();
    void transCurrFileCode();
    void setTargetCode(QString code);

private:
    QString targetCode;
};

#endif // TRANSCURRCODE_H
