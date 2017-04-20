//**************************************************
//版权：   RenGu Company
//文件名： myheader.h
//作者：   尚超     Version：1.0    Date：2016.12.13
//描述：   自定义数据类型
//其它：
//修改历史：
//
//**************************************************
#ifndef MYHEADER
#define MYHEADER
#include <QString>
#include <QObject>

//表格数据内容
//表格中需要显示的信息，集中显示了需要文件格式转换的信息
struct TransFile
{
    Qt::CheckState isSelected;      //选中状态
    int isTransSucc;                //该文件是否已经被转换过了
    QString fileName;               //文件名
    QString fileType;               //文件类型
    QString sourceCode;             //源文件编码格式（读取正常的文件格式）
    QString targetCode;             //需要转换成的目标文件编码格式
    QString fileSize;               //文件大小
    QString filePath;               //文件绝对路径,不包含文件名
    QString fileAbPath;             //文件绝对路径,包含文件名
};

#endif // MYHEADER

