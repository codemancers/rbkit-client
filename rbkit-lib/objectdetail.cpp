#include "objectdetail.h"

RBKit::ObjectDetail::ObjectDetail(QString className, quint64 objectId)
    : className(className)
    , objectId(objectId), objectGeneration(0)
{
}

void RBKit::ObjectDetail::addReference(quint64 reference)
{
    this->references.append(reference);
}

void RBKit::ObjectDetail::updateGeneration()
{
   ++objectGeneration;
}
