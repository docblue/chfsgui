#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>


class Toolbar;
class LauncherItemsWgt;
class ConfigWgt;
class MonitorWgt;
class QHBoxLayout;
class QMenu;
class QAction;
class QCloseEvent;
class QSystemTrayIcon;

struct MainWindowUIComponents{
    Toolbar* toolbarWgt;
    LauncherItemsWgt* launcherWgt;
    ConfigWgt* configWgt;
    MonitorWgt* monitorWgt;
    QHBoxLayout* contentLayout;
    QMenu* trayMenu;
    QSystemTrayIcon* trayicon;
};

struct SomeActions{
    QAction* autoRunAction;
};


class MainWindow : public QFrame
{
    Q_OBJECT

public:
    static MainWindow & instance()
    {
        static MainWindow _instance;
        return _instance;
    }

private:
    MainWindowUIComponents ui;

    MainWindow(QWidget *parent = 0);    
    ~MainWindow();

    void closeEvent(QCloseEvent * event) override;

    void createContents();
    void createActions();
public slots:
    void onEditorMode();
    void onRunningMode();
    void fireLaunch();
    void onShowWindow();
signals:

};

#endif // MAINWINDOW_H
