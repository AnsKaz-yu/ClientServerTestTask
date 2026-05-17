#pragma once

// ============================================================
// server_worker.h
// Runs in a dedicated QThread.
// Owns QTcpServer and all ClientHandler instances.
// ============================================================

#include "client_handler.h"
#include "client_config.h"

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QString>
#include <QAtomicInt>

class ServerWorker : public QObject {
    Q_OBJECT
public:
    explicit ServerWorker(QObject* parent = nullptr);
    ~ServerWorker() override;

    bool isListening() const;

public slots:
    /// Start the TCP server on the given port.
    void start(quint16 port = 12345);

    /// Stop the server and disconnect all clients.
    void stop();

    /// Send "start" command to all connected clients.
    void startAllClients();

    /// Send "stop" command to all connected clients.
    void stopAllClients();

    /// Update the threshold configuration for a specific client.
    void updateClientConfig(const ClientConfig& config);

signals:
    /// New client connected.
    void clientConnected(const QString& client_id,
                         const QString& ip,
                         quint16        port);

    /// Client disconnected.
    void clientDisconnected(const QString& client_id);

    /// A well-formed data record received from a client.
    void dataReceived(const DataRecord& record);

    /// A warning log message (threshold breach or error).
    void logMessage(const QString& message);

    /// Emitted once the server has started listening.
    void serverStarted(quint16 port);

    /// Emitted once the server has stopped.
    void serverStopped();

private slots:
    void onNewConnection();
    void onClientData(const DataRecord& record);
    void onClientDisconnected(const QString& client_id);
    void onThresholdBreached(const QString& client_id, const QString& message);

private:
    QString generateClientId() const;

    QTcpServer*                     tcp_server_{nullptr};
    QMap<QString, ClientHandler*>   handlers_;   ///< client_id -> handler
    mutable QAtomicInt              next_id_{1}; ///< mutable: incremented even in const context
};
