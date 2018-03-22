#ifndef QRPOPUP_H
#define QRPOPUP_H

class QImage;

class QRPopup : public QWidget
{
    Q_OBJECT
public:
    static QRPopup & instance()
    {
        static QRPopup _instance;
        return _instance;
    }

    void encode(QString);
    void paintEvent(QPaintEvent * event) override;
private:
    explicit QRPopup(QWidget *parent = 0);
    ~QRPopup();
    QImage* painterImage;
signals:

public slots:
};

#endif // QRPOPUP_H
