#include "monitorwgt.h"
#include "precompile.h"
#include <QTextBrowser>
#include <QRegularExpression>
#include <QDesktopServices>

LogWidget::LogWidget(QWidget *parent) : QWidget(parent)
{
    logList = new QListWidget(this);

    QLabel* logBtn = new QLabel("<a href=#><span style=\"color:silver;\">操作日志</span></a>:",this);
    logBtn->setToolTip("点击打开操作日志目录");
    connect(logBtn,&QLabel::linkActivated,[=](){
        extern QString logPathRoot;
        QDesktopServices::openUrl(QUrl(logPathRoot));
    });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(2);
    mainLayout->addWidget( logBtn );
    mainLayout->addWidget( logList );
}

void LogWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

MonitorWgt::MonitorWgt(QWidget *parent) : QWidget(parent)
{
    MyTextBrowser* editorStatus = new MyTextBrowser(this);
    editorStatus->setOpenExternalLinks(true);
    editorStatus->setOpenLinks(true);
    editorStatus->setPlaceholderText(tr("服务未运行"));
    editorStatus->setFixedHeight(100);

    LogWidget* editorLog = new LogWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(15);
    mainLayout->addWidget( editorStatus );
    mainLayout->addWidget( editorLog,1 );

    //
    // connections
    //

    connect(&g_chfsProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QString msg = g_chfsProcess.readAllStandardOutput();

        QRegExp urlReg(R"(http://[0-9+|.:]+[0-9])");
        for(QString line: msg.split("\n")){
            if( line.isEmpty() == false ){
                //TODO 使用正则进行校验
                if( line.length() > 50 && line[4]=='-'  && line[7]=='-'  && line[10]==' ' ){
                    if( editorLog->logList->count()>1000 ){
                        editorLog->logList->clear();
                    }
                    QListWidgetItem* item = new QListWidgetItem(line);
                    if( line.contains(") removed") ){
                        item->setForeground( Qt::red );
                    }else if( line.contains(") rename") || line.contains(") uploaded") || line.contains(") create")){
                        item->setForeground( Qt::darkYellow );
                    }
                    editorLog->logList->addItem( item );
                    editorLog->logList->scrollToBottom();
                }else{
                    urlReg.indexIn(line);
                    QStringList urls = urlReg.capturedTexts();
                    for(auto url: urls){
                        if( url.isEmpty() == false ){
                             QString newTextItem = QString("<pre>&nbsp;&nbsp;<a href=\"%1\" style=\"color:green\">%2</a></pre>").arg(url).arg(url);
                             editorStatus->append( newTextItem );
                        }
                    }
                }
            }
        }
    });

    connect(&g_chfsProcess,&QProcess::readyReadStandardError,this,[=](){
        editorStatus->append( QString("<font color=\"red\">") + g_chfsProcess.readAllStandardError() + QString("</font>") );
    });

    connect(&g_chfsProcess,&QProcess::started,this,[=](){
        editorStatus->clear();
        editorLog->logList->clear();
    });

    connect(&g_chfsProcess,static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),[=](int , QProcess::ExitStatus exitStatus){
        if( exitStatus == QProcess::CrashExit ){
            editorStatus->clear();
            editorLog->logList->clear();
        }
    });
}

void MonitorWgt::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
