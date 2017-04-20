//**************************************************
//版权：   RenGu Company
//文件名： mytablemodel.h
//作者：   尚超     Version：1.0    Date：2016.12.13
//描述：   自定义Model，实现自定义的表格数据索引
//其它：
//修改历史：
//
//**************************************************
#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>
#include "myheader.h"

class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MyTableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void insertMyData(TransFile *data);     //添加需要显示的数据
    void removeMyData(int index);           //删除指定的数据
    int removeMyDatas();                   //删除选中的选项

    QList<TransFile *> getFileList();       //获取Model中的数据
    void selectAll(int state);              //全选
    void shiftOther(int state);             //反选
    bool getSelectedAllStatus();            //获取当前的全选状态
    QString showStatus(QModelIndex &index); //显示需要显示的提示信息
    int getSelectedCount();                 //返回当前已经选中的条目数量
    void setSelectedItemCode(QString);      //设置选中条目的目标格式为指定的编码格式

    int checkFileSizeType(int);
    float calculateFileSize(int,int);

private:
    QList<TransFile *> transContents;       //model需要保存更新的内容
    bool isSelectedAll;                     //当前是否是全选状态

};

#endif // MYTABLEMODEL_H
