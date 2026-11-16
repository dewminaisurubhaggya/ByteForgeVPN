#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"
#include "core/Database.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Byte Forge VPN");
    QApplication::setOrganizationName("ByteForge");
    QApplication::setApplicationVersion("0.1.0");

    Database database;
    QString error;
    if (!database.open(&error)) {
        QMessageBox::critical(nullptr, "Database error",
                              "Could not initialize local storage:\n" + error);
        return 1;
    }

    MainWindow window;
    window.show();
    return app.exec();
}
