#include "cpustorage.h"
#include "cpumapping.h"
#include <QDebug>
#include <QList>
#include <QTreeView>

#include "cpuprof.h"
#include "ui/appmainwindow.h"



void RBKit::CpuProf::parseFrames(QMap<int, QVariant> frames)
{
    auto methodName = frames.value(RBKit::ECeMethodName).toString();

    if (methodName.isEmpty()) {
        return;
    }

    if (!store()->exists(methodName)) {
        //method not yet added to the datastructure, add it
        store()->addNewNode(frames);
    } else {
        //method already added, update the values
        store()->updateExistingMethod(frames);
    }
}

void RBKit::CpuProf::decodeMap(QList<QMap<int, QVariant> > data)
{
    for(int i = 0; i < data.size() ; i++) {
        //qDebug() << data[i];
        //detect starting of new frame
        parseFrames(data[i]);
        store()->incrementSampleCount();
    }
    store()->updateSelfCount();
    store()->clearFrameStack();
}

void RBKit::CpuProf::startTraversals()
{
    qDebug() << "STARTING TRAVERSALS\n\n";
    //flatprofile traversal
    //store()->traverseFlatProfile();

    qDebug() << "\n\n Call Graph";
    //callgraph traversal
    //store()->handleCallGraph();

    //setting the default view as call graph
    //store()->changeToCallGraph();
}
