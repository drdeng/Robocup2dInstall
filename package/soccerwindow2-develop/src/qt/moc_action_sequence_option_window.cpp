/****************************************************************************
** Meta object code from reading C++ file 'action_sequence_option_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "action_sequence_option_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'action_sequence_option_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ActionSequenceOptionWindow_t {
    QByteArrayData data[7];
    char stringdata0[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ActionSequenceOptionWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ActionSequenceOptionWindow_t qt_meta_stringdata_ActionSequenceOptionWindow = {
    {
QT_MOC_LITERAL(0, 0, 26), // "ActionSequenceOptionWindow"
QT_MOC_LITERAL(1, 27, 18), // "inOptionMaxNoClick"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 8), // "saveFile"
QT_MOC_LITERAL(4, 56, 16), // "clickRadioButton"
QT_MOC_LITERAL(5, 73, 8), // "loadFile"
QT_MOC_LITERAL(6, 82, 7) // "coutNum"

    },
    "ActionSequenceOptionWindow\0"
    "inOptionMaxNoClick\0\0saveFile\0"
    "clickRadioButton\0loadFile\0coutNum"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ActionSequenceOptionWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       3,    0,   42,    2, 0x08 /* Private */,
       4,    0,   43,    2, 0x08 /* Private */,
       5,    0,   44,    2, 0x08 /* Private */,
       6,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ActionSequenceOptionWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ActionSequenceOptionWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->inOptionMaxNoClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->saveFile(); break;
        case 2: _t->clickRadioButton(); break;
        case 3: _t->loadFile(); break;
        case 4: _t->coutNum(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ActionSequenceOptionWindow::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ActionSequenceOptionWindow.data,
    qt_meta_data_ActionSequenceOptionWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ActionSequenceOptionWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ActionSequenceOptionWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ActionSequenceOptionWindow.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ActionSequenceOptionWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
