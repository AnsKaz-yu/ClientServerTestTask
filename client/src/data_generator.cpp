// ============================================================
// data_generator.cpp
// ============================================================

#include "data_generator.h"

#include <QJsonArray>
#include <QRandomGenerator>
#include <QStringList>

// ---------------------------------------------------------------------------
// Helper: random double in [min, max]
// ---------------------------------------------------------------------------
static double randDouble(double min_val, double max_val) {
    const double t = QRandomGenerator::global()->generateDouble();
    return min_val + t * (max_val - min_val);
}

// ---------------------------------------------------------------------------
// Helper: random int in [min, max]
// ---------------------------------------------------------------------------
static int randInt(int min_val, int max_val) {
    return QRandomGenerator::global()->bounded(min_val, max_val + 1);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

QJsonObject DataGenerator::generateRandom() {
    const int choice = randInt(0, 2);
    switch (choice) {
        case 0:  return generateNetworkMetrics();
        case 1:  return generateDeviceStatus();
        default: return generateLog();
    }
}

QJsonObject DataGenerator::generateNetworkMetrics() {
    QJsonObject obj;
    obj["type"]        = "NetworkMetrics";
    obj["bandwidth"]   = randDouble(0.5, 1200.0);   // Mbps
    obj["latency"]     = randDouble(0.5, 500.0);    // ms
    obj["packet_loss"] = randDouble(0.0, 15.0);     // %

    // Optional extended fields for richer content variety.
    obj["jitter"]      = randDouble(0.0, 50.0);     // ms
    obj["throughput"]  = randDouble(0.1, 900.0);    // Mbps

    return obj;
}

QJsonObject DataGenerator::generateDeviceStatus() {
    QJsonObject obj;
    obj["type"]         = "DeviceStatus";
    obj["uptime"]       = randInt(0, 86400 * 30);   // seconds
    obj["cpu_usage"]    = randInt(0, 100);           // %
    obj["memory_usage"] = randInt(10, 100);          // %

    // Optional extended fields.
    obj["temperature"]  = randDouble(30.0, 95.0);   // °C
    obj["disk_usage"]   = randInt(5, 99);            // %
    obj["process_count"]= randInt(1, 512);

    return obj;
}

QJsonObject DataGenerator::generateLog(const QString& severity_override) {
    static const QStringList severities = {"INFO", "INFO", "INFO", "WARNING", "ERROR"};
    static const QStringList short_msgs = {
        "System heartbeat OK",
        "Interface eth0 up",
        "DNS resolved successfully",
        "Ping to gateway OK",
        "NTP sync complete"
    };
    static const QStringList medium_msgs = {
        "Interface eth0 restarted after link-down event detected by watchdog",
        "Firmware version check passed: current=2.4.1, latest=2.4.1",
        "DHCP lease renewed successfully for interface eth1, lease time 3600s",
        "TLS handshake completed with remote endpoint 10.0.0.1:443",
        "Scheduled maintenance window started — suppressing non-critical alerts"
    };
    static const QStringList long_msgs = {
        "Critical error: memory allocation failed in subsystem 'network-stack' "
        "while processing incoming packet from 192.168.1.100. Buffer pool exhausted. "
        "Dropping packet. Consider increasing pool size in /etc/netstack.conf.",

        "WARNING: Packet loss threshold exceeded on interface eth0. "
        "Observed loss rate: 8.7% over the last 60-second window. "
        "Possible causes: cable degradation, duplex mismatch, or upstream congestion. "
        "Initiating automatic link renegotiation procedure.",

        "INFO: Completed scheduled full backup of configuration database. "
        "Backup size: 14 MB. Checksum (SHA-256): "
        "a3f5c812e4b09d7c1f2e3a4b5c6d7e8f9a0b1c2d3e4f5a6b7c8d9e0f1a2b3c4. "
        "Stored at /var/backups/config-2024-01-15T03:00:00Z.tar.gz."
    };

    const QString severity = severity_override.isEmpty()
        ? severities[randInt(0, severities.size() - 1)]
        : severity_override;

    // Vary message length: short, medium, long
    QString message;
    const int length_class = randInt(0, 2);
    if (length_class == 0) {
        message = short_msgs[randInt(0, short_msgs.size() - 1)];
    } else if (length_class == 1) {
        message = medium_msgs[randInt(0, medium_msgs.size() - 1)];
    } else {
        message = long_msgs[randInt(0, long_msgs.size() - 1)];
    }

    QJsonObject obj;
    obj["type"]     = "Log";
    obj["message"]  = message;
    obj["severity"] = severity;
    obj["source"]   = randomString(4, 12);   // simulated subsystem name

    return obj;
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

QString DataGenerator::randomString(int min_len, int max_len) {
    static const QString charset =
        "abcdefghijklmnopqrstuvwxyz0123456789";
    const int len = randInt(min_len, max_len);
    QString result;
    result.reserve(len);
    for (int i = 0; i < len; ++i) {
        result.append(charset[randInt(0, charset.size() - 1)]);
    }
    return result;
}
