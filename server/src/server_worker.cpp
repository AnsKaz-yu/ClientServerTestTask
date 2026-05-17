// ============================================================
// server_worker.cpp
// ============================================================

#include "server_worker.h"

#include <QTcpSocket>
#include <QHostAddress>

ServerWorker::ServerWorker(QObject* parent)
    : QObject(parent)
    , tcp_server_(new QTcpServer(this))
{
    connect(tcp_server_, &QTcpServer::newConnection,
            this,         &ServerWorker::onNewConnection);
}

ServerWorker::~ServerWorker() {
    stop();
}

bool ServerWorker::isListening() const {
    return tcp_server_ && tcp_server_->isListening();
}

void ServerWorker::start(quint16 port) {
    if (tcp_server_->isListening()) {
        return;
    }

    if (!tcp_server_->listen(QHostAddress::Any, port)) {
        emit logMessage(QStringLiteral("Failed to start server: ") +
                        tcp_server_->errorString());
        return;
    }

    emit logMessage(QString("Server listening on port %1").arg(port));
    emit serverStarted(port);
}

void ServerWorker::stop() {
    if (!tcp_server_->isListening()) {
        return;
    }

    // Disconnect all clients gracefully.
    for (auto* handler : handlers_) {
        handler->deleteLater();
    }
    handlers_.clear();

    tcp_server_->close();
    emit logMessage("Server stopped");
    emit serverStopped();
}

void ServerWorker::startAllClients() {
    for (auto* handler : handlers_) {
        if (handler->isConnected()) {
            handler->sendCommand("start");
        }
    }
    emit logMessage("Sent 'start' command to all clients");
}

void ServerWorker::stopAllClients() {
    for (auto* handler : handlers_) {
        if (handler->isConnected()) {
            handler->sendCommand("stop");
        }
    }
    emit logMessage("Sent 'stop' command to all clients");
}

void ServerWorker::updateClientConfig(const ClientConfig& config) {
    auto it = handlers_.find(config.clientId());
    if (it != handlers_.end()) {
        it.value()->setConfig(config);
        emit logMessage(QString("Updated config for client %1").arg(config.clientId()));
    }
}

// ---- private slots -------------------------------------------------

void ServerWorker::onNewConnection() {
    while (tcp_server_->hasPendingConnections()) {
        QTcpSocket* socket = tcp_server_->nextPendingConnection();

        const QString client_id = generateClientId();

        auto* handler = new ClientHandler(socket, client_id, this);

        connect(handler, &ClientHandler::dataReceived,
                this,    &ServerWorker::onClientData);
        connect(handler, &ClientHandler::disconnected,
                this,    &ServerWorker::onClientDisconnected);
        connect(handler, &ClientHandler::thresholdBreached,
                this,    &ServerWorker::onThresholdBreached);

        handlers_.insert(client_id, handler);

        // Send acknowledgement immediately.
        handler->sendConnectAck();

        emit logMessage(QString("Client %1 connected from %2:%3")
                            .arg(client_id)
                            .arg(handler->peerAddress())
                            .arg(handler->peerPort()));

        emit clientConnected(client_id,
                             handler->peerAddress(),
                             handler->peerPort());
    }
}

void ServerWorker::onClientData(const DataRecord& record) {
    emit dataReceived(record);
}

void ServerWorker::onClientDisconnected(const QString& client_id) {
    if (handlers_.contains(client_id)) {
        handlers_[client_id]->deleteLater();
        handlers_.remove(client_id);
    }

    emit logMessage(QString("Client %1 disconnected").arg(client_id));
    emit clientDisconnected(client_id);
}

void ServerWorker::onThresholdBreached(const QString& client_id,
                                        const QString& message) {
    emit logMessage(QString("[%1] %2").arg(client_id, message));
}

QString ServerWorker::generateClientId() const {
    // Incrementing numeric ID is simple and collision-free in a single process.
    // next_id_ is mutable to allow use from a const method.
    return QString("DEV-%1").arg(
        next_id_.fetchAndAddRelaxed(1),
        4, 10, QChar('0'));
}
