#include "launcheritemswgt.h"
#include "precompile.h"
#include <QToolButton>
#include <QFileDialog>
#include "mysettings.h"



LauncherItemsWgt::LauncherItemsWgt(QWidget *parent) : QWidget(parent)
{
    createContents();

    //
    // connections
    //

    connect(_itemListWgt,&LauncherListWidget::currentRowChanged,this,LauncherItemsWgt::onMenubarFloating);
    connect(_itemListWgt,&LauncherListWidget::currentItemChanged,this,&LauncherItemsWgt::onItemChanging);
    connect(_itemListWgt,&LauncherListWidget::doubleClicked,this,&LauncherItemsWgt::sigFireLaunch);
    connect(_menuBtns[0],&QToolButton::clicked,this,&LauncherItemsWgt::onCreateLI);
    connect(_menuBtns[1],&QToolButton::clicked,this,&LauncherItemsWgt::onDeleteLI);
    connect(_menuBtns[2],&QToolButton::clicked,this,&LauncherItemsWgt::onRenameLI);
    connect(_menuBtns[3],&QToolButton::clicked,this,&LauncherItemsWgt::onAutoLI);
    connect(_menuBtns[4],&QToolButton::clicked,this,&LauncherItemsWgt::onSaveLI);

    //
    // 一些初始化工作
    //

    QListWidgetItem* defItem = nullptr;
    QListWidgetItem* lastusedItem = nullptr;
    QListWidgetItem* autorunItem = nullptr;

    for(auto i: MySettings::instance().getAllItems()){
        QString display = i._key+QString(i._isAutoRun?AUTOFLAG:"");
        QListWidgetItem* item = new QListWidgetItem(display);
        item->setData(Qt::UserRole,i._key);

        if( i._key==DEFAULTITEM){
            _itemListWgt->insertItem(0,item);
            defItem = item;
        }else{
            _itemListWgt->addItem(item);
        }

        if( i._isLastUsed ) lastusedItem = item;
        if( i._isAutoRun ) autorunItem = item;
    }

    if( autorunItem )            _itemListWgt->setCurrentItem(autorunItem);
    else if(lastusedItem)        _itemListWgt->setCurrentItem(lastusedItem);
    else                         _itemListWgt->setCurrentItem(defItem);
}

void LauncherItemsWgt::autoLaunch()
{
    auto item = _itemListWgt->currentItem();
    if(item){
        QString key = item->data(Qt::UserRole).toString();
        if( MySettings::instance().getItem(key)._isAutoRun ){
            emit sigFireLaunch();
        }
    }
}

QString LauncherItemsWgt::currentLaucherItemKey()
{
    QString res;

    auto item = _itemListWgt->currentItem();
    if(item){
        res = item->data(Qt::UserRole).toString();
    }

    return res;
}

void LauncherItemsWgt::onItemChanging()
{
    auto item = _itemListWgt->currentItem();
    if(item){
        MySettings::instance().onCurrentItemChanging(item->data(Qt::UserRole).toString());
    }
}

void LauncherItemsWgt::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void LauncherItemsWgt::floatMenubar(int vPoint)
{
    int barHeight = 0;
    for(auto i:_menuBtns){
        barHeight += i->size().height();
    }
    int spacerHeight = vPoint - barHeight/2;
    if( spacerHeight < 0 ){
        spacerHeight = 0;
    }
    if(spacerHeight+barHeight>size().height()){
        spacerHeight = size().height() - barHeight;
    }

    _menuLayout->itemAt(0)
            ->spacerItem()
            ->changeSize(0,spacerHeight);
    _menuLayout->update();
}

void LauncherItemsWgt::onMenubarFloating()
{
    QListWidgetItem* item = _itemListWgt->currentItem();
    if( item == nullptr ){
        return;
    }

    QRect rtInListWgt = _itemListWgt->visualItemRect(item);
    QRect rtListWgt = _itemListWgt->geometry();

    floatMenubar(rtListWgt.top()+rtInListWgt.top()+rtInListWgt.height()/2);
}


void LauncherItemsWgt::resizeEvent(QResizeEvent * e)
{
    QWidget::resizeEvent(e);
    onMenubarFloating();
}


