#ifndef CFGBASE_H
#define CFGBASE_H


class CfgBase : public QWidget
{
    Q_OBJECT
public:
    explicit CfgBase(QWidget *parent = 0);

    virtual void onEditorMode() = 0;
    virtual void onRunningMode() = 0;
protected:

signals:

public slots:
};

#endif // CFGBASE_H
