#ifndef CPUNODE_H
#define CPUNODE_H
#include <QString>
#include <vector>
#include <QDebug>
#include <QSharedPointer>

namespace RBKit
{
    class CpuNode;
    typedef QSharedPointer<CpuNode> CpuNodePtr;

    class CpuNode
    {
        QString methodName,
        label,
        filename,
        threadId;

        int singletonMethod;
        int lineNo;

        QList<CpuNodePtr> calledBy;
        QList<CpuNodePtr> calls;

        qint64 selfCount;
        qint64 totalCount;
    public:
        CpuNode(QString methodName,
                QString label,
                QString filename,
                QString threadId,
                int lineNo,
                int singletonMethod);

        void updateCalls(CpuNodePtr);
        void updateCalledBy(CpuNodePtr);

        bool existInCalls(CpuNodePtr method);
        bool existInCalledBy(CpuNodePtr method);

        void incrementTotalCount();
        void incrementSelfCount();

        unsigned long long getTotalCount();
        unsigned long long getSelfCount();

        QList<CpuNodePtr> getCalledBy();
        QList<CpuNodePtr> getCalls();

        QString getMethodName();

        void updateData(QString methodName,
                        QString label,
                        QString filename,
                        QString thread_id,
                        int line_no,
                        int singleton_method);

        //overloading << for printing using qDebug
        friend QDebug &operator<<(QDebug& stream, const CpuNode& node)
        {
            stream << "methodName :" << node.methodName;
            stream << "label :" << node.label;
            stream << "filename :" << node.filename;
            stream << "thread_id :" << node.threadId;
            stream << "line_no :" << node.lineNo;
            stream << "singleton_method :" << node.singletonMethod;

            return stream;
        }
    };
}

#endif // CPUNODE_H
