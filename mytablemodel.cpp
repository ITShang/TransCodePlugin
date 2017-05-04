#include "mytablemodel.h"
#include <QColor>
#include <QDebug>

MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return transContents.count();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 1:
            return transContents.at(index.row())->fileName;
            break;
        case 2:
            return transContents.at(index.row())->fileType;
            break;
        case 3:
            return transContents.at(index.row())->sourceCode;
            break;
        case 4:
            return transContents.at(index.row())->targetCode;
            break;
        case 5:
            {
                int sizes = transContents.at(index.row())->fileSize.toInt();
                int Kb = sizes/1024;
                int Mb = Kb/1024;
                int Gb = Mb/1024;
                QString sizeShow;
                if(Kb < 1)
                {
                    //Byte��ʾ
                    sizeShow = QString().sprintf("%d B",sizes);
                }
                else if( Mb<1)
                {
                    //Kb��ʾ
                    sizeShow = QString().sprintf("%0.3f Kb",(float)sizes/1024);
                }
                else if(Gb<1)
                {
                    //Mb��ʾ
                    sizeShow = QString().sprintf("%0.3f Mb",(float)sizes/1024/1024);
                }
                else
                {
                    //Gb��ʾ
                    sizeShow = QString().sprintf("%0.3f Gb",(float)sizes/1024/1024/1024);
                }
                return sizeShow;
            }
            break;
        case 6:
            return transContents.at(index.row())->filePath;
            break;
        }
    }
    else if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if (transContents.at(index.row())->isSelected)
            {
                return transContents.at(index.row())->isSelected == Qt::Checked ? Qt::Checked : Qt::Unchecked;
            }
            return Qt::Unchecked;
        }
    }
    else if(role == Qt::ToolTipRole)
    {
        return transContents.at(index.row())->fileAbPath;
    }
    else if(role==Qt::TextColorRole)
    {
        if(transContents.at(index.row())->isTransSucc == 1)
        {
            return QColor(0,0,255,255);
        }
    }

    return QVariant();
}

bool MyTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return false;
    }

    if(role == Qt::CheckStateRole && index.column() == 0)
    {
        if(value == Qt::Checked)
        {
            transContents.at(index.row())->isSelected  = Qt::Checked;
        }
        else
        {
            transContents.at(index.row())->isSelected = Qt::Unchecked;
        }
    }

    if(role==Qt::EditRole)
    {
        switch(index.column())
        {
        case 1:
            transContents.at(index.row())->fileName = value.toString();
            break;
        case 2:
            transContents.at(index.row())->fileType = value.toString();
            break;
        case 3:
            transContents.at(index.row())->sourceCode = value.toString();
            break;
        case 4:
            transContents.at(index.row())->targetCode = value.toString();
            break;
        case 5:
            transContents.at(index.row())->fileSize = value.toString();
            break;
        case 6:
            transContents.at(index.row())->filePath = value.toString();
            break;
        }
    }
    emit dataChanged(index,index);
    return true;

}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    //v2.0
    if (!index.isValid())
        return 0;

    if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else if(index.column() == 3)
    {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }
    else if(index.column() == 4)
    {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }

    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//�����Ҫ��ʾ������
void MyTableModel::insertMyData(TransFile *data)
{
    beginInsertRows(QModelIndex(),0,0);
    TransFile *newdata = new TransFile;
    newdata->fileAbPath = data->fileAbPath;
    newdata->fileName = data->fileName;
    newdata->filePath = data->filePath;
    newdata->fileSize = data->fileSize;
    newdata->fileType = data->fileType;
    newdata->isSelected = data->isSelected;
    newdata->sourceCode = data->sourceCode;
    newdata->targetCode = data->targetCode;
    transContents.append(newdata);
    endInsertRows();
}

//�Ƴ�����
void MyTableModel::removeMyData(int index)
{
    beginRemoveRows(QModelIndex(),0,0);
    TransFile *newdata = transContents.at(index);
    delete newdata;
    newdata=NULL;
    transContents.removeAt(index);

    endRemoveRows();
}

//ɾ��ѡ�е�ѡ��
int MyTableModel::removeMyDatas()
{
    int index =0;
    for(int i=transContents.count()-1;i>=0;i--)
    {
        if(transContents.at(i)->isSelected == Qt::Checked)
        {
            index ++;
        }
    }
    if(index==0)
    {
        return 0;
    }

    beginRemoveRows(QModelIndex(),0,index-1);
    for(int i=transContents.count()-1;i>=0;i--)
    {
        if(transContents.at(i)->isSelected == Qt::Checked)
        {
            TransFile *tFile = transContents.at(i);
            delete tFile;
            tFile = NULL;
            transContents.removeAt(i);
        }
    }

    endRemoveRows();
    return index;
}

