#ifndef CFGPORTWGT_H
#define CFGPORTWGT_H
#include "cfgbase.h"

class QLineEdit;

class CfgPortWgt : public CfgBase
{
    Q_OBJECT
public:
    explicit CfgPortWgt(QWidget *parent = 0);

    virtual void onEditorMode();
    virtual void onRunningMode();

private:
    QLineEdit* _editPort;
signals:

public slots:
};

#endif // CFGPORTWGT_H
