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
    QByteArrayData data[28];
    char stringdata0[287];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogPlayer_t qt_meta_stringdata_LogPlayer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LogPlayer"
QT_MOC_LITERAL(1, 10, 16), // "signalPlayOrStop"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 4), // "play"
QT_MOC_LITERAL(4, 33, 12), // "indexUpdated"
QT_MOC_LITERAL(5, 46, 6), // "size_t"
QT_MOC_LITERAL(6, 53, 7), // "current"
QT_MOC_LITERAL(7, 61, 7), // "maximum"
QT_MOC_LITERAL(8, 69, 13), // "quitRequested"
QT_MOC_LITERAL(9, 83, 11), // "handleTimer"
QT_MOC_LITERAL(10, 95, 12), // "stepBackward"
QT_MOC_LITERAL(11, 108, 11), // "stepForward"
QT_MOC_LITERAL(12, 120, 10), // "playOrStop"
QT_MOC_LITERAL(13, 131, 4), // "stop"
QT_MOC_LITERAL(14, 136, 12), // "playBackward"
QT_MOC_LITERAL(15, 149, 11), // "playForward"
QT_MOC_LITERAL(16, 161, 10), // "decelerate"
QT_MOC_LITERAL(17, 172, 10), // "accelerate"
QT_MOC_LITERAL(18, 183, 17), // "goToPreviousScore"
QT_MOC_LITERAL(19, 201, 13), // "goToNextScore"
QT_MOC_LITERAL(20, 215, 9), // "goToFirst"
QT_MOC_LITERAL(21, 225, 8), // "goToLast"
QT_MOC_LITERAL(22, 234, 9), // "goToIndex"
QT_MOC_LITERAL(23, 244, 5), // "index"
QT_MOC_LITERAL(24, 250, 9), // "goToCycle"
QT_MOC_LITERAL(25, 260, 5), // "cycle"
QT_MOC_LITERAL(26, 266, 8), // "showLive"
QT_MOC_LITERAL(27, 275, 11) // "setLiveMode"

    },
    "LogPlayer\0signalPlayOrStop\0\0play\0"
    "indexUpdated\0size_t\0current\0maximum\0"
    "quitRequested\0handleTimer\0stepBackward\0"
    "stepForward\0playOrStop\0stop\0playBackward\0"
    "playForward\0decelerate\0accelerate\0"
    "goToPreviousScore\0goToNextScore\0"
    "goToFirst\0goToLast\0goToIndex\0index\0"
    "goToCycle\0cycle\0showLive\0setLiveMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogPlayer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       4,    2,  117,    2, 0x06 /* Public */,
       8,    0,  122,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,  123,    2, 0x08 /* Private */,
      10,    0,  124,    2, 0x0a /* Public */,
      11,    0,  125,    2, 0x0a /* Public */,
      12,    1,  126,    2, 0x0a /* Public */,
      13,    0,  129,    2, 0x0a /* Public */,
      14,    0,  130,    2, 0x0a /* Public */,
      15,    0,  131,    2, 0x0a /* Public */,
      16,    0,  132,    2, 0x0a /* Public */,
      17,    0,  133,    2, 0x0a /* Public */,
      18,    0,  134,    2, 0x0a /* Public */,
      19,    0,  135,    2, 0x0a /* Public */,
      20,    0,  136,    2, 0x0a /* Public */,
      21,    0,  137,    2, 0x0a /* Public */,
      22,    1,  138,    2, 0x0a /* Public */,
      24,    1,  141,    2, 0x0a /* Public */,
      26,    0,  144,    2, 0x0a /* Public */,
      27,    0,  145,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5,    6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::Int,   25,
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
        case 0: _t->signalPlayOrStop((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->indexUpdated((*reinterpret_cast< size_t(*)>(_a[1])),(*reinterpret_cast< size_t(*)>(_a[2]))); break;
        case 2: _t->quitRequested(); break;
        case 3: _t->handleTimer(); break;
        case 4: _t->stepBackward(); break;
        case 5: _t->stepForward(); break;
        case 6: _t->playOrStop((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->stop(); break;
        case 8: _t->playBackward(); break;
        case 9: _t->playForward(); break;
        case 10: _t->decelerate(); break;
        case 11: _t->accelerate(); break;
        case 12: _t->goToPreviousScore(); break;
        case 13: _t->goToNextScore(); break;
        case 14: _t->goToFirst(); break;
        case 15: _t->goToLast(); break;
        case 16: _t->goToIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->goToCycle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->showLive(); break;
        case 19: _t->setLiveMode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LogPlayer::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogPlayer::signalPlayOrStop)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LogPlayer::*)(size_t , size_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogPlayer::indexUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LogPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogPlayer::quitRequested)) {
                *result = 2;
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
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void LogPlayer::signalPlayOrStop(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LogPlayer::indexUpdated(size_t _t1, size_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LogPlayer::quitRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
