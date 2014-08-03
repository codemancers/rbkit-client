#include "objectdetail.h"

ObjectDetail::ObjectDetail(QString className, quint64 objectId)
    : className(className)
    , objectId(objectId)
{
}

void ObjectDetail::addReference(quint64 reference)
{
   this->references.append(reference);
}
