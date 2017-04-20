#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rgtranscode.h"
#include "prelookfile.h"
#include "mytablemodel.h"
#include "mypushdelegate.h"
#include "myheader.h"
#include <coreplugin/documentmanager.h>

#include <QFileDialog>
#include <QFileInfo>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>
#include <QProcess>
#include <QModelIndex>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_MAC
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
#else
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    ui->statusBar->setStyleSheet(QObject::tr("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255,0);"));
    label = new QLabel(this);
    label->setStyleSheet(QObject::tr("color: rgb(0, 0, 0);"));
    ui->statusBar->addWidget(label);

    initToolTips();
    initUiStatus();
    initModel();
    initView();
    initConnect();
    setDefaultTargetPath();

    ui->targetFilePath->setFocusPolicy(Qt::NoFocus);
    preLook = new PreLookFile(this);

}

//构造函数中初始化槽函数链接
void MainWindow::initConnect()
{
    connect(ui->addNewFile,SIGNAL( clicked(bool) ),
            this,SLOT( addNewFile(bool) ));
    connect(ui->addDir,SIGNAL( clicked(bool) ),
            this,SLOT( addNewDir(bool) ));
    connect(ui->deleteFileItem,SIGNAL( clicked(bool) ),
            this,SLOT( deleteFile(bool) ));
    connect(ui->setLocalLook,SIGNAL( clicked(bool) ),
            this,SLOT( setLocalLook(bool) ));
    connect(ui->startTransCode,SIGNAL( clicked(bool) ),
            this,SLOT( startTransFileCode(bool) ));
    connect(ui->setTargetFilePath,SIGNAL( clicked(bool) ),
            this,SLOT( setTargetFilePath(bool) ));
    connect(ui->fileTableView,SIGNAL( clicked(QModelIndex) ),
            this,SLOT( showStatus(QModelIndex) ));
    connect(ui->setAllCode,SIGNAL( clicked(bool) ),
            this,SLOT( setAllSelectedItemCode(bool) ));

    connect(ui->fileTableView,SIGNAL( clicked(QModelIndex) ),
            this,SLOT( clickTableView(QModelIndex) ));
    //全选
    connect(ui->selectAll,SIGNAL( clicked(bool) ),
            this,SLOT( selectAllItems(bool) ));
    //反选
    connect(ui->selectAllOther,SIGNAL( clicked(bool) ),
            this,SLOT( selectAllOtherItems(bool) ));

}

//初始化界面的控件提示信息
void MainWindow::initToolTips()
{
    ui->addNewFile->setToolTip(tr("打开文件"));
    ui->addDir->setToolTip(tr("打开文件夹"));
    ui->deleteFileItem->setToolTip(tr("删除文件"));
    ui->setLocalLook->setToolTip(tr("查看文件内容"));
    ui->startTransCode->setToolTip(tr("开始转换选中的文件"));
}

//初始化数据成员
void MainWindow::initUiStatus()
{
    format += QObject::tr("*.h");
    format += QObject::tr("*.cpp");
    format += QObject::tr("*.txt");

    LastFilePath = QDir::currentPath();
    LastDirPath = QDir::currentPath();

    ui->selectAll->setEnabled(false);
    ui->selectAllOther->setEnabled(false);
    ui->setAllCode->setEnabled(false);
    ui->sameCode->setEnabled(false);
}

//初始化model
void MainWindow::initModel()
{
    model = new MyTableModel();
}

//初始化View
void MainWindow::initView()
{
    ui->fileTableView->setModel(model);
    ui->fileTableView->setItemDelegateForColumn(3,new MyPushDelegate(ui->fileTableView));
    ui->fileTableView->setItemDelegateForColumn(4,new MyPushDelegate(ui->fileTableView));

    ui->fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fileTableView->horizontalHeader()->setStretchLastSection(true);
}

