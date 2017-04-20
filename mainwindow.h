//**************************************************
//版权：   RenGu Company
//文件名： mainwindow.h
//作者：   尚超     Version：1.0    Date：2016.12.06
//描述：   程序主界面
//其它：
//修改历史：
//2016.12.06:sc:    可以选择多个文件添加；可以选择文件夹添加，非递归；
//                  添加多选和反选；完善表格数据更新；添加转换后的删除条目功能；
//2016.12.12:sc:    添加表格下拉框和第一列复选框；去掉设置按钮；添加预览按钮；
//**************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "myheader.h"

class QFileInfo;
class PreLookFile;          //声明文件预览类
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



    PreLookFile *preLook;           //预览文件内容窗口数据成员
    QString targetFilePath;         //文件输出路径
    QString LastFilePath;           //最近的一次选择的文件的路径
    QString LastDirPath;            //上一次选择文件夹时的路径
    Ui::MainWindow *ui;             //主界面数据成员
    QLabel *label;
    QList<TransFile *> transList;   //需要进行格式转换的信息集合
                                    //该集合信息通过主界面表格显示

    QStringList format;             //待读取的文件格式列表
    MyTableModel *model;            //model
};

#endif // MAINWINDOW_H
