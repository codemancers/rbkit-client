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

        int sample_count;
        QHash<QString, RBKit::CpuNodePtr> nodes;
        QList<QString> notReached;
        QStandardItemModel *standardModel = new QStandardItemModel;

        //creating invisible root node
        QStandardItem *rootNode = standardModel->invisibleRootItem();

        void traverseCallGraph(RBKit::CpuNodePtr, int indent=1);
    public:
        QList<QString> currentStack;

        void addNewNode(QMap<int, QVariant>);
        void updateNewNodeLocation(QString methodName, RBKit::CpuNodePtr);
        bool exists(QVariant name);
        int getSampleCount();
        void traverseNodes();
        void clearFrameStack();
        void updateExistingMethod(QMap<int, QVariant>);

        void traverseFlatProfile();
        void handleCallGraph();
        QHash<QString, RBKit::CpuNodePtr> getNodes();

        inline void incrementSampleCount() {
            this->sample_count++;
        }

        static CpuStoragePtr getStorage();

    private:
        CpuStorage() {}

    public:
    signals:
        void updateTreeModel(QStandardItemModel *standardModel);

    public slots:
        void stopCpuProfiling();
        void changeToFlatProfile();
        void changeToCallGraph();
    };


}

#endif // STORAGE_H
