#pragma once

// ============================================================
// data_generator.h
// Generates random JSON payloads for the three data types.
// ============================================================

#include <QJsonObject>
#include <QString>

class DataGenerator {
public:
    DataGenerator() = default;

    /// Returns a random QJsonObject whose "type" is one of
    /// NetworkMetrics / DeviceStatus / Log.
    QJsonObject generateRandom();

    /// Returns a random NetworkMetrics object.
    QJsonObject generateNetworkMetrics();

    /// Returns a random DeviceStatus object.
    QJsonObject generateDeviceStatus();

    /// Returns a random Log object.
    /// @param severity_override  If non-empty, forces that severity level.
    QJsonObject generateLog(const QString& severity_override = QString());

private:
    /// Generate a random string of the given approximate length.
    QString randomString(int min_len, int max_len);
};
