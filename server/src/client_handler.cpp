// ============================================================
// client_handler.cpp
// ============================================================

#include "client_handler.h"
#include "protocol.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QHostAddress>

using namespace protocol;

ClientHandler::ClientHandler(QTcpSocket* socket,
                             const QString& client_id,
                             QObject* parent)
    : QObject(parent)
    , socket_(socket)
    , client_id_(client_id)
    , config_(client_id)
{
    // Передаём владение — socket удалится вместе с handler
    socket_->setParent(this);

    connect(socket_, &QTcpSocket::readyRead,
            this,    &ClientHandler::onReadyRead);
    connect(socket_, &QTcpSocket::disconnected,
            this,    &ClientHandler::onDisconnected);
    connect(socket_, &QAbstractSocket::errorOccurred,
            this,    &ClientHandler::onSocketError);
}

ClientHandler::~ClientHandler() = default;

// ---- public helpers ------------------------------------------------

QString ClientHandler::peerAddress() const {
    return socket_ ? socket_->peerAddress().toString() : QString();
}

quint16 ClientHandler::peerPort() const {
    return socket_ ? socket_->peerPort() : 0;
}

bool ClientHandler::isConnected() const {
    return socket_ && socket_->state() == QAbstractSocket::ConnectedState;
}

void ClientHandler::sendConnectAck() {
    QJsonObject ack;
    ack["type"]      = "ConnectAck";
    ack["client_id"] = client_id_;
    ack["message"]   = "Connection accepted";
    ack["status"]    = "ok";

    if (socket_ && socket_->isOpen()) {
        socket_->write(buildFrame(ack));
    }
}

void ClientHandler::sendCommand(const QString& command) {
    QJsonObject msg;
    msg["type"]    = "Command";
    msg["command"] = command;

    if (socket_ && socket_->isOpen()) {
        socket_->write(buildFrame(msg));
    }
}

// ---- private slots -------------------------------------------------

void ClientHandler::onReadyRead() {
    read_buffer_.append(socket_->readAll());

    // Обрабатываем все полные фреймы из буфера
    while (read_buffer_.size() >= protocol::kHeaderSize) {
        // Читаем 4-байтовый заголовок длины (big-endian)
        const quint32 msg_len =
            (static_cast<quint8>(read_buffer_[0]) << 24) |
            (static_cast<quint8>(read_buffer_[1]) << 16) |
            (static_cast<quint8>(read_buffer_[2]) <<  8) |
             static_cast<quint8>(read_buffer_[3]);

        // Защита от аномально больших сообщений (>1 МБ)
        if (msg_len > protocol::kMaxMsgSize) {
            read_buffer_.clear();
            return;
        }

        const int total_needed = protocol::kHeaderSize + static_cast<int>(msg_len);
        if (read_buffer_.size() < total_needed) {
            break; // Ждём следующих данных
        }

        const QByteArray raw = read_buffer_.mid(protocol::kHeaderSize, static_cast<int>(msg_len));
        read_buffer_.remove(0, total_needed);

        processMessage(raw);
    }
}

void ClientHandler::onDisconnected() {
    emit disconnected(client_id_);
}

void ClientHandler::onSocketError(QAbstractSocket::SocketError /*error*/) {
    // Qt также пошлёт signal disconnected — здесь просто
    // пробрасываем текст ошибки в лог сервера через channel thresholdBreached
    if (socket_) {
        emit thresholdBreached(client_id_,
            QStringLiteral("Socket error: ") + socket_->errorString());
    }
}

// ---- private helpers -----------------------------------------------

void ClientHandler::processMessage(const QByteArray& raw) {
    QJsonParseError parse_error;
    const QJsonDocument doc = QJsonDocument::fromJson(raw, &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
        emit thresholdBreached(client_id_,
            QStringLiteral("Malformed JSON: ") + parse_error.errorString());
        return;
    }

    if (!doc.isObject()) {
        emit thresholdBreached(client_id_, "JSON root is not an object");
        return;
    }

    const QJsonObject obj = doc.object();
    const QString type    = obj["type"].toString();

    DataRecord record;
    record.client_id  = client_id_;
    record.data_type  = type;
    record.timestamp  = QDateTime::currentDateTime();

    bool ok = false;
    if (type == "NetworkMetrics") {
        ok = parseNetworkMetrics(obj, record);
    } else if (type == "DeviceStatus") {
        ok = parseDeviceStatus(obj, record);
    } else if (type == "Log") {
        ok = parseLog(obj, record);
    } else {
        // Неизвестный тип пакета — логируем, не падаем
        emit thresholdBreached(client_id_,
            QStringLiteral("Unknown packet type: ") + type);
        return;
    }

    if (ok) {
        emit dataReceived(record);
    }
}

