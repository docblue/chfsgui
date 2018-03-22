#include "processdecorator.h"
#include <QFile>
#include <QDir>
#include <QDebug>

QString g_chfsfile;

ProcessDecorator::ProcessDecorator(QObject *parent) : QObject(parent)
{

}

ProcessDecorator::~ProcessDecorator()
{
    clearCHFS();
}

QString ProcessDecorator::makeCHFS()const
{
    QString res;

    QString chfs = QDir::cleanPath(
                        QDir::tempPath()+
                        QDir::separator()+
                        "chfs.exe"        //QUuid::createUuid().toString()
                    );

    QFile::remove(chfs);
    QFile::copy(":/chfs_resource/chfs.exe",chfs);
    QFile::setPermissions(chfs,QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
                          QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser |
                          QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther);
    g_chfsfile = chfs;

    return res;
}



void ProcessDecorator::clearCHFS()const
{
    QFile::remove(g_chfsfile);
}