void LauncherItemsWgt::createContents()
{
    //
    // 创建启动项
    //

    _itemListWgt = new LauncherListWidget(this);
    _itemListWgt->setToolTip(tr("双击启动项可以直接启动运行。"));


    QVBoxLayout* itemsLayout = new QVBoxLayout;
    itemsLayout->setSpacing(0);
    itemsLayout->setMargin(0);
    itemsLayout->addStretch(1);
    itemsLayout->addWidget(_itemListWgt);
    itemsLayout->addStretch(1);

    //
    // 菜单栏
    //

    //排列顺序：增加，删除，重命名，自动运行，保存为文件
    for(int i=0;i<5;i++){
        QToolButton* btn = new QToolButton(this);
        btn->setFixedSize(36,36);
        btn->setIconSize(QSize(36,36));
        btn->setIcon(QIcon(QString(":/IM%1").arg(i)));
        _menuBtns.append(btn);
    }
    _menuBtns[0]->setToolTip(tr("新建启动项"));
    _menuBtns[1]->setToolTip(tr("删除启动项"));
    _menuBtns[2]->setToolTip(tr("重命名启动项"));
    _menuBtns[3]->setToolTip(tr("启用/禁用启动项的自动运行"));
    _menuBtns[4]->setToolTip(tr("将启动项保存为CHFS命令行程序的配置文件"));

    _menuLayout = new QVBoxLayout;
    _menuLayout->setSpacing(0);
    _menuLayout->setMargin(0);
    _menuLayout->addSpacerItem(new QSpacerItem(0,100));
    for(auto i:_menuBtns){
        _menuLayout->addWidget(i);
    }
    _menuLayout->addStretch(1);

    //
    // 整体布局
    //

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(itemsLayout);
    mainLayout->addLayout(_menuLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(6,0,1,0);
}

void LauncherItemsWgt::onDeleteLI()
{
    QMessageBox::StandardButton chosen = QMessageBox::question(this,tr("警告"),tr("确定删除所选启动项？"));
    if( chosen == QMessageBox::Yes ){
        QListWidgetItem* currentItem = _itemListWgt->currentItem();
        if( currentItem ){
            QString key = currentItem->data(Qt::UserRole).toString();
            if( key == DEFAULTITEM ){
                QMessageBox::critical(this,tr("提示"),tr("默认启动项不能被删除！"));
            }else{
                MySettings::instance().remove(key);
                _itemListWgt->removeItemWidget(currentItem);
                delete currentItem;
                _itemListWgt->updateGeometry();
            }
        }
    }
}

void LauncherItemsWgt::onCreateLI()
{
    LauncherItem i = MySettings::instance().onCreateSaveNewItem();

    QListWidgetItem* item = new QListWidgetItem(i._key,_itemListWgt);
    item->setData(Qt::UserRole,i._key);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    _itemListWgt->addItem(item);
    _itemListWgt->updateGeometry();

    _itemListWgt->setCurrentItem(item);
    connect(_itemListWgt->itemDelegate(), &QAbstractItemDelegate::closeEditor,this,[=](){
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _itemListWgt->itemDelegate()->disconnect(this);

        bool exist = false;
        for(int i=0;i<_itemListWgt->count();i++){
            auto it = _itemListWgt->item(i);
            if( item!=it && it->data(Qt::UserRole).toString() == item->text() ){
                exist = true;
                break;
            }
        }

        if( exist ){
            QMessageBox::critical(this,tr("提示"),tr("该名称已存在！"));
            item->setText(item->data(Qt::UserRole).toString());
            return;
        }else{
            item->setData(Qt::UserRole,item->text());
            MySettings::instance().renameKey(i._key,item->text());
        }
    } );

    _itemListWgt->editItem(item);
}

void LauncherItemsWgt::onRenameLI()
{
    QListWidgetItem* item = _itemListWgt->currentItem();
    QString key = item->data(Qt::UserRole).toString();

    if( key == DEFAULTITEM){
        QMessageBox::critical(this,tr("提示"),tr("不能修改默认启动项！"));
        return;
    }

    item->setFlags(item->flags() | Qt::ItemIsEditable);

    connect(_itemListWgt->itemDelegate(), &QAbstractItemDelegate::closeEditor,this,[=](){
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _itemListWgt->itemDelegate()->disconnect(this);

        bool exist = false;
        for(int i=0;i<_itemListWgt->count();i++){
            auto it = _itemListWgt->item(i);
            if( item!=it && it->data(Qt::UserRole).toString() == item->text() ){
                exist = true;
                break;
            }
        }

        if( exist ){
            QMessageBox::critical(this,tr("提示"),tr("该名称已存在！"));
            item->setText(key);
            return;
        }else{
            item->setData(Qt::UserRole,item->text());
            MySettings::instance().renameKey(key,item->text());
        }
    } );

    _itemListWgt->editItem(item);
}

void LauncherItemsWgt::onAutoLI()
{
    QListWidgetItem* item = _itemListWgt->currentItem();
    if( item ){
        MySettings::instance().toggleItemAutoFlag(item->data(Qt::UserRole).toString());

        for(int i=0;i<_itemListWgt->count();i++){
            auto it = _itemListWgt->item(i);
            QString key = it->data(Qt::UserRole).toString();
            it->setText(it->data(Qt::UserRole).toString()
                        + QString(MySettings::instance().getItem(key)._isAutoRun?AUTOFLAG:""));
        }
    }
}


void LauncherItemsWgt::onSaveLI()
{
    QListWidgetItem* item = _itemListWgt->currentItem();
    if( item ){
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存配置"),"","Text(*.txt);;ini(*.ini);;*(*.*)");
        if( fileName.isEmpty() == false ){
            QFile file(fileName);
            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){
                QMessageBox::critical(this,tr("提示"),tr("保存失败！"));
                return;
            }
            QTextStream out(&file);

            const QString key = item->data(Qt::UserRole).toString();
            out << "port=" << MySettings::instance().getPortValue(key) << endl;
            out << "path=" << MySettings::instance().getPathValue(key) << endl;
            out << "allow=" << MySettings::instance().getAllowValue(key) << endl;
            out << "rule=" << MySettings::instance().getRuleValue(key) << endl;
        }
    }
}
