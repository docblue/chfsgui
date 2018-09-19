#include "cfgsharedpathwgt.h"
#include "precompile.h"
#include <QFileDialog>
#include <QDesktopServices>


CfgSharedpathWgt::CfgSharedpathWgt(QWidget *parent) : CfgBase(parent)
{
    _editPath = new QLineEdit(this);
    QString currentPath = g_settings.value(PARAM_PATH).toString();
    if( currentPath.isEmpty() ){
        currentPath = QApplication::applicationDirPath();
        g_settings.setValue(PARAM_PATH, currentPath);
    }
    _editPath->setText( currentPath );

    _btnChoose = new QPushButton(tr("浏览"), this);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget( _editPath, 3);
    mainLayout->addWidget( _btnChoose );
    mainLayout->addStretch(1);

    connect(_btnChoose,&QPushButton::clicked,this,&CfgSharedpathWgt::onPathChanging);
    connect(_editPath,&QLineEdit::textEdited,this,[=](QString path){
        g_settings.setValue(PARAM_PATH, path);
    });
}

void CfgSharedpathWgt::onEditorMode()
{
    _editPath->setReadOnly( false );
    _btnChoose->setText(tr("浏览"));
    _btnChoose->setVisible( true );
}

void CfgSharedpathWgt::onRunningMode()
{
    _editPath->setReadOnly(true);
    _editPath->clearFocus();
    _btnChoose->setText(tr("打开"));
    _btnChoose->clearFocus();
    _btnChoose->setVisible(true);
}

void CfgSharedpathWgt::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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
            g_settings.setValue(PARAM_PATH, dir);
        }
    }else{
        QDesktopServices::openUrl(QUrl(_editPath->text()));
    }
}
