#ifndef CpuNode_H
#define CpuNode_H
#include <QString>
#include <vector>
#include <QDebug>

class CpuNode
{
    QString methodName,
            label,
            filename,
            thread_id;

    int singleton_method;
    double line_no;

    QList<CpuNode*> calledBy;
    QList<CpuNode*> calls;
public:
    CpuNode(QString methodName,
         QString label,
         QString filename,
         QString thread_id,
         int line_no,
         int singleton_method);

    void updateCalls(CpuNode&);
    void updateCalledBy(CpuNode&);

    bool existInCalls(CpuNode *method);
    bool existInCalledBy(CpuNode *method);

    QList<CpuNode*> getCalledBy();
    QList<CpuNode*> getCalls();

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
