//**************************************************
//版权：   RenGu Company
//文件名： rgtranscode.h
//作者：   尚超     Version：1.0    Date：2016.XX.XX
//描述：   转换方法类
//其它：
//修改历史：
//**************************************************

#ifndef RGTRANSCODE_H
#define RGTRANSCODE_H

#include <QObject>

class RGtransCode : public QObject
{
    Q_OBJECT

public:
    //转换类型
    enum TransType
    {
        TYPE_REPLACEOLDFILE,
        TYPE_CREATENEWFILE,
        TYPE_CREATEOTHERFILE
    };

    RGtransCode();

    void transFileCode(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath, TransType TtansType);
    QString checkFileCode(QString sourceFilepath);
    QString readFile(QString path, QString sourceCode);

private:
    void replaceOldFile(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath);
    void createNewFile(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath);
    void createOtherFile(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath);
    bool isutf8(QByteArray data);

signals:
    //发送实时操作状态内容
    void emitRGTCStatus(QString contents);

};

#endif // RGTRANSCODE_H
