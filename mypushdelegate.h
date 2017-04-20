//**************************************************
//版权：   RenGu Company
//文件名： mypushdelegate.h
//作者：   尚超     Version：1.0    Date：2016.12.13
//描述：   自定义编码格式下拉框委托
//其它：
//修改历史：
//
//**************************************************
#ifndef MYPUSHDELEGATE_H
#define MYPUSHDELEGATE_H

#include <QItemDelegate>

class MyPushDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    MyPushDelegate(QObject *parent=0);

    QWidget*createEditor(QWidget*parent,
                         const QStyleOptionViewItem&option,
                         const QModelIndex&index)const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;


};

#endif // MYPUSHDELEGATE_H
