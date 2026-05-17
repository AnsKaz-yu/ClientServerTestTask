#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>

// ============================================================
// data_models.h
// Shared POD structures representing the three data packet
// types exchanged between client and server.
// ============================================================

namespace dm {

/// Threshold configuration for a single client.
/// The server lets the user edit these via the settings dialog.
struct ClientConfig {
    double  max_bandwidth{1000.0};   ///< Mbps — alert if exceeded
    double  max_latency{200.0};      ///< ms  — alert if exceeded
    double  max_packet_loss{5.0};    ///< %   — alert if exceeded
    int     max_cpu_usage{90};       ///< %   — alert if exceeded
    int     max_memory_usage{90};    ///< %   — alert if exceeded
};

/// Parsed NetworkMetrics packet.
struct NetworkMetrics {
    double bandwidth{0.0};
    double latency{0.0};
    double packet_loss{0.0};
};

/// Parsed DeviceStatus packet.
struct DeviceStatus {
    int uptime{0};
    int cpu_usage{0};
    int memory_usage{0};
};

/// Parsed Log packet.
struct LogEntry {
    QString message;
    QString severity;   // "INFO" | "WARNING" | "ERROR"
};

} // namespace dm
