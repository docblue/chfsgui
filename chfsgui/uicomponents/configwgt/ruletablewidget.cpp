#include "ruletablewidget.h"
#include "precompile.h"
#include "utilities.h"

MaskCellWidget::MaskCellWidget(QWidget *parent):QWidget(parent)
{
    _cbRead = new QCheckBox(tr("读"),this);
    _cbWrite = new QCheckBox(tr("写"),this);
    _cbDelete = new QCheckBox(tr("删除 "),this);
    auto maskLayout = new QHBoxLayout(this);
    maskLayout->setMargin(0);
    maskLayout->addWidget( _cbRead );
    maskLayout->addWidget( _cbWrite );
    maskLayout->addWidget( _cbDelete );
    maskLayout->addSpacing(16);
}

void MaskCellWidget::setMask(bool r, bool w)
{
    _cbRead->setChecked(r);
    _cbWrite->setChecked(w);
}

QString MaskCellWidget::getMask()
{
    return makeRW(_cbRead->isChecked(),_cbWrite->isChecked(),_cbDelete->isChecked());
}

RuleTableWidget::RuleTableWidget(QWidget *parent) : QTableWidget(parent)
{
    setColumnCount(2);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::NoSelection);

    setHorizontalHeaderItem(0, new QTableWidgetItem(QIcon(":/res/image/dir.svg"),tr("二级目录")));
    horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(1, new QTableWidgetItem(QIcon(":/res/image/mask.svg"),tr("访问权限")));
    horizontalHeaderItem(1)->setTextAlignment(Qt::AlignLeft);
}

QTableWidgetItem* RuleTableWidget::addRuleRow(QString dir, bool r, bool w)
{
    const int newRowCount = rowCount()+1;
    setRowCount( newRowCount );

    auto newItem = new QTableWidgetItem(dir);
    setItem(newRowCount-1, 0, newItem);

    auto maskWidget = new MaskCellWidget(this);
    maskWidget->setMask(r, w);
    setCellWidget(newRowCount-1, 1, maskWidget);

    this->resizeColumnToContents(1);

    return (newItem);
}

void RuleTableWidget::removeSelectedRows()
{
    removeRow( currentRow() );
}

void RuleTableWidget::collect(QMap<QString, QString>& rules)
{
    for(int i=0; i<rowCount(); i++){
        rules[item(i,0)->text()] = dynamic_cast<MaskCellWidget*>(cellWidget(i,1))->getMask();
    }
}
