/****************************************************************************
** Meta object code from reading C++ file 'log_player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "log_player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'log_player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogPlayer_t {
    QByteArrayData data[24];
    char stringdata0[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogPlayer_t qt_meta_stringdata_LogPlayer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LogPlayer"
QT_MOC_LITERAL(1, 10, 7), // "updated"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 11), // "handleTimer"
QT_MOC_LITERAL(4, 31, 8), // "stepBack"
QT_MOC_LITERAL(5, 40, 11), // "stepForward"
QT_MOC_LITERAL(6, 52, 10), // "playOrStop"
QT_MOC_LITERAL(7, 63, 4), // "stop"
QT_MOC_LITERAL(8, 68, 8), // "playBack"
QT_MOC_LITERAL(9, 77, 11), // "playForward"
QT_MOC_LITERAL(10, 89, 13), // "goToPrevScore"
QT_MOC_LITERAL(11, 103, 13), // "goToNextScore"
QT_MOC_LITERAL(12, 117, 9), // "goToFirst"
QT_MOC_LITERAL(13, 127, 8), // "goToLast"
QT_MOC_LITERAL(14, 136, 10), // "decelerate"
QT_MOC_LITERAL(15, 147, 10), // "accelerate"
QT_MOC_LITERAL(16, 158, 9), // "goToIndex"
QT_MOC_LITERAL(17, 168, 5), // "index"
QT_MOC_LITERAL(18, 174, 9), // "goToCycle"
QT_MOC_LITERAL(19, 184, 5), // "cycle"
QT_MOC_LITERAL(20, 190, 14), // "rcsc::GameTime"
QT_MOC_LITERAL(21, 205, 1), // "t"
QT_MOC_LITERAL(22, 207, 8), // "showLive"
QT_MOC_LITERAL(23, 216, 11) // "setLiveMode"

    },
    "LogPlayer\0updated\0\0handleTimer\0stepBack\0"
    "stepForward\0playOrStop\0stop\0playBack\0"
    "playForward\0goToPrevScore\0goToNextScore\0"
    "goToFirst\0goToLast\0decelerate\0accelerate\0"
    "goToIndex\0index\0goToCycle\0cycle\0"
    "rcsc::GameTime\0t\0showLive\0setLiveMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogPlayer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  110,    2, 0x08 /* Private */,
       4,    0,  111,    2, 0x0a /* Public */,
       5,    0,  112,    2, 0x0a /* Public */,
       6,    0,  113,    2, 0x0a /* Public */,
       7,    0,  114,    2, 0x0a /* Public */,
       8,    0,  115,    2, 0x0a /* Public */,
       9,    0,  116,    2, 0x0a /* Public */,
      10,    0,  117,    2, 0x0a /* Public */,
      11,    0,  118,    2, 0x0a /* Public */,
      12,    0,  119,    2, 0x0a /* Public */,
      13,    0,  120,    2, 0x0a /* Public */,
      14,    0,  121,    2, 0x0a /* Public */,
      15,    0,  122,    2, 0x0a /* Public */,
      16,    1,  123,    2, 0x0a /* Public */,
      18,    1,  126,    2, 0x0a /* Public */,
      18,    1,  129,    2, 0x0a /* Public */,
      22,    0,  132,    2, 0x0a /* Public */,
      23,    0,  133,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LogPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LogPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        case 1: _t->handleTimer(); break;
        case 2: _t->stepBack(); break;
        case 3: _t->stepForward(); break;
        case 4: _t->playOrStop(); break;
        case 5: _t->stop(); break;
        case 6: _t->playBack(); break;
        case 7: _t->playForward(); break;
        case 8: _t->goToPrevScore(); break;
        case 9: _t->goToNextScore(); break;
        case 10: _t->goToFirst(); break;
        case 11: _t->goToLast(); break;
        case 12: _t->decelerate(); break;
        case 13: _t->accelerate(); break;
        case 14: _t->goToIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->goToCycle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->goToCycle((*reinterpret_cast< const rcsc::GameTime(*)>(_a[1]))); break;
        case 17: _t->showLive(); break;
        case 18: _t->setLiveMode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LogPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogPlayer::updated)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LogPlayer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_LogPlayer.data,
    qt_meta_data_LogPlayer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LogPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogPlayer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LogPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void LogPlayer::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
