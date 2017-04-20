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

//���캯���г�ʼ���ۺ�������
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
    //ȫѡ
    connect(ui->selectAll,SIGNAL( clicked(bool) ),
            this,SLOT( selectAllItems(bool) ));
    //��ѡ
    connect(ui->selectAllOther,SIGNAL( clicked(bool) ),
            this,SLOT( selectAllOtherItems(bool) ));

}

//��ʼ������Ŀؼ���ʾ��Ϣ
void MainWindow::initToolTips()
{
    ui->addNewFile->setToolTip(tr("���ļ�"));
    ui->addDir->setToolTip(tr("���ļ���"));
    ui->deleteFileItem->setToolTip(tr("ɾ���ļ�"));
    ui->setLocalLook->setToolTip(tr("�鿴�ļ�����"));
    ui->startTransCode->setToolTip(tr("��ʼת��ѡ�е��ļ�"));
}

//��ʼ�����ݳ�Ա
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

//��ʼ��model
void MainWindow::initModel()
{
    model = new MyTableModel();
}

//��ʼ��View
void MainWindow::initView()
{
    ui->fileTableView->setModel(model);
    ui->fileTableView->setItemDelegateForColumn(3,new MyPushDelegate(ui->fileTableView));
    ui->fileTableView->setItemDelegateForColumn(4,new MyPushDelegate(ui->fileTableView));

    ui->fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fileTableView->horizontalHeader()->setStretchLastSection(true);
}

//����ļ���ť�ۺ���
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
    setUiStatus(QObject::tr("ѡ���ļ�"));
    QStringList fileNames = QFileDialog::getOpenFileNames(this,tr("ѡ���ļ�"),
                                                    LastFilePath,
                                                    formatContent);

    if(fileNames.isEmpty())
    {
        setUiStatus(QObject::tr("δѡ���ļ�"));
    }
    else
    {
        setUiStatus(QString(QObject::tr("ѡ����%1���ļ�")).arg(fileNames.count()));
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
                                     QObject::tr("δ���������ļ�: %1:\n%2")
                                     .arg(fileName)
                                     .arg(file.errorString()));
                return;
            }
            preLook->show();
            QByteArray data = file.readAll();
            preLook->setWindowTitle(fileName);
            preLook->setFieData(data);

            //����ַ������ʽ
            RGtransCode rgTC;
            QString code = rgTC.checkFileCode(fileName);
            preLook->setReadCode(code);

            if (preLook->exec())
            {
                QFileInfo info(file);
                if(checkSameFileOpened(info))
                {
                    QMessageBox::warning(this,QObject::tr("��ʾ"),
                                         QObject::tr("�ļ�%1�Ѿ����:").arg(file.fileName()));
                    return;
                }

                insertNums++;
                setUiStatus(QObject::tr("����ļ����б�%1").arg(info.absoluteFilePath()));
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
    setUiStatus(QObject::tr("�����%1���ļ����б�").arg(insertNums));
}

