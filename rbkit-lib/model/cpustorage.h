#ifndef STORAGE_H
#define STORAGE_H
#include <QHash>
#include "cpunode.h"
#include <QVariant>
#include <vector>
#include <QStandardItem>
#include <QStandardItemModel>

namespace RBKit
{
    class CpuStorage;
    typedef QSharedPointer<CpuStorage> CpuStoragePtr;

    class CpuStorage : public QObject
    {
        Q_OBJECT;

        unsigned long long sample_count;
        QHash<QString, RBKit::CpuNodePtr> nodes;
        QList<QString> notReached;
        QStandardItemModel *callGraphModel = new QStandardItemModel;
        QStandardItemModel *flatGraphModel = new QStandardItemModel;

        //creating invisible root nodes
        QStandardItem *cgRootNode = callGraphModel->invisibleRootItem();
        QStandardItem *fgRootNode = flatGraphModel->invisibleRootItem();

        void traverseCallGraph(RBKit::CpuNodePtr, QList<QStandardItem *> *parent);
    public:
        QList<QString> currentStack;

        void addNewNode(QMap<int, QVariant>);
        void updateNewNodeLocation(QString methodName, RBKit::CpuNodePtr);
        bool exists(QVariant name);
        unsigned long long getSampleCount();
        void traverseNodes();
        void clearFrameStack();
        void updateExistingMethod(QMap<int, QVariant>);
        void updateSelfCount();

        void traverseFlatProfile();
        void handleCallGraph();
        QHash<QString, RBKit::CpuNodePtr> getNodes();

        QList<QStandardItem*> prepareRow(QString methodName, int selfCount, int totalCount);

        inline void incrementSampleCount() {
            sample_count++;
        }

        static CpuStoragePtr getStorage();

    private:
        CpuStorage();

    public:
    signals:
        void updateTreeModel(QStandardItemModel*);

    public slots:
        void stopCpuProfiling();
        void changeToFlatProfile();
        void changeToCallGraph();
    };


}

#endif // STORAGE_H
