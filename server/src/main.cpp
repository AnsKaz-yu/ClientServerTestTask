// ============================================================
// server/src/main.cpp
// ============================================================

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("DeviceMonitorServer");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("DevMonitor");

    MainWindow window;
    window.show();

    return app.exec();
}
