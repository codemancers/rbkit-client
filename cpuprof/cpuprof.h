#ifndef CPUPROF_H
#define CPUPROF_H
#include <QObject>
#include <QtTest/QtTest>
#include <QMap>
#include "storage.h"

class CpuProf : public QObject
{
    Q_OBJECT

public:
    static void decodeMap(QList<QMap<int, QVariant>> data) {
        qDebug() << data;
    }
private slots:
    void testParsing();
};

#endif // CPUPROF_H
