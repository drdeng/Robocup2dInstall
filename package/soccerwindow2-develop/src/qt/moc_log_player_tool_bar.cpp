/****************************************************************************
** Meta object code from reading C++ file 'log_player_tool_bar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "log_player_tool_bar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'log_player_tool_bar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogPlayerToolBar_t {
    QByteArrayData data[10];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogPlayerToolBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogPlayerToolBar_t qt_meta_stringdata_LogPlayerToolBar = {
    {
QT_MOC_LITERAL(0, 0, 16), // "LogPlayerToolBar"
QT_MOC_LITERAL(1, 17, 12), // "indexChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "idx"
QT_MOC_LITERAL(4, 35, 12), // "cycleChanged"
QT_MOC_LITERAL(5, 48, 5), // "cycle"
QT_MOC_LITERAL(6, 54, 9), // "editCycle"
QT_MOC_LITERAL(7, 64, 17), // "changeOrientation"
QT_MOC_LITERAL(8, 82, 15), // "Qt::Orientation"
QT_MOC_LITERAL(9, 98, 12) // "updateSlider"

    },
    "LogPlayerToolBar\0indexChanged\0\0idx\0"
    "cycleChanged\0cycle\0editCycle\0"
    "changeOrientation\0Qt::Orientation\0"
    "updateSlider"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogPlayerToolBar[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   45,    2, 0x08 /* Private */,
       7,    1,   46,    2, 0x0a /* Public */,
       9,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,

       0        // eod
};

void LogPlayerToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LogPlayerToolBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->indexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->cycleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->editCycle(); break;
        case 3: _t->changeOrientation((*reinterpret_cast< Qt::Orientation(*)>(_a[1]))); break;
        case 4: _t->updateSlider(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LogPlayerToolBar::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogPlayerToolBar::indexChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LogPlayerToolBar::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogPlayerToolBar::cycleChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LogPlayerToolBar::staticMetaObject = { {
    &QToolBar::staticMetaObject,
    qt_meta_stringdata_LogPlayerToolBar.data,
    qt_meta_data_LogPlayerToolBar,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LogPlayerToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogPlayerToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogPlayerToolBar.stringdata0))
        return static_cast<void*>(this);
    return QToolBar::qt_metacast(_clname);
}

int LogPlayerToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
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
void LogPlayerToolBar::indexChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LogPlayerToolBar::cycleChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
