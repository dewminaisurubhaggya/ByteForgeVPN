#include "Pages.h"
#include "../core/ProfileManager.h"
#include <QSqlDatabase>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QCheckBox>

static QLabel *muted(const QString &s) {
    auto *l = new QLabel(s); l->setStyleSheet("color:#94a3b8;"); return l;
}
static QGroupBox *panel(const QString &title, QVBoxLayout *parent) {
    auto *g = new QGroupBox(title); parent->addWidget(g); return g;
}

DashboardPage::DashboardPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *intro = new QLabel("Your connection at a glance");
    intro->setStyleSheet("font-size:15pt;font-weight:600;");
    l->addWidget(intro);
    auto *status = panel("Connection", l);
    auto *v = new QVBoxLayout(status);
    auto *s = new QLabel("Disconnected");
    s->setStyleSheet("font-size:22pt;font-weight:600;color:#fbbf24;");
    v->addWidget(s);
    v->addWidget(muted("Select a server before connecting. Engine integration will be added in Phase 3."));
    auto *stats = panel("Traffic statistics", l);
    auto *grid = new QHBoxLayout(stats);
    for (const auto &pair : {QStringList{"Download", "—"}, QStringList{"Upload", "—"},
                             QStringList{"Total received", "—"}, QStringList{"Total sent", "—"},
                             QStringList{"Latency", "—"}}) {
        auto *box = new QWidget; auto *b = new QVBoxLayout(box);
        b->addWidget(muted(pair[0])); auto *value = new QLabel(pair[1]);
        value->setStyleSheet("font-size:16pt;font-weight:600;"); b->addWidget(value);
        grid->addWidget(box);
    }
    l->addStretch();
}

ServersPage::ServersPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *top = new QHBoxLayout;
    top->addWidget(new QLabel("Saved server profiles")); top->addStretch();
    auto *importButton = new QPushButton("Import link");
    auto *removeButton = new QPushButton("Delete selected");
    top->addWidget(importButton); top->addWidget(removeButton); l->addLayout(top);
    auto *table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels({"Name", "Protocol", "Server", "Port"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    l->addWidget(table);
    auto *hint = muted("Imported profiles are stored locally. Engine connection support is a separate integration step.");
    l->addWidget(hint);

    ProfileManager manager(QSqlDatabase::database("byteforge-main"));
    auto refresh = [table, manager]() mutable {
        QString error;
        const auto profiles = manager.all(&error);
        if (!error.isEmpty()) { QMessageBox::critical(table, "Profile database error", error); return; }
        table->setRowCount(0);
        for (const auto &profile : profiles) {
            const int row = table->rowCount(); table->insertRow(row);
            auto *name = new QTableWidgetItem(profile.name); name->setData(Qt::UserRole, profile.id);
            table->setItem(row, 0, name); table->setItem(row, 1, new QTableWidgetItem(profile.protocol));
            table->setItem(row, 2, new QTableWidgetItem(profile.server));
            table->setItem(row, 3, new QTableWidgetItem(profile.port ? QString::number(profile.port) : "—"));
        }
    };
    connect(importButton, &QPushButton::clicked, this, [this, manager, refresh]() mutable {
        bool ok = false;
        const QString uri = QInputDialog::getMultiLineText(this, "Import server link", "Paste a VLESS, VMess, Trojan, Shadowsocks, SOCKS5 or HTTP(S) link:", {}, &ok);
        if (!ok) return;
        QString error;
        if (!manager.importUri(uri, &error)) { QMessageBox::warning(this, "Import failed", error); return; }
        refresh();
    });
    connect(removeButton, &QPushButton::clicked, this, [this, table, manager, refresh]() mutable {
        const int row = table->currentRow();
        if (row < 0) { QMessageBox::information(this, "Select a profile", "Choose a profile to delete."); return; }
        const qint64 id = table->item(row, 0)->data(Qt::UserRole).toLongLong();
        if (QMessageBox::question(this, "Delete profile", "Delete the selected profile?") != QMessageBox::Yes) return;
        QString error;
        if (!manager.remove(id, &error)) { QMessageBox::warning(this, "Delete failed", error); return; }
        refresh();
    });
    refresh();
}

SubscriptionsPage::SubscriptionsPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *form = panel("Add subscription", l);
    auto *f = new QFormLayout(form);
    f->addRow("Subscription name", new QLineEdit);
    f->addRow("Subscription URL", new QLineEdit);
    auto *actions = new QHBoxLayout; actions->addStretch(); actions->addWidget(new QPushButton("Add subscription"));
    l->addLayout(actions);
    auto *table = new QTableWidget(0, 3);
    table->setHorizontalHeaderLabels({"Name", "Profiles", "Last updated"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    l->addWidget(table);
    l->addWidget(muted("Subscription networking and parsing are scheduled for Phase 6."));
}

TunnelingPage::TunnelingPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *g = panel("Tunnel configuration", l);
    auto *f = new QFormLayout(g);
    auto *type = new QComboBox; type->addItems({"Use selected server profile", "SSH tunnel", "TLS / SSL tunnel", "Custom proxy"});
    f->addRow("Tunnel type", type);
    f->addRow("Host", new QLineEdit);
    f->addRow("Port", new QLineEdit);
    f->addRow("Username", new QLineEdit);
    f->addRow("SNI / Host header", new QLineEdit);
    f->addRow("WebSocket path", new QLineEdit);
    l->addWidget(muted("These controls are a layout foundation only; no tunnel is started or saved yet."));
    l->addStretch();
}

RoutingPage::RoutingPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *g = panel("Routing mode", l);
    auto *f = new QFormLayout(g);
    auto *mode = new QComboBox; mode->addItems({"Global proxy", "Direct", "Rule-based (planned)"});
    f->addRow("Default route", mode);
    auto *dns = new QLineEdit; dns->setPlaceholderText("e.g. 1.1.1.1");
    f->addRow("DNS server", dns);
    l->addWidget(muted("TUN, split tunneling, DNS protection and rule validation will be implemented in later phases."));
    l->addStretch();
}

