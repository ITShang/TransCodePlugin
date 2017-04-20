//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� prelookfile.h
//���ߣ�   �г�     Version��1.0    Date��2016.12.06
//������   Ԥ���������ļ���ʽ����
//������
//�޸���ʷ��
//2016.12.06:�г���    �����Զ�ʶ���ļ������ʽ���ܣ�
//**************************************************

#ifndef PRELOOKFILE_H
#define PRELOOKFILE_H

#include <QDialog>

namespace Ui {
class PreLookFile;
}

class PreLookFile : public QDialog
{
    Q_OBJECT

public:
    explicit PreLookFile(QWidget *parent = 0);
    ~PreLookFile();
    void setFilePath(QString path){filePath = path;}
    void setFieData(const QByteArray data);
    void setReadCode(QString code);
    QString getSourceCode(){return sourceCode;}
    QString getTargetCode(){return targetCode;}

private slots:
    void setSourceCode(int);
    void setTargetCode(int);
    void addThisFile(bool);

private:
    void updateTextEdit();
    bool isutf8(QByteArray data);
    QByteArray fileData;
    QString decodedStr;
    QString filePath;
    QString sourceCode;
    QString targetCode;
    Ui::PreLookFile *ui;

signals:

};

#endif // PRELOOKFILE_H
