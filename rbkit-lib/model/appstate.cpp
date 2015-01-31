#include "appstate.h"

namespace RBKit {

AppState* AppState::singleton = NULL;

void AppState::setAppState(const QString key, const QVariant value)
{
   mutex.lock();
   appState.insert(key, value);
   mutex.unlock();
}

void AppState::setSnapshotName(int key, QString snapShotName)
{
   snapshotNames[key] = snapShotName;
}

QString AppState::getSnapshotName(int key) const
{
    return snapshotNames[key];
}

void AppState::removeSnapshotName(int key)
{
    snapshotNames.remove(key);
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
    protocolVersion = QString("2.0");
}
QString AppState::getProtocolVersion() const
{
    return protocolVersion;
}

void AppState::setProtocolVersion(const QString &value)
{
    protocolVersion = value;
}


const QVariant AppState::getState(const QString &key)
{
    mutex.lock();
    const QVariant value = appState[key];
    mutex.unlock();
    return value;
}

} // namespace RBKit
