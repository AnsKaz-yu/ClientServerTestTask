#pragma once

// ============================================================
// client_handler.h
// Manages a single TCP client connection in the server thread.
// Lives in the ServerWorker's QThread.
// ============================================================

#include "client_config.h"
#include "data_models.h"
#include "protocol.h"

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDateTime>
#include <QHash>
#include <QString>

/// Emitted data record — forwarded to the GUI thread via signal.
struct DataRecord {
    QString   client_id;
    QString   data_type;     // "NetworkMetrics" | "DeviceStatus" | "Log"
    QString   content;       // human-readable summary
    QDateTime timestamp;

    // Structured fields (only one set will be populated)
    dm::NetworkMetrics network_metrics;
    dm::DeviceStatus   device_status;
    dm::LogEntry       log_entry;
    bool               is_network_metrics{false};
    bool               is_device_status{false};
    bool               is_log{false};
};

class ClientHandler : public QObject {
    Q_OBJECT
public:
    explicit ClientHandler(QTcpSocket* socket,
                           const QString& client_id,
                           QObject* parent = nullptr);

    ~ClientHandler() override;

    const QString&      clientId()     const { return client_id_; }
    QString             peerAddress()  const;
    quint16             peerPort()     const;
    bool                isConnected()  const;

    /// Send a start/stop command JSON to the client.
    void sendCommand(const QString& command);

    /// Send the acknowledgement JSON right after connection.
    void sendConnectAck();

    const ClientConfig& config() const { return config_; }
    void setConfig(const ClientConfig& cfg) { config_ = cfg; }

signals:
    /// Emitted when a well-formed data packet has been received.
    void dataReceived(const DataRecord& record);

    /// Emitted when the underlying socket disconnects.
    void disconnected(const QString& client_id);

    /// Emitted when a threshold is breached and a warning log should be shown.
    void thresholdBreached(const QString& client_id, const QString& message);

private slots:
    void onReadyRead();
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    /// Parse a complete JSON message from the read-buffer.
    void processMessage(const QByteArray& raw);

    /// Validate & parse NetworkMetrics; returns true on success.
    bool parseNetworkMetrics(const QJsonObject& obj, DataRecord& record);

    /// Validate & parse DeviceStatus; returns true on success.
    bool parseDeviceStatus(const QJsonObject& obj, DataRecord& record);

    /// Validate & parse Log; returns true on success.
    bool parseLog(const QJsonObject& obj, DataRecord& record);

    /// Check metric values against the stored thresholds.
    void checkNetworkThresholds(const dm::NetworkMetrics& m);
    void checkDeviceThresholds(const dm::DeviceStatus& s);

    /// Emit thresholdBreached only if the same key hasn't fired within kWarnCooldownSec.
    void maybeWarn(const QString& key, const QString& message);

    QTcpSocket* socket_;
    QString     client_id_;
    QByteArray  read_buffer_;   ///< accumulates partial TCP frames
    ClientConfig config_;

    /// Tracks when each metric key last emitted a warning.
    QHash<QString, QDateTime> warn_cooldowns_;

    /// Minimum interval between warnings for the same metric.
    static constexpr int kWarnCooldownSec = 5;
};
