#include "monitorwgt.h"
#include "precompile.h"
#include <QTextBrowser>
#include <QRegularExpression>

MonitorWgt::MonitorWgt(QWidget *parent) : QWidget(parent)
{
    MyTextBrowser* editor = new MyTextBrowser(this);
    editor->setOpenExternalLinks(true);
    editor->setOpenLinks(true);
    editor->setPlaceholderText(tr("服务未运行"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12,0,12,0);
    mainLayout->addWidget( editor,1 );

    //
    // connections
    //

    connect(&g_chfsProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QString msg = g_chfsProcess.readAllStandardOutput();

        const QString title("共享网址:");
        if( editor->toPlainText().contains(title) == false ){
            editor->append(title);
            editor->append("\n");
        }

        QRegExp urlReg(R"(http://[0-9+|.:]+[0-9])");
        for(auto line: msg.split("\n")){
            if( line.isEmpty() == false ){
                urlReg.indexIn(line);
                QStringList urls = urlReg.capturedTexts();
                for(auto url: urls){
                    if( url.isEmpty() == false ){
                         QString newTextItem = QString("<pre>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"%1\" style=\"color:green\">%2</a></pre>").arg(url).arg(url);
                         editor->append( newTextItem );
                         editor->append("\n");
                    }
                }
            }
        }
    });

    connect(&g_chfsProcess,&QProcess::readyReadStandardError,this,[=](){
        editor->append( QString("<font color=\"red\">") + g_chfsProcess.readAllStandardError() + QString("</font>") );
    });

    connect(&g_chfsProcess,&QProcess::started,this,[=](){
        editor->clear();
    });
}

void MonitorWgt::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
