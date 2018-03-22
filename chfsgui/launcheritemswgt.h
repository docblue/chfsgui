#ifndef LAUNCHERITEMSWGT_H
#define LAUNCHERITEMSWGT_H

#include <QWidget>
#include <QListWidget>

class QVBoxLayout;
class QToolButton;

class LauncherListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit LauncherListWidget(QWidget *parent = 0):QListWidget(parent){
    }
protected:
    virtual QSize sizeHint() const{
        if (model()->rowCount() == 0)
            return QSize(width(), 0);
        return QSize(width(), model()->rowCount()*sizeHintForRow(0));
    }
signals:
public slots:
};

#define AUTOFLAG "[自动]"

class LauncherItemsWgt : public QWidget
{
    Q_OBJECT
public:
    explicit LauncherItemsWgt(QWidget *parent = 0);
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    QString currentLaucherItemKey();
    void autoLaunch();
private:
    void floatMenubar(int vPoint);
    void createContents();

    QVBoxLayout* _menuLayout;
    LauncherListWidget* _itemListWgt;
    QList<QToolButton*> _menuBtns;
signals:
    void sigFireLaunch();
public slots:
    void onMenubarFloating();
    void onCreateLI();
    void onDeleteLI();
    void onRenameLI();
    void onAutoLI();
    void onSaveLI();
    void onItemChanging();
};

#endif // LAUNCHERITEMSWGT_H
