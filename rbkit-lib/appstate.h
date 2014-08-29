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
    AppState();
    static AppState *singleton;
public:
    const QVariant getState(const QString& ket);
    void setAppState(const QString key, const QVariant value);
    static AppState* getInstance();
};

} // namespace RBKit

#endif // RBKIT_APPSTATE_H
