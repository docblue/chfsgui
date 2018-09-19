#include "cfgallowip.h"
#include "precompile.h"
#include <QRadioButton>
#include <QListWidget>
#include <QRegExpValidator>
#include "utilities.h"


AllowipDlg::AllowipDlg(QString mode, QString oldValue, QWidget *parent):
    QDialog(parent),_mode(mode)
{
    //
    // create contents
    //

    QRadioButton* btnSingle = new QRadioButton(tr("单个"),this);
    QRadioButton* btnGroup = new QRadioButton(tr("范围"),this);
    QHBoxLayout* btnsLayout = new QHBoxLayout;
    btnsLayout->setMargin(0);
    btnsLayout->addWidget( btnSingle );
    btnsLayout->addWidget( btnGroup );
    btnsLayout->addStretch( 1 );

    QString oIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp oIpRegex ("^" + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange + "$");

    QLineEdit* editLeft = new QLineEdit(this);
    editLeft->setValidator(new QRegExpValidator(oIpRegex));
    QLineEdit* editRight = new QLineEdit(this);
    editRight->setValidator(new QRegExpValidator(oIpRegex));
    QLabel* sep = new QLabel(tr("至"),this);
    QHBoxLayout* ctrlLayout = new QHBoxLayout;
    ctrlLayout->setMargin(0);
    ctrlLayout->setSpacing(6);
    ctrlLayout->addWidget( editLeft, 1 );
    ctrlLayout->addWidget( sep );
    ctrlLayout->addWidget( editRight, 1 );

    QPushButton* btnOK = new QPushButton(tr("确定"),this);
    QPushButton* btnCancel = new QPushButton(tr("取消"),this);
    QHBoxLayout* commondLayout = new QHBoxLayout;
    commondLayout->setMargin(0);
    commondLayout->addStretch(1);
    commondLayout->addWidget( btnOK );
    commondLayout->addWidget( btnCancel );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10,10,10,15);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(26);
    mainLayout->addLayout( btnsLayout );
    mainLayout->addSpacing(6);
    mainLayout->addLayout( ctrlLayout );
    mainLayout->addStretch(3);
    mainLayout->addLayout( commondLayout );

    //
    // connections
    //

    connect(btnSingle,&QRadioButton::toggled,this,[=](bool checked){
        sep->setVisible(!checked);
        editRight->setVisible(!checked);
    });
    connect(btnOK,&QPushButton::clicked,this,[=](){
        QString startIP = editLeft->text();
        QString endIP = editRight->text();
        if( startIP.isEmpty() ){
            QMessageBox::critical(nullptr,tr("提示"),tr("请输入地址！"));
            return;
        }
        if( btnSingle->isChecked() == false && endIP.isEmpty() ){
            QMessageBox::critical(nullptr,tr("提示"),tr("请输入地址！"));
            return;
        }
        if( btnSingle->isChecked() == false ){
            int res = compareIPv4(startIP,endIP);
            if( res > 0 ){
                QMessageBox::critical(nullptr,tr("提示"),tr("结束地址应大于开始地址！"));
                return;
            }else if(res == 0){
                ips = startIP;
            }else{
                ips = startIP + "-" + endIP;
            }
        }else{
            ips = startIP;
        }

        this->accept();
    });
    connect(btnCancel,&QPushButton::clicked,this,[=](){
        this->reject();
    });



    //
    // 初始化
    //

    if(_mode==NEWALLOWIP){
        setWindowTitle( tr("添加") );
        btnSingle->setChecked(true);
    }else{
        setWindowTitle( tr("修改") );
        btnSingle->hide();
        btnGroup->hide();

        QStringList pairs = oldValue.split("-");
        if( pairs.length() == 1 ){
            sep->hide();
            editRight->hide();
        }else{
            editRight->setText(pairs[1]);
            sep->show();
            editRight->show();
        }
        editLeft->setText(pairs[0]);
        editLeft->selectAll();
    }

    setFixedSize(260,150);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

