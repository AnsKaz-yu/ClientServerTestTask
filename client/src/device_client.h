#pragma once

// ============================================================
// device_client.h
// Console-mode client that connects to the server, waits for
// ConnectAck + "start" command, then streams random JSON data.
// ============================================================

#include "data_generator.h"
#include "protocol.h"

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QString>

class DeviceClient : public QObject {
    Q_OBJECT
public:
    explicit DeviceClient(const QString& host,
                          quint16        port,
                          QObject*       parent = nullptr);

signals:
    /// Emitted when the application should exit (e.g. fatal error).
    void finished();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void onReconnectTimer();
    void onSendTimer();

private:
    // ---- helpers ----
    void connectToServer();
    void sendJson(const QJsonObject& obj);
    void processMessage(const QByteArray& raw);
    void startSending();
    void stopSending();
    void scheduleNextSend();

    // ---- state ----
    enum class State {
        Disconnected,    ///< Not connected, retrying.
        WaitingAck,      ///< Connected, waiting for ConnectAck.
        WaitingStart,    ///< Ack received, waiting for "start" command.
        Sending,         ///< Actively sending data.
        Stopped          ///< Received "stop" command.
    };

    QString        host_;
    quint16        port_;
    QTcpSocket*    socket_{nullptr};
    QTimer*        reconnect_timer_{nullptr};
    QTimer*        send_timer_{nullptr};
    QByteArray     read_buffer_;
    DataGenerator  generator_;
    State          state_{State::Disconnected};

    static constexpr int kReconnectInterval = 5000;  ///< ms
    static constexpr int kMinSendDelay      = 10;    ///< ms
    static constexpr int kMaxSendDelay      = 100;   ///< ms
};
