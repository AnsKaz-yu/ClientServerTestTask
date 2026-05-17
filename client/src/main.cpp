// ============================================================
// client/src/main.cpp
// ============================================================

#include "device_client.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTextStream>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    app.setApplicationName("DeviceMonitorClient");
    app.setApplicationVersion("1.0.0");

    // ---- CLI argument parsing ----
    QCommandLineParser parser;
    parser.setApplicationDescription("Device Monitor Client — emulates a sensor device.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption host_opt(
        {"H", "host"},
        "Server hostname or IP address (default: localhost).",
        "host",
        "localhost");
    QCommandLineOption port_opt(
        {"p", "port"},
        "Server port (default: 12345).",
        "port",
        "12345");

    parser.addOption(host_opt);
    parser.addOption(port_opt);
    parser.process(app);

    const QString host = parser.value(host_opt);
    bool ok = false;
    const quint16 port = static_cast<quint16>(
        parser.value(port_opt).toUInt(&ok));

    if (!ok || port == 0) {
        QTextStream(stderr) << "Invalid port number.\n";
        return 1;
    }

    // DeviceClient is a QObject; its lifetime is managed by QCoreApplication.
    auto* client = new DeviceClient(host, port, &app);
    QObject::connect(client, &DeviceClient::finished, &app, &QCoreApplication::quit);

    return app.exec();
}
