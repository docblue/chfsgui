#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "processdecorator.h"
#include "qtsingleapplication/qtsingleapplication.h"

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


    // set stylesheet
    QFile file(":/dark.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    a.setStyleSheet(stream.readAll());

    MainWindow::instance().setVisible( !(argc>=2 && QString(argv[1]).contains("auto")) );
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &MainWindow::instance(), SLOT(onShowWindow()));

    return a.exec();
}
