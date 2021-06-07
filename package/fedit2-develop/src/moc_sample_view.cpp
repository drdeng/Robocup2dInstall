/****************************************************************************
** Meta object code from reading C++ file 'sample_view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sample_view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sample_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SampleView_t {
    QByteArrayData data[22];
    char stringdata0[272];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SampleView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SampleView_t qt_meta_stringdata_SampleView = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SampleView"
QT_MOC_LITERAL(1, 11, 14), // "sampleSelected"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 3), // "idx"
QT_MOC_LITERAL(4, 31, 26), // "sampleIndexChangeRequested"
QT_MOC_LITERAL(5, 58, 16), // "old_visual_index"
QT_MOC_LITERAL(6, 75, 16), // "new_visual_index"
QT_MOC_LITERAL(7, 92, 21), // "sampleDeleteRequested"
QT_MOC_LITERAL(8, 114, 12), // "ballReplaced"
QT_MOC_LITERAL(9, 127, 1), // "x"
QT_MOC_LITERAL(10, 129, 1), // "y"
QT_MOC_LITERAL(11, 131, 14), // "playerReplaced"
QT_MOC_LITERAL(12, 146, 4), // "unum"
QT_MOC_LITERAL(13, 151, 14), // "setCurrentData"
QT_MOC_LITERAL(14, 166, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(15, 183, 7), // "current"
QT_MOC_LITERAL(16, 191, 21), // "menuChangeSampleIndex"
QT_MOC_LITERAL(17, 213, 16), // "menuDeleteSample"
QT_MOC_LITERAL(18, 230, 17), // "changeCoordinates"
QT_MOC_LITERAL(19, 248, 11), // "QModelIndex"
QT_MOC_LITERAL(20, 260, 5), // "index"
QT_MOC_LITERAL(21, 266, 5) // "value"

    },
    "SampleView\0sampleSelected\0\0idx\0"
    "sampleIndexChangeRequested\0old_visual_index\0"
    "new_visual_index\0sampleDeleteRequested\0"
    "ballReplaced\0x\0y\0playerReplaced\0unum\0"
    "setCurrentData\0QTreeWidgetItem*\0current\0"
    "menuChangeSampleIndex\0menuDeleteSample\0"
    "changeCoordinates\0QModelIndex\0index\0"
    "value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SampleView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    2,   62,    2, 0x06 /* Public */,
       7,    1,   67,    2, 0x06 /* Public */,
       8,    3,   70,    2, 0x06 /* Public */,
      11,    4,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    1,   86,    2, 0x08 /* Private */,
      16,    0,   89,    2, 0x08 /* Private */,
      17,    0,   90,    2, 0x08 /* Private */,
      18,    2,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double,    3,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double,    3,   12,    9,   10,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19, QMetaType::Double,   20,   21,

       0        // eod
};

void SampleView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SampleView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sampleSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sampleIndexChangeRequested((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->sampleDeleteRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ballReplaced((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 4: _t->playerReplaced((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 5: _t->setCurrentData((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->menuChangeSampleIndex(); break;
        case 7: _t->menuDeleteSample(); break;
        case 8: _t->changeCoordinates((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SampleView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SampleView::sampleSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SampleView::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SampleView::sampleIndexChangeRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SampleView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SampleView::sampleDeleteRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SampleView::*)(int , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SampleView::ballReplaced)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SampleView::*)(int , int , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SampleView::playerReplaced)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SampleView::staticMetaObject = { {
    &QTreeWidget::staticMetaObject,
    qt_meta_stringdata_SampleView.data,
    qt_meta_data_SampleView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SampleView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SampleView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SampleView.stringdata0))
        return static_cast<void*>(this);
    return QTreeWidget::qt_metacast(_clname);
}

int SampleView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SampleView::sampleSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SampleView::sampleIndexChangeRequested(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SampleView::sampleDeleteRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SampleView::ballReplaced(int _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SampleView::playerReplaced(int _t1, int _t2, double _t3, double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
