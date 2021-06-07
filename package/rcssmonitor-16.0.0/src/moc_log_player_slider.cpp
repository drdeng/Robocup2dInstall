/****************************************************************************
** Meta object code from reading C++ file 'log_player_slider.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "log_player_slider.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'log_player_slider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogPlayerSlider_t {
    QByteArrayData data[11];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogPlayerSlider_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogPlayerSlider_t qt_meta_stringdata_LogPlayerSlider = {
    {
QT_MOC_LITERAL(0, 0, 15), // "LogPlayerSlider"
QT_MOC_LITERAL(1, 16, 12), // "cycleChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 5), // "cycle"
QT_MOC_LITERAL(4, 36, 17), // "changeOrientation"
QT_MOC_LITERAL(5, 54, 15), // "Qt::Orientation"
QT_MOC_LITERAL(6, 70, 11), // "orientation"
QT_MOC_LITERAL(7, 82, 11), // "updateIndex"
QT_MOC_LITERAL(8, 94, 6), // "size_t"
QT_MOC_LITERAL(9, 101, 7), // "current"
QT_MOC_LITERAL(10, 109, 7) // "maximum"

    },
    "LogPlayerSlider\0cycleChanged\0\0cycle\0"
    "changeOrientation\0Qt::Orientation\0"
    "orientation\0updateIndex\0size_t\0current\0"
    "maximum"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogPlayerSlider[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   32,    2, 0x0a /* Public */,
       7,    2,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8,    9,   10,

       0        // eod
};

void LogPlayerSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LogPlayerSlider *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->cycleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->changeOrientation((*reinterpret_cast< Qt::Orientation(*)>(_a[1]))); break;
        case 2: _t->updateIndex((*reinterpret_cast< size_t(*)>(_a[1])),(*reinterpret_cast< size_t(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LogPlayerSlider::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogPlayerSlider::cycleChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LogPlayerSlider::staticMetaObject = { {
    &QSlider::staticMetaObject,
    qt_meta_stringdata_LogPlayerSlider.data,
    qt_meta_data_LogPlayerSlider,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LogPlayerSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogPlayerSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogPlayerSlider.stringdata0))
        return static_cast<void*>(this);
    return QSlider::qt_metacast(_clname);
}

int LogPlayerSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void LogPlayerSlider::cycleChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
