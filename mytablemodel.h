//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� mytablemodel.h
//���ߣ�   �г�     Version��1.0    Date��2016.12.13
//������   �Զ���Model��ʵ���Զ���ı����������
//������
//�޸���ʷ��
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

    void insertMyData(TransFile *data);     //�����Ҫ��ʾ������
    void removeMyData(int index);           //ɾ��ָ��������
    int removeMyDatas();                   //ɾ��ѡ�е�ѡ��

    QList<TransFile *> getFileList();       //��ȡModel�е�����
    void selectAll(int state);              //ȫѡ
    void shiftOther(int state);             //��ѡ
    bool getSelectedAllStatus();            //��ȡ��ǰ��ȫѡ״̬
    QString showStatus(QModelIndex &index); //��ʾ��Ҫ��ʾ����ʾ��Ϣ
    int getSelectedCount();                 //���ص�ǰ�Ѿ�ѡ�е���Ŀ����
    void setSelectedItemCode(QString);      //����ѡ����Ŀ��Ŀ���ʽΪָ���ı����ʽ

    int checkFileSizeType(int);
    float calculateFileSize(int,int);

private:
    QList<TransFile *> transContents;       //model��Ҫ������µ�����
    bool isSelectedAll;                     //��ǰ�Ƿ���ȫѡ״̬

};

#endif // MYTABLEMODEL_H
