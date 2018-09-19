#include "accdlg.h"
#include "precompile.h"
#include <QTableWidget>
#include <QMenu>
#include <QFileDialog>
#include "ruletablewidget.h"
#include "../../utilities.h"



AccDlg::AccDlg(QString acc, QWidget *parent) :
    QDialog(parent),_accModify(acc)
{
    createContents();
    createMenu();

    //
    // connections
    //

    connect(ui.btnOK,&QPushButton::clicked,this,&AccDlg::onBtnOK);
    connect(ui._btnCancel,&QPushButton::clicked,this,[=](){
        this->reject();
    });
    connect(ui.btnNew,&QPushButton::clicked,[=](){
        ui.addMenu->popup(this->mapToGlobal(ui.btnNew->geometry().topRight()));
    });
    connect(ui.btnRemove,&QPushButton::clicked,[=](){
        if( ui.maskTable->selectedItems().isEmpty() == false ){
            QMessageBox::StandardButton chosen = QMessageBox::question(this,tr("警告"),tr("确定删除所选条目？"));
            if( chosen == QMessageBox::Yes ){
                ui.maskTable->removeSelectedRows();
            }
        }
    });
    connect(_chooseAction,&QAction::triggered,this,[=](){
        QString root = g_settings.value(PARAM_PATH).toString();
        QString dir = QFileDialog::getExistingDirectory(nullptr, tr("选择二级目录"), root, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(dir.isEmpty()==false && dir.contains(root) && dir!=root ){
            QString path = dir.right(dir.length()-root.length());
            if( path.startsWith("/") ){
                path = path.right(path.length()-1);
            }
            auto newItem = ui.maskTable->addRuleRow(path.split("/")[0],true,false);
            ui.maskTable->editItem(newItem);
        }
    });
    connect(_typeAction,&QAction::triggered,this,[=](){
        auto newItem = ui.maskTable->addRuleRow(createDirName(),true,false);
        ui.maskTable->editItem(newItem);
    });


    //
    // 初始化
    //

    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("添加账户"));
    if( isAddMode() == false ){
        setWindowTitle(tr("修改账户"));
        auto accs = getAccsFromRulestr();
        auto rules = getRulesFromRulestr(acc);

        ui.editUser->setText( acc );
        if( acc == GUESTUSER ){
            ui.editUser->setText( tr("访客") );
            ui.editPwd->setEnabled(false);
        }else{
            ui.editPwd->setText( accs[acc] );
        }

        ui.cbRead->setChecked( rules[ROOTPATH].contains("R") );
        ui.cbWrite->setChecked( rules[ROOTPATH].contains("W") );

        for(auto r: rules.keys()){
            if(r != ROOTPATH){
                ui.maskTable->addRuleRow(r,rules[r].contains("R"),rules[r].contains("W"));
            }
        }

        ui.editUser->setEnabled(false);
    }
}

void AccDlg::onBtnOK()
{
    QString user = ui.editUser->text();
    if( isAddMode() == false && user==tr("访客") ){
        user = GUESTUSER;
    }
    QString pwd = ui.editPwd->text();

    QMap<QString, QString> rules;
    rules[ROOTPATH] = makeRW(ui.cbRead->isChecked(),ui.cbWrite->isChecked(),ui.cbDelete->isChecked());
    ui.maskTable->collect(rules);

    if( isAddMode() ){ //新增
        if( user.isEmpty() || pwd.isEmpty() ){
            QMessageBox::critical(nullptr,tr("提示"),tr("账户信息不能为空！"));
            return;
        }
    }

    addUser(user,pwd,rules);
    this->accept();
}

QString AccDlg::createDirName()
{
    return  QString("NewDir%1").arg(ui.maskTable->rowCount()+1);
}


void AccDlg::createMenu()
{
    ui.addMenu = new QMenu(this);
    _chooseAction = ui.addMenu->addAction(tr("选择已存在目录"));
    _typeAction = ui.addMenu->addAction(tr("手动输入"));
}

void AccDlg::createContents()
{
    ui.editUser = new QLineEdit(this);
    ui.editUser->setFixedWidth(120);
    auto userLayout = new QHBoxLayout;
    userLayout->addWidget(new QLabel(tr("用户："),this));
    userLayout->addWidget( ui.editUser );
    userLayout->addStretch(1);

    ui.editPwd = new QLineEdit(this);
    ui.editPwd->setFixedWidth(120);
    ui.editPwd->setEchoMode(QLineEdit::Password);
    auto pwdLayout = new QHBoxLayout;
    pwdLayout->addWidget(new QLabel(tr("密码："),this));
    pwdLayout->addWidget( ui.editPwd );
    pwdLayout->addStretch(1);

    ui.cbRead = new QCheckBox(tr("读"),this);
    ui.cbRead->setChecked(true);
    ui.cbRead->setToolTip(tr("相关操作：浏览 + 下载"));
    ui.cbWrite = new QCheckBox(tr("写"),this);
    ui.cbWrite->setToolTip(tr("相关操作：读操作 + 上传 + 创建"));
    ui.cbDelete = new QCheckBox(tr("删除"),this);
    ui.cbDelete->setToolTip(tr("相关操作：写操作 + 删除"));
    auto maskLayout = new QHBoxLayout;
    maskLayout->addWidget(new QLabel(tr("根目录访问权限："),this));
    maskLayout->addWidget( ui.cbRead );
    maskLayout->addWidget( ui.cbWrite );
    maskLayout->addWidget( ui.cbDelete );
    maskLayout->setSpacing(10);
    maskLayout->addStretch(1);

    auto line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);

    ui.maskTable = new RuleTableWidget(this);

    ui.btnNew = new QPushButton(tr("添加"),this);
    ui.btnRemove = new QPushButton(tr("删除"),this);
    QVBoxLayout* ctrlLayout = new QVBoxLayout;
    ctrlLayout->setMargin(0);
    ctrlLayout->addSpacing(21);
    ctrlLayout->addWidget( ui.btnNew );
    ctrlLayout->addWidget( ui.btnRemove );
    ctrlLayout->addStretch(1);

    auto maskPanelLayout = new QHBoxLayout;
    maskPanelLayout->addWidget(ui.maskTable,1);
    maskPanelLayout->addLayout(ctrlLayout );

    auto line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);

    ui.btnOK = new QPushButton(tr("确定"),this);
    ui._btnCancel = new QPushButton(tr("取消"),this);
    QHBoxLayout* menuBtnLayout = new QHBoxLayout;
    menuBtnLayout->addStretch(1);
    menuBtnLayout->addWidget( ui.btnOK );
    menuBtnLayout->addWidget( ui._btnCancel );

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

    setMinimumWidth(420);
}