CfgAllowip::CfgAllowip(QWidget *parent) : CfgBase(parent)
{
    //
    // create contents
    //

    _btnBlack = new QRadioButton(tr("黑名单"),this);
    _btnWhite = new QRadioButton(tr("白名单"),this);
    QHBoxLayout* btnsLayout = new QHBoxLayout;
    btnsLayout->setMargin(0);
    btnsLayout->addWidget( _btnBlack );
    btnsLayout->addSpacing(26);
    btnsLayout->addWidget( _btnWhite );
    btnsLayout->addStretch( 1 );

    _btnNew = new QPushButton(tr("添加"),this);
    _btnRemove = new QPushButton(tr("删除"),this);
    _btnModify = new QPushButton(tr("修改"),this);
    _btnReset = new QPushButton(tr("重置"),this);
    QVBoxLayout* ctrlLayout = new QVBoxLayout;
    ctrlLayout->setMargin(0);
    ctrlLayout->addWidget( _btnNew );
    ctrlLayout->addWidget( _btnRemove );
    ctrlLayout->addWidget( _btnModify );
    ctrlLayout->addStretch(1);
    ctrlLayout->addWidget( _btnReset );

    _listWgt = new QListWidget(this);
    QHBoxLayout* listLayout = new QHBoxLayout;
    listLayout->setMargin(0);
    listLayout->addWidget( _listWgt, 1 );
    listLayout->addLayout( ctrlLayout );
    listLayout->addStretch(1);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addLayout( btnsLayout );
    mainLayout->addLayout( listLayout, 1 );

    initIPs();

    //
    // connections
    //


    connect(_btnBlack,&QRadioButton::clicked,this,[=](){
        QString ips = QString("not(%1)").arg(collectAllItems());
        g_settings.setValue(PARAM_ALLOW,ips);
    });

    connect(_btnWhite,&QRadioButton::clicked,this,[=](){
        g_settings.setValue(PARAM_ALLOW,collectAllItems());
    });

    connect(_btnNew,&QPushButton::clicked,this,[=](){
        AllowipDlg dlg(NEWALLOWIP,"",this);
        if( QDialog::Accepted == dlg.exec() ){
            _listWgt->addItem(dlg.ips);
            g_settings.setValue(PARAM_ALLOW, _btnBlack->isChecked()?QString("not(%1)").arg(collectAllItems()):collectAllItems());
        }
    });
    connect(_btnRemove,&QPushButton::clicked,this,[=](){
        QListWidgetItem* currentItem = _listWgt->currentItem();
        if( currentItem ){
            QMessageBox::StandardButton chosen = QMessageBox::question(this,tr("警告"),tr("确定删除所选地址？"));
            if( chosen == QMessageBox::Yes ){
                _listWgt->removeItemWidget(currentItem);
                delete currentItem;
                g_settings.setValue(PARAM_ALLOW, _btnBlack->isChecked()?QString("not(%1)").arg(collectAllItems()):collectAllItems());
            }
        }
    });
    connect(_btnModify,&QPushButton::clicked,this,[=](){
        QListWidgetItem* currentItem = _listWgt->currentItem();
        if( currentItem ){
            AllowipDlg dlg(MODALLOWIP,currentItem->text(),this);
            if( QDialog::Accepted == dlg.exec() ){
                _listWgt->addItem(dlg.ips);
                _listWgt->removeItemWidget(currentItem);
                delete currentItem;
                g_settings.setValue(PARAM_ALLOW, _btnBlack->isChecked()?QString("not(%1)").arg(collectAllItems()):collectAllItems());
            }
        }
    });
    connect(_btnReset,&QPushButton::clicked,this,[=](){
        QMessageBox::StandardButton chosen = QMessageBox::question(this,tr("警告"),tr("确定重置？"));
        if( chosen == QMessageBox::Yes ){
            _listWgt->clear();
            _btnBlack->setChecked(true);
            g_settings.setValue(PARAM_ALLOW,"");
        }
    });
}

void CfgAllowip::initIPs()
{
    _listWgt->clear();

    QString allow = g_settings.value(PARAM_ALLOW).toString();
    if( allow.isEmpty() ){
        _btnBlack->setChecked( true );
    }else{
        QString ips(allow);
        if(allow.startsWith("not",Qt::CaseInsensitive)){
             _btnBlack->setChecked( true );
             ips = allow.mid(4, allow.length()-5);
        }else{
             _btnWhite->setChecked( true );
        }

        _listWgt->addItems(ips.split(","));
    }
}

QString CfgAllowip::collectAllItems()
{
    QString ips;
    for(int i=0;i<_listWgt->count();i++){
        ips += _listWgt->item(i)->text();
        ips += ",";
    }
    if( ips.endsWith(",") ){
        ips.remove(ips.length()-1,1);
    }

    return ips;
}

void CfgAllowip::onEditorMode()
{
    onModeChanged(true);
}

void CfgAllowip::onRunningMode()
{
    onModeChanged(false);
}

void CfgAllowip::onModeChanged(bool isEditor)
{
    _btnBlack->setEnabled(isEditor);
    _btnWhite->setEnabled(isEditor);

    _btnNew->setVisible(isEditor);
    _btnRemove->setVisible(isEditor);
    _btnModify->setVisible(isEditor);
    _btnReset->setVisible(isEditor);
}
