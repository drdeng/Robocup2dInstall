/****************************************************************************
** Meta object code from reading C++ file 'monitor_client.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "monitor_client.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MonitorClient_t {
    QByteArrayData data[7];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorClient_t qt_meta_stringdata_MonitorClient = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MonitorClient"
QT_MOC_LITERAL(1, 14, 8), // "received"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 19), // "disconnectRequested"
QT_MOC_LITERAL(4, 44, 18), // "reconnectRequested"
QT_MOC_LITERAL(5, 63, 13), // "handleReceive"
QT_MOC_LITERAL(6, 77, 11) // "handleTimer"

    },
    "MonitorClient\0received\0\0disconnectRequested\0"
    "reconnectRequested\0handleReceive\0"
    "handleTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MonitorClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MonitorClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->received(); break;
        case 1: _t->disconnectRequested(); break;
        case 2: _t->reconnectRequested(); break;
        case 3: _t->handleReceive(); break;
        case 4: _t->handleTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MonitorClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MonitorClient::received)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MonitorClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MonitorClient::disconnectRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MonitorClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MonitorClient::reconnectRequested)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MonitorClient::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MonitorClient.data,
    qt_meta_data_MonitorClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MonitorClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MonitorClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MonitorClient::received()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MonitorClient::disconnectRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MonitorClient::reconnectRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
