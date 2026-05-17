/****************************************************************************
** Meta object code from reading C++ file 'server_worker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../server/src/server_worker.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server_worker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSServerWorkerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSServerWorkerENDCLASS = QtMocHelpers::stringData(
    "ServerWorker",
    "clientConnected",
    "",
    "client_id",
    "ip",
    "port",
    "clientDisconnected",
    "dataReceived",
    "DataRecord",
    "record",
    "logMessage",
    "message",
    "serverStarted",
    "serverStopped",
    "start",
    "stop",
    "startAllClients",
    "stopAllClients",
    "updateClientConfig",
    "ClientConfig",
    "config",
    "onNewConnection",
    "onClientData",
    "onClientDisconnected",
    "onThresholdBreached"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSServerWorkerENDCLASS_t {
    uint offsetsAndSizes[50];
    char stringdata0[13];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[3];
    char stringdata5[5];
    char stringdata6[19];
    char stringdata7[13];
    char stringdata8[11];
    char stringdata9[7];
    char stringdata10[11];
    char stringdata11[8];
    char stringdata12[14];
    char stringdata13[14];
    char stringdata14[6];
    char stringdata15[5];
    char stringdata16[16];
    char stringdata17[15];
    char stringdata18[19];
    char stringdata19[13];
    char stringdata20[7];
    char stringdata21[16];
    char stringdata22[13];
    char stringdata23[21];
    char stringdata24[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSServerWorkerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSServerWorkerENDCLASS_t qt_meta_stringdata_CLASSServerWorkerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 12),  // "ServerWorker"
        QT_MOC_LITERAL(13, 15),  // "clientConnected"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 9),  // "client_id"
        QT_MOC_LITERAL(40, 2),  // "ip"
        QT_MOC_LITERAL(43, 4),  // "port"
        QT_MOC_LITERAL(48, 18),  // "clientDisconnected"
        QT_MOC_LITERAL(67, 12),  // "dataReceived"
        QT_MOC_LITERAL(80, 10),  // "DataRecord"
        QT_MOC_LITERAL(91, 6),  // "record"
        QT_MOC_LITERAL(98, 10),  // "logMessage"
        QT_MOC_LITERAL(109, 7),  // "message"
        QT_MOC_LITERAL(117, 13),  // "serverStarted"
        QT_MOC_LITERAL(131, 13),  // "serverStopped"
        QT_MOC_LITERAL(145, 5),  // "start"
        QT_MOC_LITERAL(151, 4),  // "stop"
        QT_MOC_LITERAL(156, 15),  // "startAllClients"
        QT_MOC_LITERAL(172, 14),  // "stopAllClients"
        QT_MOC_LITERAL(187, 18),  // "updateClientConfig"
        QT_MOC_LITERAL(206, 12),  // "ClientConfig"
        QT_MOC_LITERAL(219, 6),  // "config"
        QT_MOC_LITERAL(226, 15),  // "onNewConnection"
        QT_MOC_LITERAL(242, 12),  // "onClientData"
        QT_MOC_LITERAL(255, 20),  // "onClientDisconnected"
        QT_MOC_LITERAL(276, 19)   // "onThresholdBreached"
    },
    "ServerWorker",
    "clientConnected",
    "",
    "client_id",
    "ip",
    "port",
    "clientDisconnected",
    "dataReceived",
    "DataRecord",
    "record",
    "logMessage",
    "message",
    "serverStarted",
    "serverStopped",
    "start",
    "stop",
    "startAllClients",
    "stopAllClients",
    "updateClientConfig",
    "ClientConfig",
    "config",
    "onNewConnection",
    "onClientData",
    "onClientDisconnected",
    "onThresholdBreached"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSServerWorkerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,  110,    2, 0x06,    1 /* Public */,
       6,    1,  117,    2, 0x06,    5 /* Public */,
       7,    1,  120,    2, 0x06,    7 /* Public */,
      10,    1,  123,    2, 0x06,    9 /* Public */,
      12,    1,  126,    2, 0x06,   11 /* Public */,
      13,    0,  129,    2, 0x06,   13 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    1,  130,    2, 0x0a,   14 /* Public */,
      14,    0,  133,    2, 0x2a,   16 /* Public | MethodCloned */,
      15,    0,  134,    2, 0x0a,   17 /* Public */,
      16,    0,  135,    2, 0x0a,   18 /* Public */,
      17,    0,  136,    2, 0x0a,   19 /* Public */,
      18,    1,  137,    2, 0x0a,   20 /* Public */,
      21,    0,  140,    2, 0x08,   22 /* Private */,
      22,    1,  141,    2, 0x08,   23 /* Private */,
      23,    1,  144,    2, 0x08,   25 /* Private */,
      24,    2,  147,    2, 0x08,   27 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UShort,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::UShort,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::UShort,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   11,

       0        // eod
};

Q_CONSTINIT const QMetaObject ServerWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSServerWorkerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSServerWorkerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSServerWorkerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ServerWorker, std::true_type>,
        // method 'clientConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'clientDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'dataReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DataRecord &, std::false_type>,
        // method 'logMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'serverStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'serverStopped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'start'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'start'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startAllClients'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopAllClients'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateClientConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const ClientConfig &, std::false_type>,
        // method 'onNewConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClientData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DataRecord &, std::false_type>,
        // method 'onClientDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onThresholdBreached'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void ServerWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ServerWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clientConnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[3]))); break;
        case 1: _t->clientDisconnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->dataReceived((*reinterpret_cast< std::add_pointer_t<DataRecord>>(_a[1]))); break;
        case 3: _t->logMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->serverStarted((*reinterpret_cast< std::add_pointer_t<quint16>>(_a[1]))); break;
        case 5: _t->serverStopped(); break;
        case 6: _t->start((*reinterpret_cast< std::add_pointer_t<quint16>>(_a[1]))); break;
        case 7: _t->start(); break;
        case 8: _t->stop(); break;
        case 9: _t->startAllClients(); break;
        case 10: _t->stopAllClients(); break;
        case 11: _t->updateClientConfig((*reinterpret_cast< std::add_pointer_t<ClientConfig>>(_a[1]))); break;
        case 12: _t->onNewConnection(); break;
        case 13: _t->onClientData((*reinterpret_cast< std::add_pointer_t<DataRecord>>(_a[1]))); break;
        case 14: _t->onClientDisconnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->onThresholdBreached((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ServerWorker::*)(const QString & , const QString & , quint16 );
            if (_t _q_method = &ServerWorker::clientConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ServerWorker::*)(const QString & );
            if (_t _q_method = &ServerWorker::clientDisconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ServerWorker::*)(const DataRecord & );
            if (_t _q_method = &ServerWorker::dataReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ServerWorker::*)(const QString & );
            if (_t _q_method = &ServerWorker::logMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ServerWorker::*)(quint16 );
            if (_t _q_method = &ServerWorker::serverStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ServerWorker::*)();
            if (_t _q_method = &ServerWorker::serverStopped; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *ServerWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServerWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSServerWorkerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ServerWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void ServerWorker::clientConnected(const QString & _t1, const QString & _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ServerWorker::clientDisconnected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ServerWorker::dataReceived(const DataRecord & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ServerWorker::logMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ServerWorker::serverStarted(quint16 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ServerWorker::serverStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
