#include "toolbar.h"
#include "precompile.h"
#include <QToolButton>
#include <QMenu>
#include "mainwindow.h"
#include <QSettings>
#include "aboutdlg.h"

Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(48);

    createContents();
    initState();


    //
    // connections
    //

    connect(&g_chfsProcess,&QProcess::started,this,[=](){
        runningState();
        emit sigRunningMode();
    });

    void (QProcess::* signalFinished)(int,QProcess::ExitStatus) = &QProcess::finished;
    connect(&g_chfsProcess,signalFinished,this,[=](){
        if( g_chfsProcess.exitStatus() == QProcess::CrashExit){ //手动KILL进程
            initState();
            emit sigEditorMode();
        }else{
            stopState();
        }
    });

    connect(_btnPlay,&QToolButton::clicked,this,&Toolbar::sigFireLaunch);
    connect(_btnStop,&QToolButton::clicked,this,[=](){
        g_chfsProcess.kill();
    });

    connect(_btnReturn,&QToolButton::clicked,this,[=](){
        initState();
        emit sigEditorMode();
    });


    //
    //  actions
    //

    createActions();
}

void Toolbar::createActions()
{
    QMenu* menu = new QMenu;

    extern SomeActions someActions;
    someActions.autoRunAction = menu->addAction(tr("开机启动"),this,SIGNAL(sigAutorunSwitching(bool)));
    someActions.autoRunAction->setCheckable(true);
    someActions.autoRunAction->setChecked( !QSettings(APPDOMAIN, QString(APPKEY)+"/run").value(AUTORUNFORBIDDEN).toBool() );

    menu->addSeparator();
    QAction* aboutAct = menu->addAction(tr("关于"));
    connect(aboutAct,&QAction::triggered,this,[=](){
        AboutDlg dlg(this);
        dlg.exec();
    });
    menu->addSeparator();
    menu->addAction(tr("退出"),this,SIGNAL(sigQuit()));

    _btnMenu->setMenu(menu);
    _btnMenu->setPopupMode(QToolButton::InstantPopup);
}

void Toolbar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Toolbar::createContents()
{
    _btnReturn = new QToolButton(this);
    _btnReturn->setFixedSize(48,48);
    _btnReturn->setIcon(QIcon(":/res/image/return.svg"));
    _btnReturn->setIconSize(QSize(32,32));
    _btnReturn->setToolTip(tr("返回"));

    _btnPlay = new QToolButton(this);
    _btnPlay->setFixedSize(48,48);
    _btnPlay->setIcon(QIcon(":/res/image/play.svg"));
    _btnPlay->setIconSize(QSize(32,32));
    _btnPlay->setToolTip(tr("运行"));


    _btnStop = new QToolButton(this);
    _btnStop->setFixedSize(48,48);
    _btnStop->setIcon(QIcon(":/res/image/stop.svg"));
    _btnStop->setIconSize(QSize(32,32));
    _btnStop->setToolTip(tr("停止运行"));

    _btnMenu = new QToolButton(this);
    _btnMenu->setFixedSize(48,48);
    _btnMenu->setIcon(QIcon(":/res/image/menu.svg"));
    _btnMenu->setIconSize(QSize(32,32));
    _btnMenu->setToolTip(tr("菜单"));


    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(_btnReturn);
    mainLayout->addWidget(_btnPlay);
    mainLayout->addWidget(_btnStop);
    mainLayout->addStretch();
    mainLayout->addWidget(_btnMenu);
}


void Toolbar::initState()
{
    _btnReturn->hide();
    _btnPlay->show();
    _btnStop->hide();
}

void Toolbar::runningState()
{
    _btnReturn->hide();
    _btnPlay->hide();
    _btnStop->show();
}

void Toolbar::stopState()
{
    _btnReturn->show();
    _btnPlay->show();
    _btnStop->hide();
}
