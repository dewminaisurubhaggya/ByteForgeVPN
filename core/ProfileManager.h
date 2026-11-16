#pragma once
#include <QString>
#include <QList>
#include <QSqlDatabase>

struct ServerProfile {
    qint64 id = 0;
    QString name;
    QString protocol;
    QString server;
    int port = 0;
    QString sourceUri;
};

class ProfileManager {
public:
    explicit ProfileManager(QSqlDatabase database);
    bool importUri(const QString &uri, QString *error = nullptr);
    QList<ServerProfile> all(QString *error = nullptr) const;
    bool remove(qint64 id, QString *error = nullptr);
private:
    QSqlDatabase db_;
};