//���һ���ļ���
void MainWindow::addNewDir(bool)
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                 tr("ѡ���ļ���"),
                                                 LastDirPath);
    if(path == NULL)
    {
        return;
    }
    LastDirPath = path;
    setUiStatus(QObject::tr("ѡ���ļ���:%1").arg(LastDirPath));
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
                                 QObject::tr("δ���������ļ� %1:\n%2")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        QByteArray data = file.readAll();
        preLook->setWindowTitle(fileName);
        preLook->setFieData(data);

        //����ַ������ʽ
        RGtransCode rgTC;
        QString code = rgTC.checkFileCode(fileName);
        preLook->setReadCode(code);

        {
            QFileInfo info(file);
            if(checkSameFileOpened(info))
            {
                QMessageBox::warning(this,QObject::tr("��ʾ"),QObject::tr("�ļ��Ѿ���ӣ�"));
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

//����Ƿ��Ѿ���ӹ���ͬ���ļ�
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

//����Ƿ��Ѿ���ӹ���ͬ���ļ�
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

//ɾ���ļ���ť�ۺ���
void MainWindow::deleteFile(bool)
{
   int num = model->removeMyDatas();
   setUiStatus(QObject::tr("ɾ��%1����Ŀ").arg(num));

   checkIsSelectedItem();
   if(model->getFileList().count()<=0)
   {
       ui->selectAll->setCheckState(Qt::Unchecked);
       ui->selectAll->setEnabled(false);
       ui->selectAllOther->setCheckState(Qt::Unchecked);
       ui->selectAllOther->setEnabled(false);
   }


}

//�����ļ����·����ť�ۺ���
void MainWindow::setTargetFilePath(bool)
{
    if(targetFilePath != NULL)
    {
        setUiStatus(QObject::tr("����Ĭ�ϴ洢·��"));
        QString path =  QFileDialog::getExistingDirectory(this,
                                                          tr("�����ļ����·��"),
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

//����Ĭ�ϵ�����ļ�·��
void MainWindow::setDefaultTargetPath()
{
    targetFilePath = QDir::currentPath();
    ui->targetFilePath->setText(targetFilePath);
    setUiStatus(QObject::tr("����Ĭ�ϴ洢·��:%1").arg(targetFilePath));
}

//����״̬��ʾ����ʵʱ��ʾ����
void MainWindow::setUiStatus(QString contents)
{
//    ui->statusBar->showMessage(contents);
    label->setText(contents);
}

//ˢ�±������
void MainWindow::updateTable()
{
}

//��ʼ��ʽת����ť�ۺ���
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
                if(QMessageBox::Ok == QMessageBox::warning(this,tr("��ʾ"),
                                                           tr("�ļ��Ѿ����ڣ��Ƿ񸲸�ԭ�ļ���"),
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
                //�Ǹ�����ת��������Ŀ¼�´������ļ�
                rgTC.transFileCode(tFile->fileAbPath,
                                   tFile->sourceCode,
                                   tFile->targetCode,
                                   targetFilePath,
                                   RGtransCode::TYPE_CREATENEWFILE);
            }
            else if(result  == 1)
            {
                //������ת�����滻Դ�ļ�
                rgTC.transFileCode(tFile->fileAbPath,
                                   tFile->sourceCode,
                                   tFile->targetCode,
                                   targetFilePath,
                                   RGtransCode::TYPE_REPLACEOLDFILE);
            }
            else if(result == 2)
            {
                //�Ǹ�����ת������Դ�ļ�ͬ��Ŀ¼�´������ļ�
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
                             QObject::tr("��ʾ"),
                             QObject::tr("û��ѡ����Ҫת�����ļ�"));
    }
    else
    {
        QMessageBox::information(this,
                                 QObject::tr("��ʾ"),
                                 QObject::tr("�ɹ�ת��%1���ļ�,�ļ����·����\n%2")
                                 .arg(index)
                                 .arg(targetFilePath));
    }
    clearTransSuccList();
    setUiStatus(QString(QObject::tr("�ɹ�ת��%1���ļ�")).arg(index));
}

//���ñ���Ĭ�ϵ�������ļ�
void MainWindow::setLocalLook(bool)
{
    int row = ui->fileTableView->currentIndex().row();
    if(row<0)
    {
        setUiStatus(QObject::tr("δѡ���Ӧ��Ԥ���ļ�"));
        return;
    }
    QString fileAbPath = model->getFileList().at(row)->fileAbPath;
    //���ñ���Ĭ���ļ���������ļ�
    #ifdef Q_OS_MAC
        QProcess *icemProcess=new QProcess;
        QStringList arguments;
        QString bash= QObject::tr("open");
        arguments<<QObject::tr("-e"<<fileAbPath;
        bool r = icemProcess->startDetached(bash,arguments);
        if(r == false)
        {
            QMessageBox::warning(this,tr("��ʾ"),tr("δ����ȷ���ļ�������ϵ������Ա"));
            setUiStatus(QObject::tr("δ����������ļ�"));
        }
        else
        {
            setUiStatus(QObject::tr("����ļ�����:%1").arg(fileAbPath));
        }
    #else
        QDesktopServices look;
        bool r = look.openUrl(QUrl(fileAbPath));
        if(r == false)
        {
            QMessageBox::warning(this,tr("��ʾ"),tr("δ����ȷ���ļ�������ϵ������Ա"));
            setUiStatus(QObject::tr("δ����������ļ�"));
        }
        else
        {
            setUiStatus(QObject::tr("����ļ�����:%1").arg(fileAbPath));
        }
    #endif

}

//�����Ҫת�����ļ��ǲ�����Ŀ��·��������ͬ���ļ�
bool MainWindow::checkFileExited(TransFile *tFile)
{
    //�����鵽����ͬ�ļ����ڣ�����true�����򷵻�false

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

//���ȫѡ
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
    setUiStatus(QObject::tr("ȫѡ"));
}

//�����ѡ
void MainWindow::selectAllOtherItems(bool bl)
{
    Q_UNUSED(bl)
    int state = ui->selectAll->checkState();
    model->shiftOther(state);
    if(model->getSelectedAllStatus())
    {
        //Ϊȫѡ״̬
        ui->selectAll->setCheckState(Qt::Checked);
    }
    else
    {
        //��ȫѡ״̬
        ui->selectAll->setCheckState(Qt::Unchecked);
    }

    checkIsSelectedItem();
}

//����Ѿ�ת���ɹ����б�
void MainWindow::clearTransSuccList()
{
}

//�ı��ȡ�ļ���ʽ������
void MainWindow::changeSourceCode(int)
{

}

//�ı�Ŀ���ļ���ʽ������
void MainWindow::changeTargetCode(int)
{

}

//��ʾ����Ӧ��ʵʱ��ʾ��Ϣ
void MainWindow::showStatus(QModelIndex index)
{
    ui->statusBar->showMessage(model->showStatus(index));
}

//����Ƿ���ѡ�е���Ŀ
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

//һ������ѡ�е���Ŀ��Ŀ���ʽ
void MainWindow::setAllSelectedItemCode(bool)
{
    model->setSelectedItemCode(ui->sameCode->currentText());
}

//������������Ŀ�����¼�
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
