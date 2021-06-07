/****************************************************************************
** Meta object code from reading C++ file 'color_setting_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "color_setting_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'color_setting_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ColorSettingDialog_t {
    QByteArrayData data[8];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ColorSettingDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ColorSettingDialog_t qt_meta_stringdata_ColorSettingDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ColorSettingDialog"
QT_MOC_LITERAL(1, 19, 12), // "colorChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 16), // "setDefaultColors"
QT_MOC_LITERAL(4, 50, 11), // "selectEntry"
QT_MOC_LITERAL(5, 62, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(6, 79, 4), // "item"
QT_MOC_LITERAL(7, 84, 6) // "reject"

    },
    "ColorSettingDialog\0colorChanged\0\0"
    "setDefaultColors\0selectEntry\0"
    "QListWidgetItem*\0item\0reject"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ColorSettingDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       4,    1,   42,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

       0        // eod
};

void ColorSettingDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorSettingDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->colorChanged(); break;
        case 1: _t->setDefaultColors(); break;
        case 2: _t->selectEntry(); break;
        case 3: _t->selectEntry((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->reject(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ColorSettingDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorSettingDialog::colorChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ColorSettingDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ColorSettingDialog.data,
    qt_meta_data_ColorSettingDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ColorSettingDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorSettingDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ColorSettingDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ColorSettingDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void ColorSettingDialog::colorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
