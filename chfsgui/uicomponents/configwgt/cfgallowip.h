#ifndef CFGALLOWIP_H
#define CFGALLOWIP_H
#include "cfgbase.h"
#include <QDialog>

class QListWidget;
class QRadioButton;
class QPushButton;

#define NEWALLOWIP "newip"
#define MODALLOWIP "modip"

class AllowipDlg : public QDialog
{
    Q_OBJECT
public:
    explicit AllowipDlg(QString mode, QString oldValue, QWidget *parent = 0);

    QString ips;
protected:

private:
    QString _mode;  
signals:

public slots:
};

class CfgAllowip : public CfgBase
{
    Q_OBJECT
public:
    explicit CfgAllowip(QWidget *parent = 0);

    virtual void onEditorMode();
    virtual void onRunningMode();
protected:
    QString collectAllItems();
    void onModeChanged(bool isEditor);
private:
    QListWidget* _listWgt;

    QRadioButton* _btnBlack;
    QRadioButton* _btnWhite;
    QPushButton* _btnNew;
    QPushButton* _btnRemove;
    QPushButton* _btnModify;
    QPushButton* _btnReset;
signals:

public slots:
};

#endif // CFGALLOWIP_H
