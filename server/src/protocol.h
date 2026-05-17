#pragma once

// ============================================================
// protocol.h
// Shared framing helpers for the 4-byte length-prefix protocol.
//
// Wire format per message:
//   [ uint32 big-endian length ][ JSON bytes ]
// ============================================================

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

/// Protocol constants.
namespace protocol {

inline constexpr int kHeaderSize   = 4;
inline constexpr quint32 kMaxMsgSize = 1024u * 1024u;  ///< 1 MB sanity cap

/// Encode a JSON object into a length-prefixed frame ready to write to a socket.
inline QByteArray buildFrame(const QJsonObject& obj) {
    const QByteArray payload = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    const quint32 len = static_cast<quint32>(payload.size());

    QByteArray frame;
    frame.reserve(kHeaderSize + static_cast<int>(len));
    frame.resize(kHeaderSize);
    frame[0] = static_cast<char>((len >> 24) & 0xFF);
    frame[1] = static_cast<char>((len >> 16) & 0xFF);
    frame[2] = static_cast<char>((len >>  8) & 0xFF);
    frame[3] = static_cast<char>( len        & 0xFF);
    frame.append(payload);
    return frame;
}

} // namespace protocol
