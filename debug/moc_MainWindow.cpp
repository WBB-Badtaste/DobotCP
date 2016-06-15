/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[280];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "onPenTypeChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "penType"
QT_MOC_LITERAL(4, 37, 16), // "onLoadBtnClicked"
QT_MOC_LITERAL(5, 54, 18), // "transformAllPoints"
QT_MOC_LITERAL(6, 73, 22), // "onTextDesignBtnClicked"
QT_MOC_LITERAL(7, 96, 14), // "onGetPoseTimer"
QT_MOC_LITERAL(8, 111, 24), // "onPlaybackStartBtnVerify"
QT_MOC_LITERAL(9, 136, 17), // "onStartBtnClicked"
QT_MOC_LITERAL(10, 154, 16), // "onStopBtnClicked"
QT_MOC_LITERAL(11, 171, 18), // "onConfigBtnClicked"
QT_MOC_LITERAL(12, 190, 19), // "onUserDefBtnClicked"
QT_MOC_LITERAL(13, 210, 17), // "onMouseBtnClicked"
QT_MOC_LITERAL(14, 228, 19), // "onSendPlaybackTimer"
QT_MOC_LITERAL(15, 248, 11), // "delayedInit"
QT_MOC_LITERAL(16, 260, 19) // "onPeriodicTaskTimer"

    },
    "MainWindow\0onPenTypeChanged\0\0penType\0"
    "onLoadBtnClicked\0transformAllPoints\0"
    "onTextDesignBtnClicked\0onGetPoseTimer\0"
    "onPlaybackStartBtnVerify\0onStartBtnClicked\0"
    "onStopBtnClicked\0onConfigBtnClicked\0"
    "onUserDefBtnClicked\0onMouseBtnClicked\0"
    "onSendPlaybackTimer\0delayedInit\0"
    "onPeriodicTaskTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x08 /* Private */,
       4,    0,   87,    2, 0x08 /* Private */,
       5,    0,   88,    2, 0x08 /* Private */,
       6,    0,   89,    2, 0x08 /* Private */,
       7,    0,   90,    2, 0x08 /* Private */,
       8,    0,   91,    2, 0x08 /* Private */,
       9,    0,   92,    2, 0x08 /* Private */,
      10,    0,   93,    2, 0x08 /* Private */,
      11,    0,   94,    2, 0x08 /* Private */,
      12,    0,   95,    2, 0x08 /* Private */,
      13,    0,   96,    2, 0x08 /* Private */,
      14,    0,   97,    2, 0x08 /* Private */,
      15,    0,   98,    2, 0x08 /* Private */,
      16,    0,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
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

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onPenTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onLoadBtnClicked(); break;
        case 2: _t->transformAllPoints(); break;
        case 3: _t->onTextDesignBtnClicked(); break;
        case 4: _t->onGetPoseTimer(); break;
        case 5: _t->onPlaybackStartBtnVerify(); break;
        case 6: _t->onStartBtnClicked(); break;
        case 7: _t->onStopBtnClicked(); break;
        case 8: _t->onConfigBtnClicked(); break;
        case 9: _t->onUserDefBtnClicked(); break;
        case 10: _t->onMouseBtnClicked(); break;
        case 11: _t->onSendPlaybackTimer(); break;
        case 12: _t->delayedInit(); break;
        case 13: _t->onPeriodicTaskTimer(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
