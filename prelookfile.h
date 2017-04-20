//**************************************************
//版权：   RenGu Company
//文件名： prelookfile.h
//作者：   尚超     Version：1.0    Date：2016.12.06
//描述：   预览和设置文件格式界面
//其它：
//修改历史：
//2016.12.06:尚超：    完善自动识别文件编码格式功能；
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
