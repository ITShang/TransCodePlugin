#include "mypushdelegate.h"
#include <QComboBox>
#include <QDebug>

MyPushDelegate::MyPushDelegate(QObject *parent):
    QItemDelegate(parent)
{

}


//创建QComBox控件
QWidget* MyPushDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QComboBox *editor = new QComboBox(parent);
    editor->addItem(QLatin1String("UTF-8"));
    editor->addItem(QLatin1String("GB2312"));

    return editor;
}

//设置QComBox控件的显示
void MyPushDelegate::setEditorData(QWidget *editor,
                                       const QModelIndex &index) const
{
    QString text = index.model()->data(index,Qt::DisplayRole).toString();
    QComboBox * limitComBox = static_cast<QComboBox*>(editor);
    int tindex = limitComBox->findText(text);
    limitComBox->setCurrentIndex(tindex);
}

//根据QComBox控件中显示的值修改model中的值
void MyPushDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(model)
    Q_UNUSED(index)
    //设置不能修改下拉框的值
//    QComboBox *limitComBox = static_cast<QComboBox*>(editor);
//    QString text = limitComBox->currentText();
//    model->setData(index,text,Qt::EditRole);
}

//更新QComBox控件的尺寸、位置
void MyPushDelegate::updateEditorGeometry(QWidget *editor,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}


