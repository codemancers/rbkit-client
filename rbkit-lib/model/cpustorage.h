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

        void traverseCallGraph(RBKit::CpuNodePtr, QStandardItem &cgRootNode, QStandardItem *parent);
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

        void traverseFlatProfile(QStandardItem &fgRootNode);
        void handleCallGraph(QStandardItem &cgRootNode);
        QHash<QString, RBKit::CpuNodePtr> getNodes();

        QList<QStandardItem*> prepareRow(QString methodName, int selfCount, int totalCount);

        inline void incrementSampleCount() {
            this->sample_count++;
        }

        static CpuStoragePtr getStorage();

    private:
        CpuStorage();

    public:
    signals:
        void updateTreeModel(QStandardItemModel*);

    public slots:
        void fillCallGraphModel(QStandardItemModel*);
        void fillFlatProfileModel(QStandardItemModel*);
    };


}

#endif // STORAGE_H
