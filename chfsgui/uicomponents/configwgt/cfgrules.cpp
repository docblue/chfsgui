#include "cfgrules.h"
#include "precompile.h"
#include "mysettings.h"
#include <QListWidget>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "utilities.h"
#include "accdlg.h"


CfgRules::CfgRules(QWidget *parent) : CfgBase(parent)
{
    createContents();

    //
    // connections
    //

    connect(_accList, &QListWidget::currentRowChanged,this,&CfgRules::accChanged);
    connect(&MySettings::instance(),&MySettings::currentItemChanged,this,&CfgRules::launchitemChanged);
    connect(_btnAccNew,&QPushButton::clicked,this,[=](){
        AccDlg dlg(_currentKey,"",this);
        dlg.exec();

        int oldRow = _accList->currentRow();
        launchitemChanged(_currentKey);
        _accList->setCurrentRow( oldRow );
    });
    connect(_btnAccModify,&QPushButton::clicked,this,[=](){
        auto item = _accList->currentItem();
        if(item){
            AccDlg dlg(_currentKey,item->data(Qt::UserRole).toString(),this);
            dlg.exec();

            int oldRow = _accList->currentRow();
            launchitemChanged(_currentKey);
            _accList->setCurrentRow( oldRow );
        }
    });
    connect(_btnAccRemove,&QPushButton::clicked,[=](){
        auto item = _accList->currentItem();
        auto currentUser = item->data(Qt::UserRole).toString();
        if(item && currentUser!=GUESTUSER){
            QMessageBox::StandardButton chosen = QMessageBox::question(this,tr("警告"),tr("确定删除所选账户？"));
            if( chosen == QMessageBox::Yes ){
                delete _accList->takeItem(_accList->currentRow());
                MySettings::instance().removeUser(_currentKey,currentUser);
            }
        }
    });
}

void CfgRules::launchitemChanged(QString key)
{
    _accList->clear();
    _currentKey = key;
    auto accs = MySettings::instance().getAccsFromRulestr(key);

    QListWidgetItem* item = new QListWidgetItem(tr("访客"));
    item->setData(Qt::UserRole,GUESTUSER);
    _accList->addItem(item);

    for(auto i: accs.keys()){
        if( i != GUESTUSER){
            QListWidgetItem* item = new QListWidgetItem(i);
            item->setData(Qt::UserRole,i);
            _accList->addItem(item);
        }
    }

    _accList->setCurrentRow(0);
}

void CfgRules::accChanged()
{
    _accessTree->clear();

    auto item = _accList->currentItem();
    if(item){
        auto rules = MySettings::instance().getRulesFromRulestr(_currentKey, item->data(Qt::UserRole).toString());

        QTreeWidgetItem *root = new QTreeWidgetItem((QTreeWidget*)0,QStringList{ROOTPATH,rule2humanbeings(rules[ROOTPATH])});
        auto i = rules.constBegin();
        while (i != rules.constEnd()) {
            QString dir = i.key();
            QString mask = i.value();
            if( dir !=  ROOTPATH ){
                root->addChild( new QTreeWidgetItem(root,QStringList{dir,rule2humanbeings(mask)}) );
            }
            ++i;
        }
        _accessTree->addTopLevelItem( root );

        _accessTree->expandAll();
        resizeTreeColumn();
    }
}

void CfgRules::resizeEvent(QResizeEvent * event)
{
    resizeTreeColumn();
    CfgBase::resizeEvent(event);
}

void CfgRules::resizeTreeColumn()
{
    _accessTree->resizeColumnToContents(0);
    _accessTree->setColumnWidth(0, _accessTree->columnWidth(0)+20);
}

void CfgRules::createContents()
{
    _accList = new QListWidget(this);
    QVBoxLayout* accLayout = new QVBoxLayout;
    accLayout->setMargin(0);
    accLayout->addWidget( new QLabel(tr("账户:"),this) );
    accLayout->addWidget( _accList, 1 );

    _btnAccNew = new QPushButton(tr("添加"),this);
    _btnAccRemove = new QPushButton(tr("删除"),this);
    _btnAccModify = new QPushButton(tr("修改"),this);
    QVBoxLayout* ctrlAccLayout = new QVBoxLayout;
    ctrlAccLayout->setMargin(0);
    ctrlAccLayout->addSpacing(21);//暂时就这样简单处理
    ctrlAccLayout->addWidget( _btnAccNew );
    ctrlAccLayout->addWidget( _btnAccRemove );
    ctrlAccLayout->addWidget( _btnAccModify );
    ctrlAccLayout->addStretch(1);

    _accessTree = new QTreeWidget(this);
    _accessTree->setItemsExpandable(false);
    _accessTree->setColumnCount(2);
    QTreeWidgetItem* itemHeader = new QTreeWidgetItem(QStringList{tr("目录"),tr("权限")});
    itemHeader->setIcon(0,QIcon(":/res/image/dir.svg"));
    itemHeader->setIcon(1,QIcon(":/res/image/mask.svg"));
    _accessTree->setHeaderItem( itemHeader );
    _accessTree->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addLayout( accLayout, 4 );
    mainLayout->addLayout( ctrlAccLayout );
    mainLayout->addStretch(2);
    mainLayout->addWidget( _accessTree, 10 );
    mainLayout->addStretch(1);
}

void CfgRules::onEditorMode()
{
    onModeChanged(true);
}

void CfgRules::onRunningMode()
{
    onModeChanged(false);
}

void CfgRules::onModeChanged(bool isEditor)
{
    _btnAccNew->setVisible(isEditor);
    _btnAccRemove->setVisible(isEditor);
    _btnAccModify->setVisible(isEditor);
}
