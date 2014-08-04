#include "objectdetail.h"

RBKit::ObjectDetail::ObjectDetail(QString className, quint64 objectId)
    : className(className)
    , objectId(objectId)
{
}

void RBKit::ObjectDetail::addReference(quint64 reference)
{
   this->references.append(reference);
}
