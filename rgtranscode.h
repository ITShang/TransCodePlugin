//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� rgtranscode.h
//���ߣ�   �г�     Version��1.0    Date��2016.XX.XX
//������   ת��������
//������
//�޸���ʷ��
//**************************************************

#ifndef RGTRANSCODE_H
#define RGTRANSCODE_H

#include <QObject>

class RGtransCode : public QObject
{
    Q_OBJECT

public:
    //ת������
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
    //����ʵʱ����״̬����
    void emitRGTCStatus(QString contents);

};

#endif // RGTRANSCODE_H
