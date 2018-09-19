#include "configwgt.h"
#include "precompile.h"
#include "uicomponents/configwgt/cfgsharedpathwgt.h"
#include "uicomponents/configwgt/cfgallowip.h"
#include "uicomponents/configwgt/cfgrules.h"
#include "uicomponents/configwgt/cfgportwgt.h"


ConfigWgt::ConfigWgt(QWidget *parent) : QWidget(parent)
{
    createComponents();
    setProperty("form","true");
}

void ConfigWgt::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ConfigWgt::onEditorMode()
{
    CfgBase* ws[]{_sharedpathWgt,_allowWgt,_rulesWgt,_portWgt};
    for(auto i : ws){
        i->onEditorMode();
    }
}

void ConfigWgt::onRunningMode()
{
    CfgBase* ws[]{_sharedpathWgt,_allowWgt,_rulesWgt,_portWgt};
    for(auto i : ws){
        i->onRunningMode();
    }
}


void ConfigWgt::createComponents()
{
    const int ITEM_LEFT_OFFSET = 26;

    _sharedpathWgt = new CfgSharedpathWgt(this);
    _allowWgt = new CfgAllowip(this);
    _rulesWgt = new CfgRules(this);
    _portWgt = new CfgPortWgt(this);

    // 共享目录

    QHBoxLayout* pathLabelLayout = new QHBoxLayout;
    pathLabelLayout->addWidget(new QLabel(tr("共享目录"),this));
    pathLabelLayout->addStretch(1);

    QHBoxLayout* pathLayout = new QHBoxLayout;
    pathLayout->setContentsMargins(ITEM_LEFT_OFFSET,0,0,0);
    pathLayout->addWidget(_sharedpathWgt,1);

    //监听端口

    QHBoxLayout* portLabelLayout = new QHBoxLayout;
    portLabelLayout->addWidget(new QLabel(tr("监听端口"),this));
    portLabelLayout->addStretch(1);

    QHBoxLayout* portLayout = new QHBoxLayout;
    portLayout->setContentsMargins(ITEM_LEFT_OFFSET,0,0,0);    
    portLayout->addWidget( _portWgt );
    portLayout->addStretch(1);


    // 账户控制

    QHBoxLayout* rulesLabelLayout = new QHBoxLayout;
    rulesLabelLayout->addWidget(new QLabel(tr("账户控制"),this));
    rulesLabelLayout->addStretch(1);

    QHBoxLayout* rulesLayout = new QHBoxLayout;
    rulesLayout->setContentsMargins(ITEM_LEFT_OFFSET,0,0,0);
    rulesLayout->addWidget(_rulesWgt,1);


    // 地址过滤

    QHBoxLayout* allowLabelLayout = new QHBoxLayout;
    allowLabelLayout->addWidget(new QLabel(tr("地址过滤"),this));
    allowLabelLayout->addStretch(1);

    QHBoxLayout* allowLayout = new QHBoxLayout;
    allowLayout->setContentsMargins(ITEM_LEFT_OFFSET,0,0,0);
    allowLayout->addWidget(_allowWgt,1);


    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5,0,5,0);
    mainLayout->setSpacing(6);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(pathLabelLayout);
    mainLayout->addLayout(pathLayout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(portLabelLayout);
    mainLayout->addLayout(portLayout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(rulesLabelLayout);
    mainLayout->addLayout(rulesLayout, 2);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(allowLabelLayout);
    mainLayout->addLayout(allowLayout, 1);
    mainLayout->addSpacing(6);
}

