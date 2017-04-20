#include "mypushdelegate.h"
#include <QComboBox>
#include <QDebug>

MyPushDelegate::MyPushDelegate(QObject *parent):
    QItemDelegate(parent)
{

}


//����QComBox�ؼ�
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

//����QComBox�ؼ�����ʾ
void MyPushDelegate::setEditorData(QWidget *editor,
                                       const QModelIndex &index) const
{
    QString text = index.model()->data(index,Qt::DisplayRole).toString();
    QComboBox * limitComBox = static_cast<QComboBox*>(editor);
    int tindex = limitComBox->findText(text);
    limitComBox->setCurrentIndex(tindex);
}

//����QComBox�ؼ�����ʾ��ֵ�޸�model�е�ֵ
void MyPushDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(model)
    Q_UNUSED(index)
    //���ò����޸��������ֵ
//    QComboBox *limitComBox = static_cast<QComboBox*>(editor);
//    QString text = limitComBox->currentText();
//    model->setData(index,text,Qt::EditRole);
}

//����QComBox�ؼ��ĳߴ硢λ��
void MyPushDelegate::updateEditorGeometry(QWidget *editor,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}


