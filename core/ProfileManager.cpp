#include "ProfileManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

namespace {
QString decodeBase64Url(QString value) {
    QByteArray bytes = value.toLatin1();
    bytes.replace('-', '+'); bytes.replace('_', '/');
    while (bytes.size() % 4) bytes.append('=');
    return QString::fromUtf8(QByteArray::fromBase64(bytes));
}
void fail(QString *out, const QString &message) { if (out) *out = message; }
}

ProfileManager::ProfileManager(QSqlDatabase database) : db_(database) {}

bool ProfileManager::importUri(const QString &raw, QString *error) {
    const QString input = raw.trimmed();
    if (input.isEmpty()) { fail(error, "Paste a server link first."); return false; }

    ServerProfile profile;
    profile.sourceUri = input;
    QString jsonText;
    if (input.startsWith("vmess://", Qt::CaseInsensitive)) {
        jsonText = decodeBase64Url(input.mid(8));
        QJsonParseError parseError;
        const auto doc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            fail(error, "Invalid VMess link: expected a base64-encoded JSON profile."); return false;
        }
        const auto obj = doc.object();
        profile.protocol = "VMess";
        profile.name = obj.value("ps").toString("VMess server");
        profile.server = obj.value("add").toString();
        profile.port = obj.value("port").isDouble() ? obj.value("port").toInt() : obj.value("port").toString().toInt();
    } else {
        QUrl url(input);
        const QString scheme = url.scheme().toLower();
        if (scheme == "vless") profile.protocol = "VLESS";
        else if (scheme == "trojan") profile.protocol = "Trojan";
        else if (scheme == "ss" || scheme == "shadowsocks") profile.protocol = "Shadowsocks";
        else if (scheme == "socks" || scheme == "socks5") profile.protocol = "SOCKS5";
        else if (scheme == "http" || scheme == "https") profile.protocol = scheme.toUpper();
        else { fail(error, "Unsupported link scheme. Supported imports: VLESS, VMess, Trojan, Shadowsocks, SOCKS5 and HTTP(S)."); return false; }
        profile.server = url.host();
        profile.port = url.port();
        profile.name = QUrlQuery(url).queryItemValue("remarks");
        if (profile.name.isEmpty()) profile.name = QUrl::fromPercentEncoding(url.fragment(QUrl::FullyEncoded).toUtf8());
        if (profile.name.isEmpty()) profile.name = profile.protocol + " — " + profile.server;
    }
    if (profile.server.isEmpty() || profile.port < 1 || profile.port > 65535) {
        fail(error, "The link is missing a valid server address or port."); return false;
    }

    QSqlQuery query(db_);
    query.prepare("INSERT INTO profiles(name, protocol, config) VALUES(?, ?, ?)");
    query.addBindValue(profile.name);
    query.addBindValue(profile.protocol);
    query.addBindValue(input);
    if (!query.exec()) { fail(error, query.lastError().text()); return false; }
    return true;
}

QList<ServerProfile> ProfileManager::all(QString *error) const {
    QList<ServerProfile> result;
    QSqlQuery query(db_);
    if (!query.exec("SELECT id, name, protocol, config FROM profiles ORDER BY id DESC")) {
        fail(error, query.lastError().text()); return result;
    }
    while (query.next()) {
        ServerProfile p;
        p.id = query.value(0).toLongLong(); p.name = query.value(1).toString();
        p.protocol = query.value(2).toString(); p.sourceUri = query.value(3).toString();
        if (p.sourceUri.startsWith("vmess://", Qt::CaseInsensitive)) {
            const QString payload = decodeBase64Url(p.sourceUri.mid(8));
            const auto doc = QJsonDocument::fromJson(payload.toUtf8());
            if (doc.isObject()) {
                p.server = doc.object().value("add").toString();
                p.port = doc.object().value("port").isDouble() ? doc.object().value("port").toInt() : doc.object().value("port").toString().toInt();
            }
        } else {
            QUrl url(p.sourceUri);
            p.server = url.host(); p.port = url.port();
        }
        result.append(p);
    }
    return result;
}

bool ProfileManager::remove(qint64 id, QString *error) {
    QSqlQuery query(db_); query.prepare("DELETE FROM profiles WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec()) { fail(error, query.lastError().text()); return false; }
    if (query.numRowsAffected() == 0) { fail(error, "The selected profile no longer exists."); return false; }
    return true;
}
