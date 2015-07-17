#ifndef CPUPROF_H
#define CPUPROF_H
#include <QObject>
#include <QtTest/QtTest>
#include <QMap>
#include "cpustorage.h"

class CpuProf : public QObject
{
    Q_OBJECT
    storage *store = new storage();
public:
    void decodeMap(QList<QMap<int, QVariant>> data);
    void parseFrames(QMap<int, QVariant> *frames);

    //handeling traversals
    void startTraversals();
};

#endif // CPUPROF_H
