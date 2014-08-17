#include "objectdetail.h"

RBKit::ObjectDetail::ObjectDetail()
{
}

RBKit::ObjectDetail::ObjectDetail(QString className, quint64 objectId)
    : className(className)
    , objectId(objectId), objectGeneration(0)
{
}

RBKit::ObjectDetail::ObjectDetail(const RBKit::ObjectDetail &original)
    : className(original.className), objectId(original.objectId)
    , objectGeneration(original.objectGeneration), fileName(original.fileName)
    , lineNumber(original.lineNumber), references(original.references)
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