//添加文件按钮槽函数
void MainWindow::addNewFile(bool)
{

    QString formatContent;
    for(int i=0;i<format.count();i++)
    {
        formatContent += format.at(i);
        formatContent += QObject::tr(" (");
        formatContent += format.at(i);
        formatContent += QObject::tr(")");
        if(i!=format.count()-1)
        {
            formatContent += QObject::tr(";;");
        }
    }
    setUiStatus(QObject::tr("选择文件"));
    QStringList fileNames = QFileDialog::getOpenFileNames(this,tr("选择文件"),
                                                    LastFilePath,
                                                    formatContent);

    if(fileNames.isEmpty())
    {
        setUiStatus(QObject::tr("未选择文件"));
    }
    else
    {
        setUiStatus(QString(QObject::tr("选择了%1个文件")).arg(fileNames.count()));
    }

    int insertNums=0;
    foreach(QString fileName,fileNames)
    {
        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
            {
                QMessageBox::warning(this, QObject::tr("Codecs"),
                                     QObject::tr("未能正常打开文件: %1:\n%2")
                                     .arg(fileName)
                                     .arg(file.errorString()));
                return;
            }
            preLook->show();
            QByteArray data = file.readAll();
            preLook->setWindowTitle(fileName);
            preLook->setFieData(data);

            //检测字符编码格式
            RGtransCode rgTC;
            QString code = rgTC.checkFileCode(fileName);
            preLook->setReadCode(code);

            if (preLook->exec())
            {
                QFileInfo info(file);
                if(checkSameFileOpened(info))
                {
                    QMessageBox::warning(this,QObject::tr("提示"),
                                         QObject::tr("文件%1已经添加:").arg(file.fileName()));
                    return;
                }

                insertNums++;
                setUiStatus(QObject::tr("添加文件到列表：%1").arg(info.absoluteFilePath()));
                TransFile *tFile = new TransFile;
                tFile->isSelected = Qt::Unchecked;
                tFile->fileName = info.baseName();
                tFile->fileType = info.suffix();
                tFile->sourceCode = preLook->getSourceCode();
                tFile->targetCode = preLook->getTargetCode();
                tFile->fileSize = QString::number(info.size());
                tFile->filePath = info.absolutePath();
                tFile->fileAbPath = info.absoluteFilePath();

                LastFilePath = tFile->filePath;
                model->insertRow(0);
                model->insertMyData(tFile);
            }
        }
    }
    if(insertNums>0)
    {
        ui->selectAll->setEnabled(true);
        ui->selectAllOther->setEnabled(true);
    }
    setUiStatus(QObject::tr("添加了%1个文件到列表").arg(insertNums));
}

//添加一个文件夹
void MainWindow::addNewDir(bool)
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                 tr("选择文件夹"),
                                                 LastDirPath);
    if(path == NULL)
    {
        return;
    }
    LastDirPath = path;
    setUiStatus(QObject::tr("选择文件夹:%1").arg(LastDirPath));
    QDir dir(path);

    int insertNums=0;
    foreach(QString fileName,dir.entryList(format,QDir::Files))
    {
        if(fileName == NULL)
        {
            break;
        }
        fileName = path + QDir::separator() + fileName;
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly))
        {
            QMessageBox::warning(this, QObject::tr("Codecs"),
                                 QObject::tr("未能正常打开文件 %1:\n%2")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        QByteArray data = file.readAll();
        preLook->setWindowTitle(fileName);
        preLook->setFieData(data);

        //检测字符编码格式
        RGtransCode rgTC;
        QString code = rgTC.checkFileCode(fileName);
        preLook->setReadCode(code);

        {
            QFileInfo info(file);
            if(checkSameFileOpened(info))
            {
                QMessageBox::warning(this,QObject::tr("提示"),QObject::tr("文件已经添加！"));
                return;
            }

            TransFile *tFile = new TransFile;
            tFile->isSelected = Qt::Unchecked;
            tFile->fileName = info.baseName();
            tFile->fileType = info.suffix();
            tFile->sourceCode = preLook->getSourceCode();
            tFile->targetCode = preLook->getTargetCode();
            tFile->fileSize = QString::number(info.size());
            tFile->filePath = info.absolutePath();
            tFile->fileAbPath = info.absoluteFilePath();

            insertNums++;
            model->insertRow(0);
            model->insertMyData(tFile);
        }

    }

    if(insertNums>0)
    {
        ui->selectAll->setEnabled(true);
        ui->selectAllOther->setEnabled(true);
    }
    return ;
}

