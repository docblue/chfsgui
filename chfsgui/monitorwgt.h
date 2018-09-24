#ifndef MONITORWGT_H
#define MONITORWGT_H

#include "uicomponents/qrpopup.h"
#include <QTextBrowser>
#include <QMouseEvent>
#include <QListWidget>

class MyTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit MyTextBrowser(QWidget *parent = 0):QTextBrowser(parent)
    {
    }

    void mouseMoveEvent(QMouseEvent * e) override
    {
        QTextBrowser::mouseMoveEvent(e);

        QString url = anchorAt(e->pos());
        if( url.isEmpty() ){//关闭QR
            QRPopup::instance().hide();
        }else{
            if( _oldAnchor != url ){//更新QR
                QRPopup::instance().encode(url);
            }
            //移动QR
            QRPopup::instance().setGeometry(QRect(e->globalPos()+QPoint(16,16),QRPopup::instance().size()));
            QRPopup::instance().show();
        }

        _oldAnchor = url;
    }

    void mouseReleaseEvent(QMouseEvent * e) override
    {
        QTextBrowser::mouseReleaseEvent(e);

        QRPopup::instance().hide();
        _oldAnchor = "";
    }
private:
    QString _oldAnchor;
signals:

public slots:
};


class QMenu;
class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    QListWidget* logList;
private:
    void createActions();
    QMenu* menu;
signals:

public slots:

};


class MonitorWgt : public QWidget
{
    Q_OBJECT
public:
    explicit MonitorWgt(QWidget *parent = 0);
    void paintEvent(QPaintEvent *) override;
signals:

public slots:
};

#endif // MONITORWGT_H
