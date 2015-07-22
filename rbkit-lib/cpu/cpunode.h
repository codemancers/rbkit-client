#ifndef CpuNode_H
#define CpuNode_H
#include <QString>
#include <vector>
#include <QDebug>
#include <QSharedPointer>

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
    friend QDebug &operator<<(QDebug&, const CpuNode&);
};

#endif // CpuNode_H
