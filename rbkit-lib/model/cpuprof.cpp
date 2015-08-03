#include "cpustorage.h"
#include "cpumapping.h"
#include <QDebug>
#include <QList>

#include "cpuprof.h"


RBKit::CpuStoragePtr store(new RBKit::CpuStorage());

void RBKit::CpuProf::parseFrames(QMap<int, QVariant> frames)
{
    auto methodName = frames.value(RBKit::ECeMethodName).toString();

    if (methodName.isEmpty()) {
        return;
    }

    if (!store->exists(methodName)) {
        //method not yet added to the datastructure, add it
        store->addNewNode(frames);
    } else {
        //method already added, update the values
        store->updateExistingMethod(frames);
    }
}

void RBKit::CpuProf::decodeMap(QList<QMap<int, QVariant> > data)
{
    for(int i = 0; i < data.size() ; i++) {
        //qDebug() << data[i];
        //detect starting of new frame
        parseFrames(data[i]);
        store->incrementSampleCount();
    }
    store->clearFrameStack();
}

void RBKit::CpuProf::startTraversals()
{
    qDebug() << "STARTING TRAVERSALS\n\n";
    //flatprofile traversal
    store->traverseFlatProfile();

    qDebug() << "\n\n Call Graph";
    //callgraph traversal
    store->handleCallGraph();
}
