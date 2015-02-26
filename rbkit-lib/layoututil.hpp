#ifndef LAYOUTUTIL_HPP
#define LAYOUTUTIL_HPP

#include <QBoxLayout>

inline void makeMarginSpacingZero(QBoxLayout *layout) {
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

#endif // LAYOUTUTIL_HPP

