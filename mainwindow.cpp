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
#include <QTextCodec>

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
    ui->addNewFile->setToolTip(QObject::tr("Open File"));
    ui->addDir->setToolTip(QObject::tr("Open Folder"));
    ui->deleteFileItem->setToolTip(QObject::tr("Delete File"));
    ui->setLocalLook->setToolTip(QObject::tr("View File"));
    ui->startTransCode->setToolTip(QObject::tr("Start Trans"));
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
    setUiStatus(QObject::tr("Select File"));
    QStringList fileNames = QFileDialog::getOpenFileNames(this,QObject::tr("Select File"),
                                                    LastFilePath,
                                                    formatContent);

    if(fileNames.isEmpty())
    {
        setUiStatus(QObject::tr("No File Select"));
    }
    else
    {
        setUiStatus(QString(QObject::tr("Selected %1 File")).arg(fileNames.count()));
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
                                     QObject::tr("Can't Open File: %1:\n%2")
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
                    QMessageBox::warning(this,QObject::tr("Information"),
                                         QObject::tr("Add A New File:%1:").arg(file.fileName()));
                    return;
                }

                insertNums++;
                setUiStatus(QObject::tr("Add A New File to Trans List:%1").arg(info.absoluteFilePath()));
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
    setUiStatus(tr("Add %1 Files To Trans List").arg(insertNums));
}

//添加一个文件夹
void MainWindow::addNewDir(bool)
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                 tr("Select Folder"),
                                                 LastDirPath,QFileDialog::ReadOnly);
    if(path == NULL)
    {
        return;
    }
    LastDirPath = path;
    targetFilePath = path;
    ui->targetFilePath->setText(targetFilePath);
    setUiStatus(tr("Select Folder:%1").arg(LastDirPath));
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
            QMessageBox::warning(this, QObject::tr("Information"),
                                 QObject::tr("Can't Open File: %1:\n%2")
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
                QMessageBox::warning(this,QObject::tr("Information"),QObject::tr("File has added"));
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
   setUiStatus(QObject::tr("Delete %1 Items").arg(num));

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
        setUiStatus(QObject::tr("Set Defult Target File Path"));
        QString path =  QFileDialog::getExistingDirectory(this,
                                                          QObject::tr("Set Defult Path"),
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
    setUiStatus(QObject::tr("Set Defult Path:%1").arg(targetFilePath));
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
                if(QMessageBox::Ok == QMessageBox::warning(this,QObject::tr("Information"),
                                                           QObject::tr("File Is Exited,Do You Want To OverRide This File?"),
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
        QMessageBox::warning(this,QObject::tr("Information"),QObject::tr("Not Select Trans File"));
    }
    else
    {
        QMessageBox::information(this,QObject::tr("Information"),QObject::tr("Success Trans %1 File,File Path:\n%2").arg(index).arg(targetFilePath));
    }
    clearTransSuccList();
    setUiStatus(QString(QObject::tr("Success Trans %1 File")).arg(index));
}

//调用本地默认的软件打开文件
void MainWindow::setLocalLook(bool)
{
    int row = ui->fileTableView->currentIndex().row();
    if(row<0)
    {
        setUiStatus(QObject::tr("No File Selected To View"));
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
            QMessageBox::warning(this,QObject::tr("Information"),QObject::tr("Can't Open File,Please call Author"));
            setUiStatus(QObject::tr("Can't View File Success"));
        }
        else
        {
            setUiStatus(QObject::tr("View File:%1").arg(fileAbPath));
        }
    #else
        QDesktopServices look;
        bool r = look.openUrl(QUrl(fileAbPath));
        if(r == false)
        {
            QMessageBox::warning(this,tr("Information"),tr("Can't Open File,Please call Author"));
            setUiStatus(QObject::tr("Can't View File Success"));
        }
        else
        {
            setUiStatus(QObject::tr("View File:%1").arg(fileAbPath));
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
    setUiStatus(QObject::tr("Selected All"));
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
