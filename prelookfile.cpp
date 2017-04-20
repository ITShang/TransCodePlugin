#include "prelookfile.h"
#include "ui_prelookfile.h"

#include <QFileDialog>
#include <QDir>
#include <QTextCodec>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include "rgtranscode.h"


PreLookFile::PreLookFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreLookFile)
{
    ui->setupUi(this);
    sourceCode = QLatin1String("UTF-8");
    targetCode = QLatin1String("UTF-8");

    connect(ui->sourceCode,SIGNAL( currentIndexChanged(int) ),
            this,SLOT( setSourceCode(int) ));
    connect(ui->targetCode,SIGNAL( currentIndexChanged(int) ),
            this,SLOT( setTargetCode(int) ));
    connect(ui->add,SIGNAL( clicked() ),
            this,SLOT( accept() ));
}

//������Ҫ��ʾ������ԭ��
void PreLookFile::setFieData(const QByteArray data)
{
    fileData = data;
    updateTextEdit();
}

//����Ĭ�ϵĶ�ȡ��ʽ����֮ǰ�ĸ�ʽ�жϷ����ó�����
void PreLookFile::setReadCode(QString code)
{
    if(code == QLatin1String("UTF-8"))
    {
        ui->sourceCode->setCurrentIndex(0);
        ui->targetCode->setCurrentIndex(1);
    }
    else
    {
        ui->sourceCode->setCurrentIndex(1);
        ui->targetCode->setCurrentIndex(0);
    }
}

//�������õĶ�ȡ��ʽ����ԭ������ת����ʾ������ؼ�
void PreLookFile::updateTextEdit()
{
    QTextCodec *codec = QTextCodec::codecForName(sourceCode.toUtf8());

    QTextStream in(&fileData);
    in.setAutoDetectUnicode(false);
    in.setCodec(codec);
    decodedStr = in.readAll();

    ui->plainTextEdit->setPlainText(decodedStr);
}

//���ö�ȡԭ�����ݵĸ�ʽ
void PreLookFile::setSourceCode(int index)
{
    switch (index)
    {
    case 0:
        sourceCode = QLatin1String("UTF-8");
        break;
    case 1:
        sourceCode = QLatin1String("GB2312");
        break;
    default:
        sourceCode = QLatin1String("UTF-8");
        break;
    }
    updateTextEdit();
}

//����ת����Ŀ���ļ��ĸ�ʽ������Ҫת��ʲô��ʽ���ļ�
void PreLookFile::setTargetCode(int index)
{
    switch (index)
    {
    case 0:
        targetCode = QLatin1String("UTF-8");
        break;
    case 1:
        targetCode = QLatin1String("GB2312");
        break;
    default:
        targetCode = QLatin1String("UTF-8");
        break;
    }
}

//��ӱ��ε����õ��ļ����õ������棬���ص�����������ý���
void PreLookFile::addThisFile(bool)
{
    close();
}

PreLookFile::~PreLookFile()
{
    delete ui;
}
