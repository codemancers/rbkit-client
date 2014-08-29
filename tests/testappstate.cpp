#include "testappstate.h"

TestAppState::TestAppState()
{
}

void TestAppState::testAppState()
{
   RBKit::AppState *appState = RBKit::AppState::getInstance();

   appState->setAppState("foo", QVariant(QString("Emacs")));

   const QVariant state = appState->getState("foo");
   QVERIFY(state.toString() == QString("Emacs"));
}
