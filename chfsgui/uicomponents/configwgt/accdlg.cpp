#include "accdlg.h"
#include "precompile.h"
#include <QTableWidget>
#include "mysettings.h"
#include <QMenu>
#include <QFileDialog>
#include "ruletablewidget.h"

inline QString makeRW(bool r , bool w)
{
    if( w )         return "RW";
    else if( r )    return "R";
    else            return "";
}

AccDlg::AccDlg(QString key, QString acc, QWidget *parent) :
    QDialog(parent),_currentKey(key),_accModify(acc)
{
    createContents();
    createMenu();

    //
    // connections
    //

    connect(_btnOK,&QPushButton::clicked,this,&AccDlg::onBtnOK);
    connect(_btnCancel,&QPushButton::clicked,this,[=](){
        this->reject();
    });
    connect(_btnNew,&QPushButton::clicked,[=](){
        _addMenu->popup(this->mapToGlobal(_btnNew->geometry().topRight()));
    });
    connect(_btnRemove,&QPushButton::clicked,[=](){
        if( _maskTable->selectedItems().isEmpty() == false ){
            QMessageBox::StandardButton chosen = QMessageBox::question(this,tr("警告"),tr("确定删除所选条目？"));
            if( chosen == QMessageBox::Yes ){
                _maskTable->removeSelectedRows();
            }
        }
    });
    connect(_chooseAction,&QAction::triggered,this,[=](){
        QString root = MySettings::instance().getPathValue(key);
        QString dir = QFileDialog::getExistingDirectory(nullptr, tr("选择二级目录"), root, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(dir.isEmpty()==false && dir.contains(root) && dir!=root ){
            QString path = dir.right(dir.length()-root.length());
            if( path.startsWith("/") ){
                path = path.right(path.length()-1);
            }
            auto newItem = _maskTable->addRuleRow(path.split("/")[0],true,false);
            _maskTable->editItem(newItem);
        }
    });
    connect(_typeAction,&QAction::triggered,this,[=](){
        auto newItem = _maskTable->addRuleRow(createDirName(),true,false);
        _maskTable->editItem(newItem);
    });


    //
    // 初始化
    //

    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("添加账户"));
    if( isAddMode() == false ){
        setWindowTitle(tr("修改账户"));
        auto accs = MySettings::instance().getAccsFromRulestr(key);
        auto rules = MySettings::instance().getRulesFromRulestr(key, acc);

        _editUser->setText( acc );
        if( acc == GUESTUSER ){
            _editUser->setText( tr("访客") );
            _editPwd->setEnabled(false);
        }else{
            _editPwd->setText( accs[acc] );
        }

        _cbRead->setChecked( rules[ROOTPATH].contains("R") );
        _cbWrite->setChecked( rules[ROOTPATH].contains("W") );

        for(auto r: rules.keys()){
            if(r != ROOTPATH){
                _maskTable->addRuleRow(r,rules[r].contains("R"),rules[r].contains("W"));
            }
        }

        _editUser->setEnabled(false);
    }
}

void AccDlg::onBtnOK()
{
    QString user = _editUser->text();
    if( isAddMode() == false && user==tr("访客") ){
        user = GUESTUSER;
    }
    QString pwd = _editPwd->text();

    QMap<QString, QString> rules;
    rules[ROOTPATH] = makeRW(_cbRead->isChecked(),_cbWrite->isChecked());
    _maskTable->collect(rules);

    if( isAddMode() ){ //新增
        if( user.isEmpty() || pwd.isEmpty() ){
            QMessageBox::critical(nullptr,tr("提示"),tr("账户信息不能为空！"));
            return;
        }
    }

    MySettings::instance().addUser(_currentKey,user,pwd,rules);
    this->accept();
}

QString AccDlg::createDirName()
{
    return  QString("NewDir%1").arg(_maskTable->rowCount()+1);
}


void AccDlg::createMenu()
{
    _addMenu = new QMenu(this);
    _chooseAction = _addMenu->addAction(tr("选择已存在目录"));
    _typeAction = _addMenu->addAction(tr("手动输入"));
}

void AccDlg::createContents()
{
    _editUser = new QLineEdit(this);
    _editUser->setFixedWidth(120);
    auto userLayout = new QHBoxLayout;
    userLayout->addWidget(new QLabel(tr("用户："),this));
    userLayout->addWidget( _editUser );
    userLayout->addStretch(1);

    _editPwd = new QLineEdit(this);
    _editPwd->setFixedWidth(120);
    _editPwd->setEchoMode(QLineEdit::Password);
    auto pwdLayout = new QHBoxLayout;
    pwdLayout->addWidget(new QLabel(tr("密码："),this));
    pwdLayout->addWidget( _editPwd );
    pwdLayout->addStretch(1);

    _cbRead = new QCheckBox(tr("读"),this);
    _cbRead->setChecked(true);
    _cbWrite = new QCheckBox(tr("写"),this);
    auto maskLayout = new QHBoxLayout;
    maskLayout->addWidget(new QLabel(tr("根目录访问权限："),this));
    maskLayout->addWidget( _cbRead );
    maskLayout->addWidget( _cbWrite );
    maskLayout->addStretch(1);

    auto line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);

    _maskTable = new RuleTableWidget(this);

    _btnNew = new QPushButton(tr("添加"),this);
    _btnRemove = new QPushButton(tr("删除"),this);
    QVBoxLayout* ctrlLayout = new QVBoxLayout;
    ctrlLayout->setMargin(0);
    ctrlLayout->addSpacing(21);
    ctrlLayout->addWidget( _btnNew );
    ctrlLayout->addWidget( _btnRemove );
    ctrlLayout->addStretch(1);

    auto maskPanelLayout = new QHBoxLayout;
    maskPanelLayout->addWidget(_maskTable,1);
    maskPanelLayout->addLayout(ctrlLayout );

    auto line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);

    _btnOK = new QPushButton(tr("确定"),this);
    _btnCancel = new QPushButton(tr("取消"),this);
    QHBoxLayout* menuBtnLayout = new QHBoxLayout;
    menuBtnLayout->addStretch(1);
    menuBtnLayout->addWidget( _btnOK );
    menuBtnLayout->addWidget( _btnCancel );

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20,15,20,15);
    mainLayout->addLayout( userLayout );
    mainLayout->addLayout( pwdLayout );
    mainLayout->addSpacing(6);
    mainLayout->addLayout( maskLayout );
    mainLayout->addSpacing(12);
    mainLayout->addWidget( line );
    mainLayout->addSpacing(12);
    mainLayout->addLayout( maskPanelLayout );
    mainLayout->addSpacing(12);
    mainLayout->addWidget( line2 );
    mainLayout->addSpacing(24);
    mainLayout->addLayout( menuBtnLayout );
    mainLayout->addStretch(1);
}

