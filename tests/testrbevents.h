#ifndef TEST_RBKIT_EVENTS_H
#define TEST_RBKIT_EVENTS_H

#include "AutoTest.h"
#include "model/objectstore.h"

class TestRbEvents : public QObject
{
    Q_OBJECT

private slots:
    void testParseObjectCreateEvent();
    void testParseObjectDestroyEvent();
    void testParseGcStatsEvent();
    void testParseGCStartEvent();
    void testParseObjectDumpEvent();
};

#endif // TEST_RBKIT_EVENTS_H