LogsPage::LogsPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *top = new QHBoxLayout; top->addWidget(new QLabel("Application diagnostics")); top->addStretch();
    top->addWidget(new QPushButton("Export report")); l->addLayout(top);
    auto *log = new QPlainTextEdit; log->setReadOnly(true);
    log->setPlainText("Byte Forge VPN started.\nFoundation build: engine not configured.\nNo connection has been attempted.");
    l->addWidget(log);
    l->addWidget(muted("Sensitive credentials must never be written to diagnostic logs."));
}

SettingsPage::SettingsPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *g = panel("General", l);
    auto *f = new QFormLayout(g);
    auto *theme = new QComboBox; theme->addItems({"Dark", "Light (planned)"});
    f->addRow("Theme", theme);
    f->addRow("", new QCheckBox("Start with Windows (planned)"));
    f->addRow("", new QCheckBox("Minimize to system tray (planned)"));
    auto *c = panel("Connection", l);
    auto *cf = new QFormLayout(c);
    auto *mode = new QComboBox; mode->addItems({"System Proxy", "TUN (planned)", "Custom Tunnel"});
    cf->addRow("Default mode", mode);
    cf->addRow("Connection timeout", new QLineEdit("10 seconds"));
    l->addStretch();
}

AboutPage::AboutPage(QWidget *p) : QWidget(p)
{
    auto *l = new QVBoxLayout(this);
    auto *name = new QLabel("Byte Forge VPN");
    name->setStyleSheet("font-size:22pt;font-weight:700;");
    l->addWidget(name);
    l->addWidget(new QLabel("Windows VPN and tunneling client"));
    l->addWidget(muted("Version 0.1.0 — Phase 1 foundation"));
    l->addWidget(muted("Built with C++20, Qt 6 Widgets, CMake and SQLite."));
    l->addStretch();
}
