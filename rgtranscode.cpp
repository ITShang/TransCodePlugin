#include "rgtranscode.h"
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QDir>
#include <QDebug>

RGtransCode::RGtransCode()
{

}

//读取指定文件的内容
//path:             源文件全路径
//sourceCode:       读取文件正常的编码格式
//返回文件的内容
QString RGtransCode::readFile(QString path, QString sourceCode)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly))
    {
        return QLatin1String("");
    }

    QTextStream reader(&file);
    reader.setCodec(QTextCodec::codecForName(sourceCode.toLocal8Bit()));
    QString text = reader.readAll();

    file.close();
    return text;
}

//转换方法的对外入口
//sourceFilepath:   源文件全路径
//sourceCode:       读取文件正常的编码格式
//targetCode:       转换后的目标文件编码格式
//targrtFilePath:   目标文件夹路径
//TtansType:        转换类型
void RGtransCode::transFileCode(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath, TransType TtansType)
{
    if(TtansType == TYPE_REPLACEOLDFILE)
    {
        replaceOldFile(sourceFilepath,sourceCode,targetCode,targrtFilePath);
    }
    else if(TtansType == TYPE_CREATENEWFILE)
    {
        createNewFile(sourceFilepath,sourceCode,targetCode,targrtFilePath);
    }
    else if(TtansType == TYPE_CREATEOTHERFILE)
    {
        createOtherFile(sourceFilepath,sourceCode,targetCode,targrtFilePath);
    }
}

void RGtransCode::replaceOldFile(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath)
{
    emit emitRGTCStatus(QLatin1String("开始转换"));

    QString contents = readFile(sourceFilepath,sourceCode);
    QFileInfo info(sourceFilepath);
    QString fileName = targrtFilePath + QDir::separator() + info.fileName();

    emit emitRGTCStatus(QLatin1String("读取正常"));

    QFile file(sourceFilepath);

    if(!file.open(QFile::WriteOnly))
    {
        emit emitRGTCStatus(QLatin1String("文件打开失败"));
        return;
    }

    file.close();


    QFile file_new(fileName);
    if(!file_new.open(QFile::WriteOnly))
    {
        emit emitRGTCStatus(QLatin1String("文件创建失败"));
        return;
    }
    QTextStream writer(&file_new);
    writer.setCodec(QTextCodec::codecForName(targetCode.toLocal8Bit()));
    writer<<contents;

    file_new.close();

    emit emitRGTCStatus(QLatin1String("转换成功，路径：")+fileName);
}

void RGtransCode::createNewFile(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath)
{
    emit emitRGTCStatus(QLatin1String("开始转换"));

    QString contents = readFile(sourceFilepath,sourceCode);
    QFileInfo info(sourceFilepath);
    QString fileName = targrtFilePath + QDir::separator() + info.fileName();

    QFile file(fileName);
    if(!file.open(QFile::WriteOnly))
    {
        emit emitRGTCStatus(QLatin1String("文件创建失败"));
        return;
    }

    QTextStream writer(&file);
    writer.setCodec(QTextCodec::codecForName(targetCode.toLocal8Bit()));
    writer<<contents;
    file.close();

    emit emitRGTCStatus(QLatin1String("转换成功，路径：")+fileName);
}

void RGtransCode::createOtherFile(QString sourceFilepath, QString sourceCode, QString targetCode, QString targrtFilePath)
{
    emit emitRGTCStatus(QLatin1String("开始转换"));

    int existedNum = 1;
    bool exitsted = false;
    QString contents = readFile(sourceFilepath,sourceCode);
    QFileInfo info(sourceFilepath);
    QString fileName = targrtFilePath
            + QDir::separator()
            + info.baseName()
            + QLatin1String("(")
            + targetCode
            + QLatin1String(").")
            + info.suffix();


    while( !exitsted )
    {
        QFile file(fileName);
        if(file.exists())
        {
            existedNum++;
            fileName = targrtFilePath
                    + QDir::separator()
                    + info.baseName()
                    + QLatin1String("(" )
                    + targetCode
                    + QLatin1String(")(")
                    + QString::number(existedNum)
                    + QLatin1String(").")
                    + info.suffix();
        }
        else
        {
            exitsted = true;
        }
        file.close();
    }

    QFile file_New(fileName);
    if(!file_New.open(QFile::WriteOnly))
    {
        emit emitRGTCStatus(QLatin1String("文件创建失败"));
        return;
    }

    QTextStream writer(&file_New);
    writer.setCodec(QTextCodec::codecForName(targetCode.toLocal8Bit()));
    writer<<contents;
    file_New.close();
    existedNum = -1;

    emit emitRGTCStatus(QLatin1String("转换成功，路径：")+fileName);
}

//自动判断文件的格式
QString RGtransCode::checkFileCode(QString sourceFilepath)
{
    QString returnCode = QLatin1String("GB2312");
    QFile file(sourceFilepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        return QLatin1String("读取失败");
    }

    QByteArray data = file.read(file.size());
    if(isutf8(data))
    {
        returnCode = QLatin1String("UTF-8");
    }
    else
    {
        returnCode = QLatin1String("GB2312");
    }
    file.close();
    return returnCode;
}


//判断是否是UTF-8文本
bool RGtransCode::isutf8(QByteArray data)
{
    int i = 0;
    int size = data.size();

    while(i<size)
    {
        int step = 0;

        if((data[i] & 0x80) == 0x00)                         //0x80 = 128
        {
            step = 1;
        }
        else if((data[i] & 0xe0) == 0xc0)                    //0xe0 = 224, 0xc0 = 192
        {
            if(i + 1 >= size) return false;
            if((data[i + 1] & 0xc0) != 0x80) return false;   //0xc0 = 192, 0x80 = 128

            step = 2;
        }
        else if((data[i] & 0xf0) == 0xe0)                    //0xf0 = 240, 0xe0 = 224
        {
            if(i + 2 >= size) return false;
            if((data[i + 1] & 0xc0) != 0x80) return false;
            if((data[i + 2] & 0xc0) != 0x80) return false;

            step = 3;
        }
        else
        {
            return false;
        }

        i += step;
    }

    if(i == size) return true;

    return false;
}



