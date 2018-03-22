#ifndef CONFIGWGT_H
#define CONFIGWGT_H

class CfgSharedpathWgt;
class CfgAllowip;
class CfgRules;
class CfgPortWgt;

class ConfigWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWgt(QWidget *parent = 0);
    void paintEvent(QPaintEvent *) override;

    void createComponents();
    void onEditorMode();
    void onRunningMode();
private:
    CfgSharedpathWgt* _sharedpathWgt;
    CfgAllowip* _allowWgt;
    CfgRules* _rulesWgt;
    CfgPortWgt* _portWgt;
signals:

public slots:
};

#endif // CONFIGWGT_H
