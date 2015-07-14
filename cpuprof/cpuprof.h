#ifndef CPUPROF_H
#define CPUPROF_H
#include <QObject>
#include <QtTest/QtTest>
#include "storage.h"

class CpuProf : public QObject
{
    Q_OBJECT

private slots:
    void testParsing();
};

#endif // CPUPROF_H
