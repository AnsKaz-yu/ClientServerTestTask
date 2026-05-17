// ============================================================
// device_client.cpp
// ============================================================

#include "device_client.h"
#include "protocol.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTextStream>
#include <QDateTime>
#include <QThread>

// Convenience: print a timestamped line to stdout.
static void printLog(const QString& msg) {
    QTextStream out(stdout);
    out << QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ")
        << msg << Qt::endl;
}

DeviceClient::DeviceClient(const QString& host,
                            quint16        port,
                            QObject*       parent)
    : QObject(parent)
    , host_(host)
    , port_(port)
    , socket_(new QTcpSocket(this))
    , reconnect_timer_(new QTimer(this))
    , send_timer_(new QTimer(this))
{
    reconnect_timer_->setSingleShot(true);
    reconnect_timer_->setInterval(kReconnectInterval);

    send_timer_->setSingleShot(true);

    connect(socket_, &QTcpSocket::connected,
            this,    &DeviceClient::onConnected);
    connect(socket_, &QTcpSocket::disconnected,
            this,    &DeviceClient::onDisconnected);
    connect(socket_, &QTcpSocket::readyRead,
            this,    &DeviceClient::onReadyRead);
    connect(socket_, &QAbstractSocket::errorOccurred,
            this,    &DeviceClient::onSocketError);
    connect(reconnect_timer_, &QTimer::timeout,
            this,             &DeviceClient::onReconnectTimer);
    connect(send_timer_, &QTimer::timeout,
            this,        &DeviceClient::onSendTimer);

    connectToServer();
}

// ---- private helpers -----------------------------------------------

void DeviceClient::connectToServer() {
    printLog(QString("Connecting to %1:%2 ...").arg(host_).arg(port_));
    state_ = State::Disconnected;
    socket_->connectToHost(host_, port_);
}

void DeviceClient::sendJson(const QJsonObject& obj) {
    socket_->write(protocol::buildFrame(obj));
}

void DeviceClient::processMessage(const QByteArray& raw) {
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(raw, &err);

    if (err.error != QJsonParseError::NoError) {
        printLog(QString("Bad JSON from server: ") + err.errorString());
        return;
    }
    if (!doc.isObject()) {
        printLog("Server sent non-object JSON");
        return;
    }

    const QJsonObject obj = doc.object();
    const QString type    = obj["type"].toString();

    if (type == "ConnectAck") {
        const QString status    = obj["status"].toString();
        const QString client_id = obj["client_id"].toString();
        printLog(QString("ConnectAck received — client_id=%1, status=%2")
                .arg(client_id, status));
        state_ = State::WaitingStart;
        printLog("Waiting for 'start' command from server...");

    } else if (type == "Command") {
        const QString cmd = obj["command"].toString();
        if (cmd == "start") {
            printLog("Received 'start' command. Starting data transmission.");
            startSending();
        } else if (cmd == "stop") {
            printLog("Received 'stop' command. Pausing transmission.");
            stopSending();
        } else {
            printLog(QString("Unknown command: ") + cmd);
        }

    } else {
        printLog(QString("Unknown message type from server: ") + type);
    }
}

void DeviceClient::startSending() {
    state_ = State::Sending;
    scheduleNextSend();
}

void DeviceClient::stopSending() {
    state_ = State::Stopped;
    send_timer_->stop();
}

void DeviceClient::scheduleNextSend() {
    const int delay = QRandomGenerator::global()->bounded(
        kMinSendDelay, kMaxSendDelay + 1);
    send_timer_->start(delay);
}

// ---- private slots -------------------------------------------------

void DeviceClient::onConnected() {
    printLog(QString("Connected to server %1:%2").arg(host_).arg(port_));
    reconnect_timer_->stop();
    state_ = State::WaitingAck;
}

void DeviceClient::onDisconnected() {
    printLog("Disconnected from server. Retrying in 5 seconds...");
    send_timer_->stop();
    state_ = State::Disconnected;
    read_buffer_.clear();
    reconnect_timer_->start(kReconnectInterval);
}

void DeviceClient::onReadyRead() {
    read_buffer_.append(socket_->readAll());

    while (read_buffer_.size() >= protocol::kHeaderSize) {
        const quint32 msg_len =
            (static_cast<quint8>(read_buffer_[0]) << 24) |
            (static_cast<quint8>(read_buffer_[1]) << 16) |
            (static_cast<quint8>(read_buffer_[2]) <<  8) |
             static_cast<quint8>(read_buffer_[3]);

        if (msg_len > protocol::kMaxMsgSize) {
            printLog("Received oversized message, discarding buffer.");
            read_buffer_.clear();
            return;
        }

        const int total = protocol::kHeaderSize + static_cast<int>(msg_len);
        if (read_buffer_.size() < total) break;

        const QByteArray raw = read_buffer_.mid(protocol::kHeaderSize, static_cast<int>(msg_len));
        read_buffer_.remove(0, total);
        processMessage(raw);
    }
}

void DeviceClient::onSocketError(QAbstractSocket::SocketError /*error*/) {
    printLog(QString("Socket error: ") + socket_->errorString());
    if (state_ != State::Disconnected) {
        // onDisconnected will be emitted by Qt, which schedules reconnect.
    }
}

void DeviceClient::onReconnectTimer() {
    connectToServer();
}

void DeviceClient::onSendTimer() {
    if (state_ != State::Sending) return;
    if (!socket_->isValid() ||
        socket_->state() != QAbstractSocket::ConnectedState) return;

    const QJsonObject data = generator_.generateRandom();
    sendJson(data);

    // Log a brief summary to console.
    const QString type = data["type"].toString();
    printLog(QString("Sent %1").arg(type));

    scheduleNextSend();
}
