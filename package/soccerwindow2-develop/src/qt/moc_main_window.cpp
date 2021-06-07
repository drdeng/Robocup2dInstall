/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "main_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[56];
    char stringdata0[760];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 11), // "viewUpdated"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "openRCG"
QT_MOC_LITERAL(4, 32, 7), // "saveRCG"
QT_MOC_LITERAL(5, 40, 13), // "openDebugView"
QT_MOC_LITERAL(6, 54, 13), // "saveDebugView"
QT_MOC_LITERAL(7, 68, 12), // "openDrawData"
QT_MOC_LITERAL(8, 81, 7), // "kickOff"
QT_MOC_LITERAL(9, 89, 11), // "setLiveMode"
QT_MOC_LITERAL(10, 101, 14), // "connectMonitor"
QT_MOC_LITERAL(11, 116, 16), // "connectMonitorTo"
QT_MOC_LITERAL(12, 133, 17), // "disconnectMonitor"
QT_MOC_LITERAL(13, 151, 10), // "killServer"
QT_MOC_LITERAL(14, 162, 11), // "startServer"
QT_MOC_LITERAL(15, 174, 13), // "restartServer"
QT_MOC_LITERAL(16, 188, 7), // "command"
QT_MOC_LITERAL(17, 196, 18), // "toggleDragMoveMode"
QT_MOC_LITERAL(18, 215, 2), // "on"
QT_MOC_LITERAL(19, 218, 18), // "showLauncherDialog"
QT_MOC_LITERAL(20, 237, 14), // "changePlayMode"
QT_MOC_LITERAL(21, 252, 4), // "mode"
QT_MOC_LITERAL(22, 257, 5), // "point"
QT_MOC_LITERAL(23, 263, 13), // "toggleMenuBar"
QT_MOC_LITERAL(24, 277, 13), // "toggleToolBar"
QT_MOC_LITERAL(25, 291, 15), // "toggleStatusBar"
QT_MOC_LITERAL(26, 307, 16), // "toggleFullScreen"
QT_MOC_LITERAL(27, 324, 20), // "showPlayerTypeDialog"
QT_MOC_LITERAL(28, 345, 16), // "showDetailDialog"
QT_MOC_LITERAL(29, 362, 11), // "changeStyle"
QT_MOC_LITERAL(30, 374, 7), // "checked"
QT_MOC_LITERAL(31, 382, 22), // "showColorSettingDialog"
QT_MOC_LITERAL(32, 405, 21), // "showFontSettingDialog"
QT_MOC_LITERAL(33, 427, 21), // "showMonitorMoveDialog"
QT_MOC_LITERAL(34, 449, 20), // "showViewConfigDialog"
QT_MOC_LITERAL(35, 470, 22), // "showDebugMessageWindow"
QT_MOC_LITERAL(36, 493, 17), // "toggleDebugServer"
QT_MOC_LITERAL(37, 511, 16), // "startDebugServer"
QT_MOC_LITERAL(38, 528, 15), // "stopDebugServer"
QT_MOC_LITERAL(39, 544, 19), // "showImageSaveDialog"
QT_MOC_LITERAL(40, 564, 5), // "about"
QT_MOC_LITERAL(41, 570, 17), // "printShortcutKeys"
QT_MOC_LITERAL(42, 588, 12), // "resizeCanvas"
QT_MOC_LITERAL(43, 601, 4), // "size"
QT_MOC_LITERAL(44, 606, 16), // "saveImageAndQuit"
QT_MOC_LITERAL(45, 623, 20), // "receiveMonitorPacket"
QT_MOC_LITERAL(46, 644, 19), // "updatePositionLabel"
QT_MOC_LITERAL(47, 664, 13), // "dropBallThere"
QT_MOC_LITERAL(48, 678, 8), // "dropBall"
QT_MOC_LITERAL(49, 687, 3), // "pos"
QT_MOC_LITERAL(50, 691, 12), // "freeKickLeft"
QT_MOC_LITERAL(51, 704, 13), // "freeKickRight"
QT_MOC_LITERAL(52, 718, 10), // "movePlayer"
QT_MOC_LITERAL(53, 729, 11), // "moveObjects"
QT_MOC_LITERAL(54, 741, 10), // "yellowCard"
QT_MOC_LITERAL(55, 752, 7) // "redCard"

    },
    "MainWindow\0viewUpdated\0\0openRCG\0saveRCG\0"
    "openDebugView\0saveDebugView\0openDrawData\0"
    "kickOff\0setLiveMode\0connectMonitor\0"
    "connectMonitorTo\0disconnectMonitor\0"
    "killServer\0startServer\0restartServer\0"
    "command\0toggleDragMoveMode\0on\0"
    "showLauncherDialog\0changePlayMode\0"
    "mode\0point\0toggleMenuBar\0toggleToolBar\0"
    "toggleStatusBar\0toggleFullScreen\0"
    "showPlayerTypeDialog\0showDetailDialog\0"
    "changeStyle\0checked\0showColorSettingDialog\0"
    "showFontSettingDialog\0showMonitorMoveDialog\0"
    "showViewConfigDialog\0showDebugMessageWindow\0"
    "toggleDebugServer\0startDebugServer\0"
    "stopDebugServer\0showImageSaveDialog\0"
    "about\0printShortcutKeys\0resizeCanvas\0"
    "size\0saveImageAndQuit\0receiveMonitorPacket\0"
    "updatePositionLabel\0dropBallThere\0"
    "dropBall\0pos\0freeKickLeft\0freeKickRight\0"
    "movePlayer\0moveObjects\0yellowCard\0"
    "redCard"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      48,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  254,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  255,    2, 0x08 /* Private */,
       4,    0,  256,    2, 0x08 /* Private */,
       5,    0,  257,    2, 0x08 /* Private */,
       6,    0,  258,    2, 0x08 /* Private */,
       7,    0,  259,    2, 0x08 /* Private */,
       8,    0,  260,    2, 0x08 /* Private */,
       9,    0,  261,    2, 0x08 /* Private */,
      10,    0,  262,    2, 0x08 /* Private */,
      11,    0,  263,    2, 0x08 /* Private */,
      12,    0,  264,    2, 0x08 /* Private */,
      13,    0,  265,    2, 0x08 /* Private */,
      14,    0,  266,    2, 0x08 /* Private */,
      15,    0,  267,    2, 0x08 /* Private */,
      15,    1,  268,    2, 0x08 /* Private */,
      17,    1,  271,    2, 0x08 /* Private */,
      19,    0,  274,    2, 0x08 /* Private */,
      20,    2,  275,    2, 0x08 /* Private */,
      23,    0,  280,    2, 0x08 /* Private */,
      24,    0,  281,    2, 0x08 /* Private */,
      25,    0,  282,    2, 0x08 /* Private */,
      26,    0,  283,    2, 0x08 /* Private */,
      27,    0,  284,    2, 0x08 /* Private */,
      28,    0,  285,    2, 0x08 /* Private */,
      29,    1,  286,    2, 0x08 /* Private */,
      31,    0,  289,    2, 0x08 /* Private */,
      32,    0,  290,    2, 0x08 /* Private */,
      33,    0,  291,    2, 0x08 /* Private */,
      34,    0,  292,    2, 0x08 /* Private */,
      35,    0,  293,    2, 0x08 /* Private */,
      36,    1,  294,    2, 0x08 /* Private */,
      37,    0,  297,    2, 0x08 /* Private */,
      38,    0,  298,    2, 0x08 /* Private */,
      39,    0,  299,    2, 0x08 /* Private */,
      40,    0,  300,    2, 0x08 /* Private */,
      41,    0,  301,    2, 0x08 /* Private */,
      42,    1,  302,    2, 0x08 /* Private */,
      44,    0,  305,    2, 0x08 /* Private */,
      45,    0,  306,    2, 0x0a /* Public */,
      46,    1,  307,    2, 0x0a /* Public */,
      47,    0,  310,    2, 0x0a /* Public */,
      48,    1,  311,    2, 0x0a /* Public */,
      50,    1,  314,    2, 0x0a /* Public */,
      51,    1,  317,    2, 0x0a /* Public */,
      52,    1,  320,    2, 0x0a /* Public */,
      53,    0,  323,    2, 0x0a /* Public */,
      54,    0,  324,    2, 0x0a /* Public */,
      55,    0,  325,    2, 0x0a /* Public */,

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
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,   21,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   30,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QSize,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   49,
    QMetaType::Void, QMetaType::QPoint,   49,
    QMetaType::Void, QMetaType::QPoint,   49,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->viewUpdated(); break;
        case 1: _t->openRCG(); break;
        case 2: _t->saveRCG(); break;
        case 3: _t->openDebugView(); break;
        case 4: _t->saveDebugView(); break;
        case 5: _t->openDrawData(); break;
        case 6: _t->kickOff(); break;
        case 7: _t->setLiveMode(); break;
        case 8: _t->connectMonitor(); break;
        case 9: _t->connectMonitorTo(); break;
        case 10: _t->disconnectMonitor(); break;
        case 11: _t->killServer(); break;
        case 12: _t->startServer(); break;
        case 13: _t->restartServer(); break;
        case 14: _t->restartServer((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->toggleDragMoveMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->showLauncherDialog(); break;
        case 17: _t->changePlayMode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 18: _t->toggleMenuBar(); break;
        case 19: _t->toggleToolBar(); break;
        case 20: _t->toggleStatusBar(); break;
        case 21: _t->toggleFullScreen(); break;
        case 22: _t->showPlayerTypeDialog(); break;
        case 23: _t->showDetailDialog(); break;
        case 24: _t->changeStyle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->showColorSettingDialog(); break;
        case 26: _t->showFontSettingDialog(); break;
        case 27: _t->showMonitorMoveDialog(); break;
        case 28: _t->showViewConfigDialog(); break;
        case 29: _t->showDebugMessageWindow(); break;
        case 30: _t->toggleDebugServer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->startDebugServer(); break;
        case 32: _t->stopDebugServer(); break;
        case 33: _t->showImageSaveDialog(); break;
        case 34: _t->about(); break;
        case 35: _t->printShortcutKeys(); break;
        case 36: _t->resizeCanvas((*reinterpret_cast< const QSize(*)>(_a[1]))); break;
        case 37: _t->saveImageAndQuit(); break;
        case 38: _t->receiveMonitorPacket(); break;
        case 39: _t->updatePositionLabel((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 40: _t->dropBallThere(); break;
        case 41: _t->dropBall((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 42: _t->freeKickLeft((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 43: _t->freeKickRight((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 44: _t->movePlayer((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 45: _t->moveObjects(); break;
        case 46: _t->yellowCard(); break;
        case 47: _t->redCard(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::viewUpdated)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 48)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 48;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 48)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 48;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::viewUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
