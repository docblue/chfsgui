#include "qrpopup.h"
#include <QWidget>
#include <QImage>
#include <QPainter>
#include "QR-Code-generator-master/QrCode.hpp"
using namespace qrcodegen;

QRPopup::QRPopup(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::ToolTip);
    setFixedSize(120,120);
}

QRPopup::~QRPopup()
{
    delete painterImage;
}


void QRPopup::encode(QString data)
{
    const int margin = 3;

    std::vector<QrSegment> segs =  QrSegment::makeSegments(data.toStdString().c_str());
    QrCode qr1 = QrCode::encodeSegments(segs, QrCode::Ecc::LOW, 5, 5, 2, false);

    int size = qr1.getSize() + margin*2;
    painterImage = new QImage(size,size,QImage::Format_ARGB32_Premultiplied );
    painterImage->fill( Qt::white );

    for (int y = 0; y < qr1.getSize(); y++) {
        for (int x = 0; x < qr1.getSize(); x++) {
            if ( qr1.getModule(x,y) )
                painterImage->setPixel(x+margin,y+margin,Qt::black);
        }
    }

    repaint();
}

void QRPopup::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event)

    QPainter p(this);
    p.drawPixmap(rect(),QPixmap::fromImage(*painterImage));
}
