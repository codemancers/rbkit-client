#ifndef NODE_H
#define NODE_H
#include <QString>
#include <vector>
#include <QDebug>

class Node
{
    QString methodName,
            label,
            filename,
            thread_id;

    int singleton_method;
    double line_no;

    QList<Node*> calledBy;
    QList<Node*> calls;
public:
    Node(QString methodName,
         QString label,
         QString filename,
         QString thread_id,
         int line_no,
         int singleton_method);

    void updateCalls(Node*);
    void updateCalledBy(Node*);

    bool existInCalls(Node *method);
    bool existInCalledBy(Node *method);

    QList<Node*> getCalledBy();
    QList<Node*> getCalls();

    QString getMethodName();

    void updateData(QString methodName,
                    QString label,
                    QString filename,
                    QString thread_id,
                    int line_no,
                    int singleton_method);

    //overloading << for printing using qDebug
    friend QDebug &operator<<(QDebug&, const Node&);
};

#endif // NODE_H
