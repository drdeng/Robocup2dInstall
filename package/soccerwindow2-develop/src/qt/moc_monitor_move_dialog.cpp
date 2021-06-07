/****************************************************************************
** Meta object code from reading C++ file 'monitor_move_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "monitor_move_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_move_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MonitorMoveDialog_t {
    QByteArrayData data[15];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorMoveDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorMoveDialog_t qt_meta_stringdata_MonitorMoveDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MonitorMoveDialog"
QT_MOC_LITERAL(1, 18, 8), // "executed"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "readFieldStatus"
QT_MOC_LITERAL(4, 44, 4), // "open"
QT_MOC_LITERAL(5, 49, 4), // "save"
QT_MOC_LITERAL(6, 54, 15), // "toggleBallCheck"
QT_MOC_LITERAL(7, 70, 2), // "on"
QT_MOC_LITERAL(8, 73, 18), // "toggleBallVelCheck"
QT_MOC_LITERAL(9, 92, 13), // "toggleLeftAll"
QT_MOC_LITERAL(10, 106, 14), // "toggleRightAll"
QT_MOC_LITERAL(11, 121, 15), // "toggleLeftCheck"
QT_MOC_LITERAL(12, 137, 5), // "index"
QT_MOC_LITERAL(13, 143, 16), // "toggleRightCheck"
QT_MOC_LITERAL(14, 160, 11) // "sendCommand"

    },
    "MonitorMoveDialog\0executed\0\0readFieldStatus\0"
    "open\0save\0toggleBallCheck\0on\0"
    "toggleBallVelCheck\0toggleLeftAll\0"
    "toggleRightAll\0toggleLeftCheck\0index\0"
    "toggleRightCheck\0sendCommand"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorMoveDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    1,   73,    2, 0x08 /* Private */,
       8,    1,   76,    2, 0x08 /* Private */,
       9,    1,   79,    2, 0x08 /* Private */,
      10,    1,   82,    2, 0x08 /* Private */,
      11,    1,   85,    2, 0x08 /* Private */,
      13,    1,   88,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

void MonitorMoveDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MonitorMoveDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->executed(); break;
        case 1: _t->readFieldStatus(); break;
        case 2: _t->open(); break;
        case 3: _t->save(); break;
        case 4: _t->toggleBallCheck((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->toggleBallVelCheck((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->toggleLeftAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->toggleRightAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->toggleLeftCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->toggleRightCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->sendCommand(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MonitorMoveDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MonitorMoveDialog::executed)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MonitorMoveDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_MonitorMoveDialog.data,
    qt_meta_data_MonitorMoveDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MonitorMoveDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorMoveDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorMoveDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MonitorMoveDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void MonitorMoveDialog::executed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
