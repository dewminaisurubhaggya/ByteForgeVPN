#pragma once
#include <QString>
#include <QSqlDatabase>

class Database
{
public:
    Database() = default;
    ~Database();
    bool open(QString *error = nullptr);
    QSqlDatabase connection() const { return db_; }

private:
    QSqlDatabase db_;
};
