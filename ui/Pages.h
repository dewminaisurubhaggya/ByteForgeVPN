#pragma once
#include <QWidget>

class DashboardPage : public QWidget { Q_OBJECT public: explicit DashboardPage(QWidget *p=nullptr); };
class ServersPage : public QWidget { Q_OBJECT public: explicit ServersPage(QWidget *p=nullptr); };
class SubscriptionsPage : public QWidget { Q_OBJECT public: explicit SubscriptionsPage(QWidget *p=nullptr); };
class TunnelingPage : public QWidget { Q_OBJECT public: explicit TunnelingPage(QWidget *p=nullptr); };
class RoutingPage : public QWidget { Q_OBJECT public: explicit RoutingPage(QWidget *p=nullptr); };
class LogsPage : public QWidget { Q_OBJECT public: explicit LogsPage(QWidget *p=nullptr); };
class SettingsPage : public QWidget { Q_OBJECT public: explicit SettingsPage(QWidget *p=nullptr); };
class AboutPage : public QWidget { Q_OBJECT public: explicit AboutPage(QWidget *p=nullptr); };