//��ȡModel�е�����
QList<TransFile *> MyTableModel::getFileList()
{
    return transContents;
}

//ȫѡ
void MyTableModel::selectAll(int state)
{
    switch(state)
    {
    case 0:
        for(int i=0;i<transContents.count();i++)
        {
            transContents.at(i)->isSelected = Qt::Unchecked;
        }
        break;
    case 2:
        for(int i=0;i<transContents.count();i++)
        {
            transContents.at(i)->isSelected = Qt::Checked;
        }
        break;
    }
    emit dataChanged(QModelIndex(),QModelIndex());
}

//��ѡ
void MyTableModel::shiftOther(int state)
{
    Q_UNUSED(state)
    for(int i=0;i<transContents.count();i++)
    {
        if(transContents.at(i)->isSelected == Qt::Checked)
        {
            transContents.at(i)->isSelected = Qt::Unchecked;
        }
        else if(transContents.at(i)->isSelected == Qt::Unchecked)
        {
            transContents.at(i)->isSelected = Qt::Checked;
        }
    }

    emit dataChanged(QModelIndex(),QModelIndex());
}

// ��ͷ����
QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            switch(section)
            {
            case 1:
                return QObject::tr("Name");
                break;
            case 2:
                return QObject::tr("Type");
                break;
            case 3:
                return QObject::tr("Soutce Code");
                break;
            case 4:
                return QObject::tr("Target Code");
                break;
            case 5:
                return QObject::tr("Size");
                break;
            case 6:
                return QObject::tr("Path");
                break;
            }
        }
        if(orientation == Qt::Vertical)
        {
            return section+1;
        }
    }
    default:
        return QVariant();
    }

    return QVariant();

}

//��ȡ��ǰ��ȫѡ״̬��trueΪ��ǰ��ȫѡ
bool MyTableModel::getSelectedAllStatus()
{
    for(int i=0;i<transContents.count();i++)
    {
        if(transContents.at(i)->isSelected == Qt::Unchecked)
        {
            return false;
        }
    }
    return true;
}

//��ʾ��Ҫ��ʾ����ʾ��Ϣ
QString MyTableModel::showStatus(QModelIndex &index)
{
    return transContents.at(index.row())->fileAbPath;
}

//���ص�ǰ�Ѿ�ѡ�е���Ŀ����
int MyTableModel::getSelectedCount()
{
    int nums = 0;
    for(int i=0;i<transContents.count();i++)
    {
        if(transContents.at(i)->isSelected == Qt::Checked)
        {
            nums++;
        }
    }
    return nums;
}

//����ѡ����Ŀ��Ŀ���ʽΪָ���ı����ʽ
void MyTableModel::setSelectedItemCode(QString code)
{
    for(int i=0;i<transContents.count();i++)
    {
        if(transContents.at(i)->isSelected == Qt::Checked)
        {
            transContents.at(i)->targetCode = code;
        }
    }
    emit dataChanged(QModelIndex(),QModelIndex());
}


//�ж���ʾ���ļ���λ
int MyTableModel::checkFileSizeType(int byteSize)
{
    int type = 0;
    int Kb=byteSize/1024;
    int Mb=Kb/1024;
    int Gb=Mb/1024;
    if(Kb < 0)
    {
        type = 0;       //Byte��ʾ
    }
    else if(Mb<0)
    {
        type = 1;       //Kb��ʾ
    }
    else if(Gb<0)
    {
        type = 2;       //Mb��ʾ
    }
    else
    {
        type = 3;       //Gb��ʾ
    }

    return type;
}

//������ʾ�ĵ�λ��������Ҫ��ʾ������
float MyTableModel::calculateFileSize(int type,int byteSize)
{
    float size = 0.0;
    switch(type)
    {
    case 0:
        size = (float)byteSize;                    //Byte
        break;
    case 1:
        size = (float)byteSize/1024;               //Kb
        break;
    case 2:
        size = (float)byteSize/1024/1024;          //Mb
        break;
    case 3:
        size = (float)byteSize/1024/1024/1024;     //Gb
        break;
    }
    return size;
}



