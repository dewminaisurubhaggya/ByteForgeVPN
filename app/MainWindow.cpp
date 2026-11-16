#include "MainWindow.h"
#include "ui/Pages.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Byte Forge VPN");
    resize(1080, 700);
    setMinimumSize(820, 560);

    auto *central = new QWidget(this);
    auto *root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto *sidebar = new QWidget;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(205);
    auto *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(16, 22, 16, 16);
    sideLayout->setSpacing(10);

    auto *brand = new QLabel("BYTE FORGE\nVPN");
    brand->setObjectName("brand");
    sideLayout->addWidget(brand);

    navigation_ = new QListWidget;
    navigation_->setObjectName("navigation");
    const QStringList items = {"Dashboard", "Servers", "Subscriptions",
                               "Tunneling", "Routing", "Logs", "Settings", "About"};
    for (const auto &item : items)
        navigation_->addItem(item);
    navigation_->setCurrentRow(0);
    sideLayout->addWidget(navigation_, 1);

    auto *version = new QLabel("Version 0.1.0  •  Foundation");
    version->setObjectName("muted");
    sideLayout->addWidget(version);

    auto *content = new QWidget;
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(26, 22, 26, 14);
    contentLayout->setSpacing(14);

    auto *header = new QHBoxLayout;
    auto *heading = new QLabel("Dashboard");
    heading->setObjectName("pageHeading");
    header->addWidget(heading);
    header->addStretch();
    auto *mode = new QLabel("Mode: Not connected");
    mode->setObjectName("muted");
    header->addWidget(mode);
    contentLayout->addLayout(header);

    pages_ = new QStackedWidget;
    pages_->addWidget(new DashboardPage);
    pages_->addWidget(new ServersPage);
    pages_->addWidget(new SubscriptionsPage);
    pages_->addWidget(new TunnelingPage);
    pages_->addWidget(new RoutingPage);
    pages_->addWidget(new LogsPage);
    pages_->addWidget(new SettingsPage);
    pages_->addWidget(new AboutPage);
    contentLayout->addWidget(pages_, 1);

    auto *footer = new QFrame;
    footer->setObjectName("footer");
    auto *footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(12, 8, 12, 8);
    statusLabel_ = new QLabel("● Disconnected");
    statusLabel_->setObjectName("status");
    connectButton_ = new QPushButton("Connect");
    connectButton_->setObjectName("primaryButton");
    connectButton_->setEnabled(false);
    connectButton_->setToolTip("VPN engine integration is not part of the UI foundation yet.");
    footerLayout->addWidget(statusLabel_);
    footerLayout->addStretch();
    footerLayout->addWidget(new QLabel("Engine not configured"));
    footerLayout->addWidget(connectButton_);
    contentLayout->addWidget(footer);

    root->addWidget(sidebar);
    root->addWidget(content, 1);
    setCentralWidget(central);

    connect(navigation_, &QListWidget::currentRowChanged, this, [this, heading](int row) {
        navigate(row);
        heading->setText(navigation_->item(row)->text());
    });

    applyTheme();
}

void MainWindow::navigate(int index)
{
    if (index >= 0 && index < pages_->count())
        pages_->setCurrentIndex(index);
}

void MainWindow::applyTheme()
{
    setStyleSheet(R"(
        QWidget { background: #111827; color: #e5e7eb; font-family: "Segoe UI"; font-size: 10pt; }
        #sidebar { background: #0b1220; border-right: 1px solid #263244; }
        #brand { color: #f8fafc; font-size: 19pt; font-weight: 700; padding: 8px 4px 22px 4px; }
        #navigation { background: transparent; border: 0; outline: 0; }
        #navigation::item { padding: 12px 10px; border-radius: 6px; color: #aab6c7; }
        #navigation::item:selected { background: #1d3554; color: #ffffff; }
        #navigation::item:hover:!selected { background: #172235; }
        #pageHeading { font-size: 19pt; font-weight: 600; color: #f8fafc; }
        #muted { color: #94a3b8; }
        #footer { background: #0b1220; border: 1px solid #263244; border-radius: 8px; }
        #status { color: #fbbf24; font-weight: 600; }
        #primaryButton { background: #2563eb; border: 0; border-radius: 6px; padding: 9px 22px; font-weight: 600; }
        #primaryButton:disabled { background: #334155; color: #94a3b8; }
        QPushButton { background: #243247; border: 1px solid #34445c; border-radius: 5px; padding: 7px 12px; }
        QPushButton:hover { background: #30435e; }
        QLineEdit, QComboBox, QPlainTextEdit { background: #0b1220; border: 1px solid #334155; border-radius: 5px; padding: 7px; }
        QTableWidget { background: #0b1220; border: 1px solid #263244; gridline-color: #263244; }
        QHeaderView::section { background: #172235; padding: 7px; border: 0; }
        QGroupBox { border: 1px solid #263244; border-radius: 7px; margin-top: 12px; padding: 12px; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; color: #93c5fd; }
    )");
}
