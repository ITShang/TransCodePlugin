//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� myheader.h
//���ߣ�   �г�     Version��1.0    Date��2016.12.13
//������   �Զ�����������
//������
//�޸���ʷ��
//
//**************************************************
#ifndef MYHEADER
#define MYHEADER
#include <QString>
#include <QObject>

//�����������
//�������Ҫ��ʾ����Ϣ��������ʾ����Ҫ�ļ���ʽת������Ϣ
struct TransFile
{
    Qt::CheckState isSelected;      //ѡ��״̬
    int isTransSucc;                //���ļ��Ƿ��Ѿ���ת������
    QString fileName;               //�ļ���
    QString fileType;               //�ļ�����
    QString sourceCode;             //Դ�ļ������ʽ����ȡ�������ļ���ʽ��
    QString targetCode;             //��Ҫת���ɵ�Ŀ���ļ������ʽ
    QString fileSize;               //�ļ���С
    QString filePath;               //�ļ�����·��,�������ļ���
    QString fileAbPath;             //�ļ�����·��,�����ļ���
};

#endif // MYHEADER

