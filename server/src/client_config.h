#pragma once

// ============================================================
// client_config.h
// Per-client threshold settings, serialisable to/from JSON.
// ============================================================

#include "data_models.h"
#include <QJsonObject>
#include <QString>

class ClientConfig {
public:
    explicit ClientConfig(const QString& client_id = QString())
        : client_id_(client_id) {}

    // ----- accessors -----
    const QString& clientId()    const { return client_id_; }
    const dm::ClientConfig& thresholds() const { return thresholds_; }
    dm::ClientConfig&       thresholds()       { return thresholds_; }

    // ----- serialisation -----
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["client_id"]       = client_id_;
        obj["max_bandwidth"]   = thresholds_.max_bandwidth;
        obj["max_latency"]     = thresholds_.max_latency;
        obj["max_packet_loss"] = thresholds_.max_packet_loss;
        obj["max_cpu_usage"]   = thresholds_.max_cpu_usage;
        obj["max_memory_usage"]= thresholds_.max_memory_usage;
        return obj;
    }

    static ClientConfig fromJson(const QJsonObject& obj) {
        ClientConfig cfg(obj["client_id"].toString());
        cfg.thresholds_.max_bandwidth    = obj["max_bandwidth"].toDouble(1000.0);
        cfg.thresholds_.max_latency      = obj["max_latency"].toDouble(200.0);
        cfg.thresholds_.max_packet_loss  = obj["max_packet_loss"].toDouble(5.0);
        cfg.thresholds_.max_cpu_usage    = obj["max_cpu_usage"].toInt(90);
        cfg.thresholds_.max_memory_usage = obj["max_memory_usage"].toInt(90);
        return cfg;
    }

private:
    QString           client_id_;
    dm::ClientConfig  thresholds_;
};
