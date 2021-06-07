/****************************************************************************
** Meta object code from reading C++ file 'config_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "config_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'config_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConfigDialog_t {
    QByteArrayData data[11];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConfigDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConfigDialog_t qt_meta_stringdata_ConfigDialog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ConfigDialog"
QT_MOC_LITERAL(1, 13, 17), // "viewResizeApplied"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 4), // "size"
QT_MOC_LITERAL(4, 37, 10), // "configured"
QT_MOC_LITERAL(5, 48, 5), // "shown"
QT_MOC_LITERAL(6, 54, 2), // "on"
QT_MOC_LITERAL(7, 57, 9), // "updateAll"
QT_MOC_LITERAL(8, 67, 13), // "applyViewSize"
QT_MOC_LITERAL(9, 81, 13), // "editViewScale"
QT_MOC_LITERAL(10, 95, 4) // "text"

    },
    "ConfigDialog\0viewResizeApplied\0\0size\0"
    "configured\0shown\0on\0updateAll\0"
    "applyViewSize\0editViewScale\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConfigDialog[] = {

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
       4,    0,   47,    2, 0x06 /* Public */,
       5,    1,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   51,    2, 0x08 /* Private */,
       8,    0,   52,    2, 0x08 /* Private */,
       9,    1,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QSize,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void ConfigDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConfigDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->viewResizeApplied((*reinterpret_cast< const QSize(*)>(_a[1]))); break;
        case 1: _t->configured(); break;
        case 2: _t->shown((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->updateAll(); break;
        case 4: _t->applyViewSize(); break;
        case 5: _t->editViewScale((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConfigDialog::*)(const QSize & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigDialog::viewResizeApplied)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConfigDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigDialog::configured)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ConfigDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigDialog::shown)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConfigDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ConfigDialog.data,
    qt_meta_data_ConfigDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConfigDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void ConfigDialog::viewResizeApplied(const QSize & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConfigDialog::configured()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ConfigDialog::shown(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
