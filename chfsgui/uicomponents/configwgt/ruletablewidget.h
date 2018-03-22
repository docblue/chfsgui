#ifndef RULETABLEWIDGET_H
#define RULETABLEWIDGET_H

#include <QTableWidget>

class QCheckBox;
class MaskCellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaskCellWidget(QWidget *parent = 0);

    void setMask(bool r, bool w);
    QString getMask();
private:
    QCheckBox* _cbRead;
    QCheckBox* _cbWrite;
signals:

public slots:
};


class RuleTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit RuleTableWidget(QWidget *parent = 0);

    QTableWidgetItem* addRuleRow(QString dir, bool r, bool w);
    void removeSelectedRows();
    void collect(QMap<QString, QString>&);
signals:

public slots:
};

#endif // RULETABLEWIDGET_H
