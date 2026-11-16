#include "Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

Database::~Database()
{
    if (db_.isValid()) {
        const QString name = db_.connectionName();
        db_.close();
        db_ = QSqlDatabase();
        QSqlDatabase::removeDatabase(name);
    }
}

bool Database::open(QString *error)
{
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (!QDir().mkpath(dataDir)) {
        if (error) *error = "Unable to create application data directory.";
        return false;
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE", "byteforge-main");
    db_.setDatabaseName(dataDir + "/byteforge.db");
    if (!db_.open()) {
        if (error) *error = db_.lastError().text();
        return false;
    }

    QSqlQuery query(db_);
    const QStringList statements = {
        "PRAGMA foreign_keys = ON",
        "CREATE TABLE IF NOT EXISTS profiles (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, protocol TEXT NOT NULL, config TEXT NOT NULL, created_at TEXT DEFAULT CURRENT_TIMESTAMP)",
        "CREATE TABLE IF NOT EXISTS subscriptions (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, url TEXT NOT NULL, updated_at TEXT)",
        "CREATE TABLE IF NOT EXISTS app_settings (key TEXT PRIMARY KEY, value TEXT NOT NULL)"
    };
    for (const auto &sql : statements) {
        if (!query.exec(sql)) {
            if (error) *error = query.lastError().text();
            return false;
        }
    }
    return true;
}
