#ifndef CFGRULES_H
#define CFGRULES_H
#include "cfgbase.h"

class QListWidget;
class QTreeWidget;
class QPushButton;

class CfgRules : public CfgBase
{
    Q_OBJECT
public:
    explicit CfgRules(QWidget *parent = 0);
    void resizeEvent(QResizeEvent * event);

    virtual void onEditorMode();
    virtual void onRunningMode();
private:
    QTreeWidget* _accessTree;
    QListWidget* _accList;
    QPushButton* _btnAccNew;
    QPushButton* _btnAccRemove;
    QPushButton* _btnAccModify;

    void createContents();
    void resizeTreeColumn();
    void onModeChanged(bool isEditor);
signals:

public slots:
    void accChanged();
    void initAccList();
};

#endif // CFGRULES_H
