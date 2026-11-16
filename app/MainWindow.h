#pragma once
#include <QMainWindow>

class QListWidget;
class QStackedWidget;
class QLabel;
class QPushButton;

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void navigate(int index);
    void applyTheme();

    QListWidget *navigation_ = nullptr;
    QStackedWidget *pages_ = nullptr;
    QLabel *statusLabel_ = nullptr;
    QPushButton *connectButton_ = nullptr;
    bool connected_ = false;
};