//检查是否已经添加过相同的文件
bool MainWindow::checkSameFileOpened(QString fileName)
{
    QFileInfo info(fileName);
    foreach(TransFile *tFile,model->getFileList())
    {
        if(tFile->fileName == info.baseName())
        {
            if(tFile->fileType == info.suffix())
            {
                if(tFile->filePath == info.absolutePath())
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//检查是否已经添加过相同的文件
bool MainWindow::checkSameFileOpened(QFileInfo info)
{
    foreach(TransFile *tFile,model->getFileList())
    {
        if(tFile->fileName == info.baseName())
        {
            if(tFile->fileType == info.suffix())
            {
                if(tFile->filePath == info.absolutePath())
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//删除文件按钮槽函数
void MainWindow::deleteFile(bool)
{
   int num = model->removeMyDatas();
   setUiStatus(QObject::tr("删除%1个条目").arg(num));

   checkIsSelectedItem();
   if(model->getFileList().count()<=0)
   {
       ui->selectAll->setCheckState(Qt::Unchecked);
       ui->selectAll->setEnabled(false);
       ui->selectAllOther->setCheckState(Qt::Unchecked);
       ui->selectAllOther->setEnabled(false);
   }


}

//设置文件输出路径按钮槽函数
void MainWindow::setTargetFilePath(bool)
{
    if(targetFilePath != NULL)
    {
        setUiStatus(QObject::tr("设置默认存储路径"));
        QString path =  QFileDialog::getExistingDirectory(this,
                                                          tr("设置文件输出路径"),
                                                          targetFilePath);
        if(path == NULL)
        {
            return;
        }
        else
        {
            targetFilePath = path;
        }
        ui->targetFilePath->setText(targetFilePath);
    }

}

//设置默认的输出文件路径
void MainWindow::setDefaultTargetPath()
{
    targetFilePath = QDir::currentPath();
    ui->targetFilePath->setText(targetFilePath);
    setUiStatus(QObject::tr("设置默认存储路径:%1").arg(targetFilePath));
}

//设置状态提示栏的实时显示内容
void MainWindow::setUiStatus(QString contents)
{
//    ui->statusBar->showMessage(contents);
    label->setText(contents);
}

//刷新表格数据
void MainWindow::updateTable()
{
}

//开始格式转换按钮槽函数
void MainWindow::startTransFileCode(bool)
{
    int index = 0;
    clearTransSuccList();
    foreach(TransFile *tFile,model->getFileList())
    {
        if(tFile->isSelected == Qt::Checked)
        {
            index++;
            RGtransCode rgTC;
            connect(&rgTC,SIGNAL( emitRGTCStatus(QString) ),
                    this,SLOT( setUiStatus(QString) ));
            int result = 0;
            if(checkFileExited(tFile))
            {
                if(QMessageBox::Ok == QMessageBox::warning(this,tr("提示"),
                                                           tr("文件已经存在，是否覆盖原文件？"),
                                                           QMessageBox::Ok,QMessageBox::Cancel))
                {
                    result = 1;
                }
                else
                {
                    result = 2;
                }
            }

            if(result == 0)
            {
                //非覆盖性转换，在新目录下创建新文件
                rgTC.transFileCode(tFile->fileAbPath,
                                   tFile->sourceCode,
                                   tFile->targetCode,
                                   targetFilePath,
                                   RGtransCode::TYPE_CREATENEWFILE);
            }
            else if(result  == 1)
            {
                //覆盖性转换，替换源文件
                rgTC.transFileCode(tFile->fileAbPath,
                                   tFile->sourceCode,
                                   tFile->targetCode,
                                   targetFilePath,
                                   RGtransCode::TYPE_REPLACEOLDFILE);
            }
            else if(result == 2)
            {
                //非覆盖性转换，在源文件同级目录下创建新文件
                rgTC.transFileCode(tFile->fileAbPath,
                                   tFile->sourceCode,
                                   tFile->targetCode,
                                   targetFilePath,
                                   RGtransCode::TYPE_CREATEOTHERFILE);
            }
            tFile->isTransSucc = 1;
        }
    }

    if(index == 0)
    {
        QMessageBox::warning(this,
                             QObject::tr("提示"),
                             QObject::tr("没有选择需要转换的文件"));
    }
    else
    {
        QMessageBox::information(this,
                                 QObject::tr("提示"),
                                 QObject::tr("成功转换%1个文件,文件存放路径：\n%2")
                                 .arg(index)
                                 .arg(targetFilePath));
    }
    clearTransSuccList();
    setUiStatus(QString(QObject::tr("成功转换%1个文件")).arg(index));
}

//调用本地默认的软件打开文件
void MainWindow::setLocalLook(bool)
{
    int row = ui->fileTableView->currentIndex().row();
    if(row<0)
    {
        setUiStatus(QObject::tr("未选择对应的预览文件"));
        return;
    }
    QString fileAbPath = model->getFileList().at(row)->fileAbPath;
    //调用本地默认文件浏览器打开文件
    #ifdef Q_OS_MAC
        QProcess *icemProcess=new QProcess;
        QStringList arguments;
        QString bash= QObject::tr("open");
        arguments<<QObject::tr("-e"<<fileAbPath;
        bool r = icemProcess->startDetached(bash,arguments);
        if(r == false)
        {
            QMessageBox::warning(this,tr("提示"),tr("未能正确打开文件，请联系开发人员"));
            setUiStatus(QObject::tr("未能正常浏览文件"));
        }
        else
        {
            setUiStatus(QObject::tr("浏览文件内容:%1").arg(fileAbPath));
        }
    #else
        QDesktopServices look;
        bool r = look.openUrl(QUrl(fileAbPath));
        if(r == false)
        {
            QMessageBox::warning(this,tr("提示"),tr("未能正确打开文件，请联系开发人员"));
            setUiStatus(QObject::tr("未能正常浏览文件"));
        }
        else
        {
            setUiStatus(QObject::tr("浏览文件内容:%1").arg(fileAbPath));
        }
    #endif

}

//检查需要转换的文件是不是在目标路径下有相同的文件
bool MainWindow::checkFileExited(TransFile *tFile)
{
    //如果检查到有相同文件存在，返回true，否则返回false

    QString fileName = tFile->fileName
            + QObject::tr(".")
            + tFile->fileType;
    QFile file(targetFilePath
               + QObject::tr("/")
               + fileName);
    if(file.exists())
    {
        return true;
    }
    else
    {
        return false;
    }
    file.close();
}

//点击全选
void MainWindow::selectAllItems(bool bl)
{
    int state = ui->selectAll->checkState();
    if(bl)
    {
        if(ui->selectAllOther->checkState() == Qt::Checked)
        {
            ui->selectAllOther->setCheckState(Qt::Unchecked);
        }
    }
    else
    {

    }
    model->selectAll(state);
    checkIsSelectedItem();
    setUiStatus(QObject::tr("全选"));
}

//点击反选
void MainWindow::selectAllOtherItems(bool bl)
{
    Q_UNUSED(bl)
    int state = ui->selectAll->checkState();
    model->shiftOther(state);
    if(model->getSelectedAllStatus())
    {
        //为全选状态
        ui->selectAll->setCheckState(Qt::Checked);
    }
    else
    {
        //非全选状态
        ui->selectAll->setCheckState(Qt::Unchecked);
    }

    checkIsSelectedItem();
}

//清空已经转换成功的列表
void MainWindow::clearTransSuccList()
{
}

//改变读取文件格式下拉框
void MainWindow::changeSourceCode(int)
{

}

//改变目标文件格式下拉框
void MainWindow::changeTargetCode(int)
{

}

//显示表格对应的实时提示信息
void MainWindow::showStatus(QModelIndex index)
{
    ui->statusBar->showMessage(model->showStatus(index));
}

//检查是否有选中的条目
void MainWindow::checkIsSelectedItem()
{
    if(model->getSelectedCount()>0)
    {
        ui->setAllCode->setEnabled(true);
        ui->sameCode->setEnabled(true);
    }
    else
    {
        ui->setAllCode->setEnabled(false);
        ui->sameCode->setEnabled(false);
    }
}

//一键设置选中的条目的目标格式
void MainWindow::setAllSelectedItemCode(bool)
{
    model->setSelectedItemCode(ui->sameCode->currentText());
}

//点击表格界面的条目触发事件
void MainWindow::clickTableView(QModelIndex index)
{
    checkIsSelectedItem();
}

MainWindow::~MainWindow()
{
    this->deleteLater();
    foreach (TransFile *tmp, transList)
    {
        delete tmp;
        tmp = NULL;
    }
    transList.clear();

    delete preLook;
    delete ui;


}
