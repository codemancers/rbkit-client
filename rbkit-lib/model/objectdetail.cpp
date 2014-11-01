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

void RBKit::ObjectDetail::addReferences(QList<QVariant> _references)
{
    QList<QVariant>::const_iterator referIter;
    for(referIter = _references.constBegin();
        referIter != _references.constEnd(); ++referIter) {
        references.append(RBKit::StringUtil::hextoInt((*referIter).toString()));
    }
}

void RBKit::ObjectDetail::updateGeneration()
{
    ++objectGeneration;
}

QString RBKit::ObjectDetail::getFileLine() const
{
    if (fileName.isEmpty())
        return "";
    else
        return QString("%0:%1").arg(fileName).arg(lineNumber);
}


// ============================== static helper methods ==============================

RBKit::ObjectDetailPtr RBKit::payloadToObject(const QVariantMap& map)
{
    auto objectId = RBKit::StringUtil::hextoInt(map["object_id"].toString());
    auto className = map["class_name"].toString();

    RBKit::ObjectDetailPtr object(new RBKit::ObjectDetail(className, objectId));
    object->fileName = map["file"].toString();
    object->lineNumber = map["line"].toInt();
    object->addReferences(map["references"].toList());
    object->size = map["size"].toInt();

    return object;
}

QList<RBKit::ObjectDetailPtr> RBKit::payloadToObjects(const QVariantList& list)
{
    QList<RBKit::ObjectDetailPtr> objects;

    for (auto& entry : list) {
        auto object = RBKit::payloadToObject(entry.toMap());
        objects.append(object);
    }

    return objects;
}
