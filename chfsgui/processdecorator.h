#ifndef PROCESSDECORATOR_H
#define PROCESSDECORATOR_H
#include <QObject>

class ProcessDecorator : public QObject
{
    Q_OBJECT
public:
    explicit ProcessDecorator(QObject *parent = 0);
    ~ProcessDecorator();

    QString makeCHFS()const;
    void clearCHFS()const;
signals:

public slots:
};

#endif // PROCESSDECORATOR_H
