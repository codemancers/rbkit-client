#ifndef RBKIT_APPSTATE_H
#define RBKIT_APPSTATE_H

#include <QObject>
#include <QMutex>
#include <QHash>
#include <QVariant>

namespace RBKit {

class AppState
{
    QMutex mutex;
    QHash<QString, QVariant> appState;
    QHash<int, QString> snapshotNames;
    AppState();
    static AppState *singleton;
    QString protocolVersion;
public:
    const QVariant getState(const QString& ket);
    void setAppState(const QString key, const QVariant value);
    void setSnapshotName(int key, QString snapShotName);
    QString getSnapshotName(int key) const;
    void removeSnapshotName(int key);
    static AppState* getInstance();
    QString getProtocolVersion() const;
    void setProtocolVersion(const QString &value);
};

} // namespace RBKit

#endif // RBKIT_APPSTATE_H
