#ifndef RBKIT_OBJECTTYPEROW_H
#define RBKIT_OBJECTTYPEROW_H

namespace RBKit {

class ObjectTypeRow
{
public:
    ObjectTypeRow(int typeCount);
    int typeCount;
    int referenceCount;
    int percentage;
    int referencePercentage;
};

} // namespace RBKit

#endif // RBKIT_OBJECTTYPEROW_H
