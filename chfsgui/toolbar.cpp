#include "toolbar.h"
#include "precompile.h"
#include <QToolButton>
#include <QMenu>
#include "mainwindow.h"
#include "aboutdlg.h"
#include <QTimer>
#include <QFileDialog>

Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(50);
    setProperty("form","title");

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
        initState();
        emit sigEditorMode();
    });

    connect(_btnPlay,&QToolButton::clicked,this,&Toolbar::sigFireLaunch);
    connect(_btnStop,&QToolButton::clicked,this,[=](){
        g_chfsProcess.kill();
    });

    if( g_settings.value(PARAM_AUTO_LAUNCHE).toBool() ){
        QTimer::singleShot(800,[=](){
            emit sigFireLaunch();
        });
    }

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
    const int BTNSIZE = 42;

    _btnPlay = new QToolButton(this);
    _btnPlay->setFixedSize(BTNSIZE,BTNSIZE);
    _btnPlay->setIcon(QIcon(":/res/image/play.svg"));
    _btnPlay->setIconSize(QSize(32,32));
    _btnPlay->setToolTip(tr("运行"));


    _btnStop = new QToolButton(this);
    _btnStop->setFixedSize(BTNSIZE,BTNSIZE);
    _btnStop->setIcon(QIcon(":/res/image/stop.svg"));
    _btnStop->setIconSize(QSize(32,32));
    _btnStop->setToolTip(tr("停止运行"));

    _btnSave = new QToolButton(this);
    _btnSave->setFixedSize(BTNSIZE,BTNSIZE);
    _btnSave->setIcon(QIcon(":/res/image/save.svg"));
    _btnSave->setIconSize(QSize(32,32));
    _btnSave->setToolTip(tr("将当前配置保存为命令行程序的配置文件"));
    connect(_btnSave,&QToolButton::clicked,[=](){
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存配置"),"","ini(*.ini);;Text(*.txt);;*(*.*)");
        if( fileName.isEmpty() == false ){
            QFile file(fileName);
            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){
                QMessageBox::critical(this,tr("提示"),tr("保存失败！"));
                return;
            }

            QTextStream out(&file);
            out << "port=" << g_settings.value(PARAM_PORT).toString() << endl;
            out << "path=" << g_settings.value(PARAM_PATH).toString() << endl;
            out << "allow=" << g_settings.value(PARAM_ALLOW).toString() << endl;
            auto rules = g_settings.value(PARAM_RULE).toString().split("|");
            for(auto r: rules){
                if( r.isEmpty() == false )
                    out << "rule=" << r << endl;
            }
        }
    });

    _btnMenu = new QToolButton(this);
    _btnMenu->setFixedSize(BTNSIZE,BTNSIZE);
    _btnMenu->setIcon(QIcon(":/res/image/menu.svg"));
    _btnMenu->setIconSize(QSize(32,32));
    _btnMenu->setToolTip(tr("菜单"));

    _btnAuto = new QCheckBox(tr("随软件启动运行服务"), this);
    _btnAuto->setChecked( g_settings.value(PARAM_AUTO_LAUNCHE).toBool() );
    connect(_btnAuto,&QCheckBox::clicked,this,[=](){
        g_settings.setValue(PARAM_AUTO_LAUNCHE,_btnAuto->isChecked());
    });


    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5,1,5,1);
    mainLayout->addWidget(_btnPlay);
    mainLayout->addWidget(_btnStop);
    mainLayout->addStretch();
    mainLayout->addWidget(_btnAuto);
    mainLayout->addWidget(_btnSave);
    mainLayout->addWidget(_btnMenu);
}


void Toolbar::initState()
{
    _btnPlay->show();
    _btnStop->hide();
}

void Toolbar::runningState()
{
    _btnPlay->hide();
    _btnStop->show();
}

void Toolbar::stopState()
{
    _btnPlay->show();
    _btnStop->hide();
}
