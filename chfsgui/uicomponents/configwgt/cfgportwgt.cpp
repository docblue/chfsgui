#include "cfgportwgt.h"
#include "precompile.h"
#include "mysettings.h"


CfgPortWgt::CfgPortWgt(QWidget *parent) : CfgBase(parent)
{
    _editPort = new QLineEdit(this);
    _editPort->setFixedWidth(48);
    _editPort->setInputMask("00000");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget( _editPort );

    connect(&MySettings::instance(),&MySettings::currentItemChanged,this,[=](QString key){
        _currentKey = key;
        _editPort->setText( MySettings::instance().getPortValue(key));
    });
    connect(_editPort,&QLineEdit::editingFinished,this,[=](){
        MySettings::instance().setPortValue(_currentKey, _editPort->text());
    });
}

void CfgPortWgt::onEditorMode()
{
    _editPort->setEnabled(true);
}

void CfgPortWgt::onRunningMode()
{
    _editPort->setEnabled(false);
}

