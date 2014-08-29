#include "appstate.h"

namespace RBKit {

AppState* AppState::singleton = NULL;

void AppState::setAppState(const QString key, const QVariant value)
{
   mutex.lock();
   appState.insert(key, value);
   mutex.unlock();
}

AppState *AppState::getInstance()
{
    if (!singleton) {
        singleton = new AppState();
        return singleton;
    } else {
        return singleton;
    }
}

AppState::AppState()
{
}

const QVariant AppState::getState(const QString &key)
{
    mutex.lock();
    const QVariant value = appState[key];
    mutex.unlock();
    return value;
}

} // namespace RBKit
