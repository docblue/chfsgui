#include "cfgportwgt.h"
#include "precompile.h"


CfgPortWgt::CfgPortWgt(QWidget *parent) : CfgBase(parent)
{
    _editPort = new QLineEdit(this);
    _editPort->setFixedWidth(48);
    _editPort->setInputMask("99999");
    QString port = g_settings.value(PARAM_PORT).toString();
    if( port.isEmpty() ){
        port = "80";
        g_settings.setValue(PARAM_PORT, port);
    }
    _editPort->setText( port );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget( _editPort );


    connect(_editPort,&QLineEdit::textEdited,this,[=](){
        g_settings.setValue(PARAM_PORT, _editPort->text());
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

