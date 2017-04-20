#include "transcurrcode.h"
#include "rgtranscode.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <coreplugin/documentmanager.h>

TransCurrCode::TransCurrCode()
{
}

void TransCurrCode::setTargetCode(QString code)
{
    targetCode = code;
}

void TransCurrCode::transCurrFileCode()
{
    Core::DocumentManager* docu =  Core::DocumentManager::instance();

    QString fileName = docu->currentFile();
    if(fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(0, QObject::tr("��ʾ"),
                             QObject::tr("δ���������ļ�"));
        return;
    }

    //����ַ������ʽ
    RGtransCode rgTC;
    QString sourceCode = rgTC.checkFileCode(fileName);
    QFileInfo info(fileName);
    rgTC.transFileCode(fileName,sourceCode,targetCode,info.absolutePath(),RGtransCode::TYPE_REPLACEOLDFILE);
}
