#ifndef LAYOUTUTIL_HPP
#define LAYOUTUTIL_HPP

#include <QBoxLayout>
#include "common.h"

inline void makeMarginSpacingZero(QBoxLayout *layout) {
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

inline void makeMarginSpacingZero(QSharedPointer<QBoxLayout> layout) {
    makeMarginSpacingZero(layout.data());
}

#endif // LAYOUTUTIL_HPP

