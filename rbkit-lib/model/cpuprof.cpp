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
        //detect starting of new frame
        parseFrames(data[i]);
        store()->incrementSampleCount();
    }
    store()->updateSelfCount();
    store()->clearFrameStack();
}
