/****************************************************************************
** Meta object code from reading C++ file 'field_canvas.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "field_canvas.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'field_canvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FieldCanvas_t {
    QByteArrayData data[12];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FieldCanvas_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FieldCanvas_t qt_meta_stringdata_FieldCanvas = {
    {
QT_MOC_LITERAL(0, 0, 11), // "FieldCanvas"
QT_MOC_LITERAL(1, 12, 8), // "dropBall"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 3), // "pos"
QT_MOC_LITERAL(4, 26, 12), // "freeKickLeft"
QT_MOC_LITERAL(5, 39, 13), // "freeKickRight"
QT_MOC_LITERAL(6, 53, 15), // "playModeChanged"
QT_MOC_LITERAL(7, 69, 4), // "mode"
QT_MOC_LITERAL(8, 74, 12), // "focusChanged"
QT_MOC_LITERAL(9, 87, 5), // "point"
QT_MOC_LITERAL(10, 93, 10), // "mouseMoved"
QT_MOC_LITERAL(11, 104, 14) // "changePlayMode"

    },
    "FieldCanvas\0dropBall\0\0pos\0freeKickLeft\0"
    "freeKickRight\0playModeChanged\0mode\0"
    "focusChanged\0point\0mouseMoved\0"
    "changePlayMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FieldCanvas[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       5,    1,   70,    2, 0x06 /* Public */,
       6,    2,   73,    2, 0x06 /* Public */,
       8,    1,   78,    2, 0x06 /* Public */,
      10,    1,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       4,    0,   85,    2, 0x0a /* Public */,
       5,    0,   86,    2, 0x0a /* Public */,
      11,    1,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,    7,    3,
    QMetaType::Void, QMetaType::QPoint,    9,
    QMetaType::Void, QMetaType::QPoint,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void FieldCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FieldCanvas *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dropBall((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: _t->freeKickLeft((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->freeKickRight((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: _t->playModeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 4: _t->focusChanged((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->mouseMoved((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 6: _t->dropBall(); break;
        case 7: _t->freeKickLeft(); break;
        case 8: _t->freeKickRight(); break;
        case 9: _t->changePlayMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FieldCanvas::*)(const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldCanvas::dropBall)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FieldCanvas::*)(const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldCanvas::freeKickLeft)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FieldCanvas::*)(const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldCanvas::freeKickRight)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FieldCanvas::*)(int , const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldCanvas::playModeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FieldCanvas::*)(const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldCanvas::focusChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FieldCanvas::*)(const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldCanvas::mouseMoved)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FieldCanvas::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_FieldCanvas.data,
    qt_meta_data_FieldCanvas,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FieldCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FieldCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FieldCanvas.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FieldCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void FieldCanvas::dropBall(const QPoint & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FieldCanvas::freeKickLeft(const QPoint & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FieldCanvas::freeKickRight(const QPoint & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FieldCanvas::playModeChanged(int _t1, const QPoint & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FieldCanvas::focusChanged(const QPoint & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FieldCanvas::mouseMoved(const QPoint & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
