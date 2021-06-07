/****************************************************************************
** Meta object code from reading C++ file 'constraint_view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "constraint_view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'constraint_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConstraintView_t {
    QByteArrayData data[16];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConstraintView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConstraintView_t qt_meta_stringdata_ConstraintView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ConstraintView"
QT_MOC_LITERAL(1, 15, 18), // "constraintSelected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 3), // "idx"
QT_MOC_LITERAL(4, 39, 25), // "constraintDeleteRequested"
QT_MOC_LITERAL(5, 65, 10), // "origin_idx"
QT_MOC_LITERAL(6, 76, 12), // "terminal_idx"
QT_MOC_LITERAL(7, 89, 18), // "constraintReplaced"
QT_MOC_LITERAL(8, 108, 14), // "setCurrentData"
QT_MOC_LITERAL(9, 123, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(10, 141, 7), // "current"
QT_MOC_LITERAL(11, 149, 20), // "menuDeleteConstraint"
QT_MOC_LITERAL(12, 170, 21), // "changeConstraintIndex"
QT_MOC_LITERAL(13, 192, 11), // "QModelIndex"
QT_MOC_LITERAL(14, 204, 5), // "index"
QT_MOC_LITERAL(15, 210, 5) // "value"

    },
    "ConstraintView\0constraintSelected\0\0"
    "idx\0constraintDeleteRequested\0origin_idx\0"
    "terminal_idx\0constraintReplaced\0"
    "setCurrentData\0QTableWidgetItem*\0"
    "current\0menuDeleteConstraint\0"
    "changeConstraintIndex\0QModelIndex\0"
    "index\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConstraintView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    2,   47,    2, 0x06 /* Public */,
       7,    3,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   59,    2, 0x08 /* Private */,
      11,    0,   62,    2, 0x08 /* Private */,
      12,    2,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    5,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13, QMetaType::Int,   14,   15,

       0        // eod
};

void ConstraintView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConstraintView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->constraintSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->constraintDeleteRequested((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->constraintReplaced((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->setCurrentData((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->menuDeleteConstraint(); break;
        case 5: _t->changeConstraintIndex((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConstraintView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConstraintView::constraintSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConstraintView::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConstraintView::constraintDeleteRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ConstraintView::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConstraintView::constraintReplaced)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConstraintView::staticMetaObject = { {
    &QTableWidget::staticMetaObject,
    qt_meta_stringdata_ConstraintView.data,
    qt_meta_data_ConstraintView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConstraintView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConstraintView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConstraintView.stringdata0))
        return static_cast<void*>(this);
    return QTableWidget::qt_metacast(_clname);
}

int ConstraintView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ConstraintView::constraintSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConstraintView::constraintDeleteRequested(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConstraintView::constraintReplaced(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
