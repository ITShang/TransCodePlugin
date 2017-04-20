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

//设置需要显示的数据原型
void PreLookFile::setFieData(const QByteArray data)
{
    fileData = data;
    updateTextEdit();
}

//设置默认的读取格式，由之前的格式判断方法得出结论
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

//根据设置的读取格式，将原型数据转换显示到界面控件
void PreLookFile::updateTextEdit()
{
    QTextCodec *codec = QTextCodec::codecForName(sourceCode.toUtf8());

    QTextStream in(&fileData);
    in.setAutoDetectUnicode(false);
    in.setCodec(codec);
    decodedStr = in.readAll();

    ui->plainTextEdit->setPlainText(decodedStr);
}

//设置读取原型数据的格式
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

//设置转换的目标文件的格式，即需要转换什么格式的文件
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

//添加本次的设置的文件配置到主界面，并关掉本次这个设置界面
void PreLookFile::addThisFile(bool)
{
    close();
}

PreLookFile::~PreLookFile()
{
    delete ui;
}
