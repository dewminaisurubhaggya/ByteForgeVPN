#pragma once
#include <QWidget>

class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardPage(QWidget *parent = nullptr);
};

class ServersPage : public QWidget
{
    Q_OBJECT
public:
    explicit ServersPage(QWidget *parent = nullptr);
};

class SubscriptionsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SubscriptionsPage(QWidget *parent = nullptr);
};

class TunnelingPage : public QWidget
{
    Q_OBJECT
public:
    explicit TunnelingPage(QWidget *parent = nullptr);
};

class RoutingPage : public QWidget
{
    Q_OBJECT
public:
    explicit RoutingPage(QWidget *parent = nullptr);
};

class LogsPage : public QWidget
{
    Q_OBJECT
public:
    explicit LogsPage(QWidget *parent = nullptr);
};

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);
};

class AboutPage : public QWidget
{
    Q_OBJECT
public:
    explicit AboutPage(QWidget *parent = nullptr);
};