bool ClientHandler::parseNetworkMetrics(const QJsonObject& obj, DataRecord& record) {
    // Проверяем наличие обязательных полей
    if (!obj.contains("bandwidth") ||
        !obj.contains("latency")   ||
        !obj.contains("packet_loss")) {
        emit thresholdBreached(client_id_, "NetworkMetrics: missing required fields");
        return false;
    }

    dm::NetworkMetrics m;
    m.bandwidth    = obj["bandwidth"].toDouble();
    m.latency      = obj["latency"].toDouble();
    m.packet_loss  = obj["packet_loss"].toDouble();

    record.network_metrics    = m;
    record.is_network_metrics = true;
    record.content = QString("bw=%1 Mbps  lat=%2 ms  loss=%3 %")
                         .arg(m.bandwidth, 0, 'f', 2)
                         .arg(m.latency,   0, 'f', 2)
                         .arg(m.packet_loss, 0, 'f', 3);

    checkNetworkThresholds(m);
    return true;
}

bool ClientHandler::parseDeviceStatus(const QJsonObject& obj, DataRecord& record) {
    if (!obj.contains("uptime")       ||
        !obj.contains("cpu_usage")    ||
        !obj.contains("memory_usage")) {
        emit thresholdBreached(client_id_, "DeviceStatus: missing required fields");
        return false;
    }

    dm::DeviceStatus s;
    s.uptime       = obj["uptime"].toInt();
    s.cpu_usage    = obj["cpu_usage"].toInt();
    s.memory_usage = obj["memory_usage"].toInt();

    record.device_status    = s;
    record.is_device_status = true;
    record.content = QString("uptime=%1 s  cpu=%2 %  mem=%3 %")
                         .arg(s.uptime)
                         .arg(s.cpu_usage)
                         .arg(s.memory_usage);

    checkDeviceThresholds(s);
    return true;
}

bool ClientHandler::parseLog(const QJsonObject& obj, DataRecord& record) {
    if (!obj.contains("message") || !obj.contains("severity")) {
        emit thresholdBreached(client_id_, "Log: missing required fields");
        return false;
    }

    dm::LogEntry e;
    e.message  = obj["message"].toString();
    e.severity = obj["severity"].toString();

    record.log_entry = e;
    record.is_log    = true;
    record.content   = QStringLiteral("[") + e.severity + QStringLiteral("] ") + e.message;
    return true;
}

void ClientHandler::checkNetworkThresholds(const dm::NetworkMetrics& m) {
    const auto& t = config_.thresholds();

    if (m.bandwidth > t.max_bandwidth) {
        maybeWarn("bandwidth",
            QString("WARN: bandwidth %1 Mbps exceeds threshold %2 Mbps")
                .arg(m.bandwidth).arg(t.max_bandwidth));
    }
    if (m.latency > t.max_latency) {
        maybeWarn("latency",
            QString("WARN: latency %1 ms exceeds threshold %2 ms")
                .arg(m.latency).arg(t.max_latency));
    }
    if (m.packet_loss > t.max_packet_loss) {
        maybeWarn("packet_loss",
            QString("WARN: packet_loss %1 % exceeds threshold %2 %")
                .arg(m.packet_loss).arg(t.max_packet_loss));
    }
}

void ClientHandler::checkDeviceThresholds(const dm::DeviceStatus& s) {
    const auto& t = config_.thresholds();

    if (s.cpu_usage > t.max_cpu_usage) {
        maybeWarn("cpu_usage",
            QString("WARN: cpu_usage %1 % exceeds threshold %2 %")
                .arg(s.cpu_usage).arg(t.max_cpu_usage));
    }
    if (s.memory_usage > t.max_memory_usage) {
        maybeWarn("memory_usage",
            QString("WARN: memory_usage %1 % exceeds threshold %2 %")
                .arg(s.memory_usage).arg(t.max_memory_usage));
    }
}

void ClientHandler::maybeWarn(const QString& key, const QString& message) {
    const QDateTime now = QDateTime::currentDateTime();
    const auto it = warn_cooldowns_.constFind(key);
    if (it != warn_cooldowns_.constEnd() &&
        it->secsTo(now) < kWarnCooldownSec) {
        return;  // Ещё в cooldown — подавляем дублирующее предупреждение
    }
    warn_cooldowns_.insert(key, now);
    emit thresholdBreached(client_id_, message);
}
