/****************************************************************************
** Meta object code from reading C++ file 'action_sequence_selector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "action_sequence_selector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'action_sequence_selector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ActionSequenceSelector_t {
    QByteArrayData data[20];
    char stringdata0[272];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ActionSequenceSelector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ActionSequenceSelector_t qt_meta_stringdata_ActionSequenceSelector = {
    {
QT_MOC_LITERAL(0, 0, 22), // "ActionSequenceSelector"
QT_MOC_LITERAL(1, 23, 12), // "windowClosed"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 8), // "selected"
QT_MOC_LITERAL(4, 46, 2), // "id"
QT_MOC_LITERAL(5, 49, 14), // "updateListView"
QT_MOC_LITERAL(6, 64, 14), // "updateTreeView"
QT_MOC_LITERAL(7, 79, 14), // "clearSelection"
QT_MOC_LITERAL(8, 94, 15), // "slotItemChanged"
QT_MOC_LITERAL(9, 110, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(10, 127, 4), // "item"
QT_MOC_LITERAL(11, 132, 6), // "column"
QT_MOC_LITERAL(12, 139, 24), // "slotItemSelectionChanged"
QT_MOC_LITERAL(13, 164, 21), // "slotItemDoubleClicked"
QT_MOC_LITERAL(14, 186, 9), // "setFilter"
QT_MOC_LITERAL(15, 196, 3), // "str"
QT_MOC_LITERAL(16, 200, 17), // "setFilterModified"
QT_MOC_LITERAL(17, 218, 15), // "saveCurrentRank"
QT_MOC_LITERAL(18, 234, 20), // "saveCurrentClickRank"
QT_MOC_LITERAL(19, 255, 16) // "showOptionWindow"

    },
    "ActionSequenceSelector\0windowClosed\0"
    "\0selected\0id\0updateListView\0updateTreeView\0"
    "clearSelection\0slotItemChanged\0"
    "QTreeWidgetItem*\0item\0column\0"
    "slotItemSelectionChanged\0slotItemDoubleClicked\0"
    "setFilter\0str\0setFilterModified\0"
    "saveCurrentRank\0saveCurrentClickRank\0"
    "showOptionWindow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ActionSequenceSelector[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   83,    2, 0x0a /* Public */,
       6,    0,   84,    2, 0x0a /* Public */,
       7,    0,   85,    2, 0x0a /* Public */,
       8,    2,   86,    2, 0x08 /* Private */,
      12,    0,   91,    2, 0x08 /* Private */,
      13,    2,   92,    2, 0x08 /* Private */,
      14,    1,   97,    2, 0x08 /* Private */,
      16,    0,  100,    2, 0x08 /* Private */,
      17,    0,  101,    2, 0x08 /* Private */,
      18,    0,  102,    2, 0x08 /* Private */,
      19,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int,   10,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ActionSequenceSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ActionSequenceSelector *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->windowClosed(); break;
        case 1: _t->selected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updateListView(); break;
        case 3: _t->updateTreeView(); break;
        case 4: _t->clearSelection(); break;
        case 5: _t->slotItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->slotItemSelectionChanged(); break;
        case 7: _t->slotItemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->setFilter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->setFilterModified(); break;
        case 10: _t->saveCurrentRank(); break;
        case 11: _t->saveCurrentClickRank(); break;
        case 12: _t->showOptionWindow(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ActionSequenceSelector::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ActionSequenceSelector::windowClosed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ActionSequenceSelector::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ActionSequenceSelector::selected)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ActionSequenceSelector::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ActionSequenceSelector.data,
    qt_meta_data_ActionSequenceSelector,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ActionSequenceSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ActionSequenceSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ActionSequenceSelector.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ActionSequenceSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ActionSequenceSelector::windowClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ActionSequenceSelector::selected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
