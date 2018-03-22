#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
class QToolButton;
class QAction;


struct ToolbarActions{
    QAction* autoRunAction;
    QAction* exitAction;
    QAction* aboutAction;
};

class Toolbar : public QWidget
{
    Q_OBJECT
public:
    explicit Toolbar(QWidget *parent = 0);
    void paintEvent(QPaintEvent *) override;

    void createContents();
    void initState();
    void runningState();
    void stopState();
    void createActions();
private:
    QToolButton* _btnReturn;
    QToolButton* _btnPlay;
    QToolButton* _btnStop;
    QToolButton* _btnMenu;
signals:
    void sigEditorMode();
    void sigRunningMode();
    void sigFireLaunch();
    void sigAutorunSwitching(bool isAuto);
    void sigQuit();
public slots:

};

#endif // TOOLBAR_H
