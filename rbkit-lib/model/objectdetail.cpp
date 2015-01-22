#include "objectdetail.h"
#include "stringutil.h"

RBKit::ObjectDetail::ObjectDetail()
    : objectId(0)
    , lineNumber(0)
    , objectGeneration(0)
    , size(0)
{
}

RBKit::ObjectDetail::ObjectDetail(QString className, quint64 objectId)
    : className(className)
    , objectId(objectId)
    , lineNumber(0)
    , objectGeneration(0)
    , size(0)
{
}

RBKit::ObjectDetail::ObjectDetail(const RBKit::ObjectDetail &original)
    : className(original.className)
    , objectId(original.objectId)
    , fileName(original.fileName)
    , lineNumber(original.lineNumber)
    , objectGeneration(original.objectGeneration)
    , references(original.references)
    , size(original.size)
{
}

void RBKit::ObjectDetail::addReference(quint64 reference)
{
    this->references.append(reference);
}

void RBKit::ObjectDetail::addReferences(QList<QVariant> _references)
{
    for (auto ref : _references) {
        references.append(ref.toULongLong());
    }
}

void RBKit::ObjectDetail::updateGeneration()
{
    ++objectGeneration;
}

QString RBKit::ObjectDetail::getFileLine()
{
    if (fileName.isEmpty())
        return "";
    else
        return QString("%0:%1").arg(fileName).arg(lineNumber);
}
