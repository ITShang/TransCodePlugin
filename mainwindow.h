//**************************************************
//��Ȩ��   RenGu Company
//�ļ����� mainwindow.h
//���ߣ�   �г�     Version��1.0    Date��2016.12.06
//������   ����������
//������
//�޸���ʷ��
//2016.12.06:sc:    ����ѡ�����ļ���ӣ�����ѡ���ļ�����ӣ��ǵݹ飻
//                  ��Ӷ�ѡ�ͷ�ѡ�����Ʊ�����ݸ��£����ת�����ɾ����Ŀ���ܣ�
//2016.12.12:sc:    ��ӱ��������͵�һ�и�ѡ��ȥ�����ð�ť�����Ԥ����ť��
//**************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "myheader.h"

class QFileInfo;
class PreLookFile;          //�����ļ�Ԥ����
class QTableWidgetItem;
class MyTableModel;
class QLabel;
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void addNewFile(bool);
    void addNewDir(bool);
    void deleteFile(bool);
    void startTransFileCode(bool);
    void setTargetFilePath(bool);
    void setUiStatus(QString);
    void selectAllItems(bool);
    void selectAllOtherItems(bool);
    void changeSourceCode(int);
    void changeTargetCode(int);
    void showStatus(QModelIndex);
    void setLocalLook(bool);
    void setAllSelectedItemCode(bool);
    void clickTableView(QModelIndex);

private:
    void initConnect();
    void initToolTips();
    void initUiStatus();
    void initModel();
    void initView();
    void clearTransSuccList();

    bool checkSameFileOpened(QString fileName);
    bool checkSameFileOpened(QFileInfo info);
    bool checkFileExited(TransFile *tFile);
    void updateTable();
    void setDefaultTargetPath();
    void checkIsSelectedItem();



    PreLookFile *preLook;           //Ԥ���ļ����ݴ������ݳ�Ա
    QString targetFilePath;         //�ļ����·��
    QString LastFilePath;           //�����һ��ѡ����ļ���·��
    QString LastDirPath;            //��һ��ѡ���ļ���ʱ��·��
    Ui::MainWindow *ui;             //���������ݳ�Ա
    QLabel *label;
    QList<TransFile *> transList;   //��Ҫ���и�ʽת������Ϣ����
                                    //�ü�����Ϣͨ������������ʾ

    QStringList format;             //����ȡ���ļ���ʽ�б�
    MyTableModel *model;            //model
};

#endif // MAINWINDOW_H
