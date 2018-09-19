#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "processdecorator.h"
#include "qtsingleapplication/qtsingleapplication.h"
#include <QStandardPaths>
#include <QDir>

QString logPathRoot;

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    if (a.sendMessage("Wake up!"))
        return 0;

    ProcessDecorator pd;
    QString plantResult = pd.makeCHFS();
    if(plantResult.isEmpty()==false){
        QMessageBox::critical(nullptr,QObject::tr("错误"),plantResult);
        return -1;
    }


    //日志目录

    logPathRoot = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
    QDir().mkpath(logPathRoot);


    // set stylesheet
    QFile file(":/qss/flatblack.css");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    a.setStyleSheet(stream.readAll());

    MainWindow::instance().setVisible( !(argc>=2 && QString(argv[1]).contains("auto")) );
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &MainWindow::instance(), SLOT(onShowWindow()));

    return a.exec();
}
