#ifndef CPUNODE_H
#define CPUNODE_H
#include <QString>
#include <vector>
#include <QDebug>
#include <QSharedPointer>
#include "cpucalledby.h"
#include "cpucall.h"

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

        QSet<CpuCallPtr> calls;
        QSet<CpuCalledByPtr> calledBy;

        quint64 selfCount;
        quint64 totalCount;
    public:
        CpuNode(QString methodName,
                QString label,
                QString filename,
                QString threadId,
                int lineNo,
                int singletonMethod);

        void updateCalls(CpuNodePtr);
        void updateCalledBy(CpuNodePtr);

        inline bool existInCalledBy(CpuNodePtr method) const
        {
            return calledBy.contains(method);
        }

        inline void incrementTotalCount()
        {
            ++totalCount;
        }

        inline void incrementSelfCount()
        {
            ++selfCount;
        }

        inline quint64 getTotalCount() const
        {
            return totalCount;
        }

        inline quint64 getSelfCount() const
        {
            return selfCount;
        }

        inline QSet<CpuCalledByPtr> getCalledBy()
        {
            return calledBy;
        }

        inline QSet<CpuCallPtr> getCalls()
        {
            return calls;
        }

        inline QString getMethodName() const
        {
            return methodName;
        }

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
