#include "cfgsharedpathwgt.h"
#include "precompile.h"
#include "mysettings.h"
#include <QFileDialog>
#include <QDesktopServices>


CfgSharedpathWgt::CfgSharedpathWgt(QWidget *parent) : CfgBase(parent)
{
    _editPath = new QLineEdit(this);
    _btnChoose = new QPushButton(tr("浏览"), this);
    _labelNote = new QLabel(tr("注：‘默认启动项’的共享目录不可手动更改，仅随程序目录改变而改变"),this);

    QHBoxLayout* editLayout = new QHBoxLayout;
    editLayout->setMargin(0);
    editLayout->addWidget( _editPath, 3);
    editLayout->addWidget( _btnChoose );
    editLayout->addStretch(1);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addLayout(editLayout);
    mainLayout->addWidget( _labelNote );
    mainLayout->addStretch(1);

    connect(&MySettings::instance(),&MySettings::currentItemChanged,this,&CfgSharedpathWgt::onCurrentItemChanged);
    connect(_btnChoose,&QPushButton::clicked,this,&CfgSharedpathWgt::onPathChanging);
    connect(_editPath,&QLineEdit::textEdited,this,[=](QString path){
        MySettings::instance().setPathValue(_currentKey, path);
    });
}

void CfgSharedpathWgt::onEditorMode()
{
    _editPath->setReadOnly(_currentKey != DEFAULTITEM);
    _btnChoose->setText(tr("浏览"));
    _btnChoose->setVisible(_currentKey != DEFAULTITEM);
    _labelNote->setVisible(true);
}

void CfgSharedpathWgt::onRunningMode()
{
    _editPath->setReadOnly(true);
    _editPath->clearFocus();
    _btnChoose->setText(tr("打开"));
    _btnChoose->clearFocus();
    _btnChoose->setVisible(true);
    _labelNote->setVisible(false);
}

void CfgSharedpathWgt::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CfgSharedpathWgt::onCurrentItemChanged(QString key)
{
    _currentKey = key;

    _editPath->setEnabled(key != DEFAULTITEM);
    _btnChoose->setVisible(key != DEFAULTITEM);
    _labelNote->setVisible(key == DEFAULTITEM);

    if(key == DEFAULTITEM){
        _editPath->setText( QApplication::applicationDirPath() );
    }else{
        _editPath->setText(MySettings::instance().getPathValue(key));
    }
}

void CfgSharedpathWgt::onPathChanging()
{
    const QString flag = _btnChoose->text();
    if( flag == tr("浏览") ){
        QString dir = QFileDialog::getExistingDirectory(this, tr("选择共享目录"),
                                                        "",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
        if(dir.isEmpty()==false){
            _editPath->setText(dir);
            MySettings::instance().setPathValue(_currentKey, dir);
        }
    }else{
        QDesktopServices::openUrl(QUrl(_editPath->text()));
    }
}
