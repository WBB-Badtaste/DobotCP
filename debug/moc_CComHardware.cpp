/****************************************************************************
** Meta object code from reading C++ file 'CComHardware.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/ComWrapper/CComHardware.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CComHardware.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CComHardware_t {
    QByteArrayData data[5];
    char stringdata0[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CComHardware_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CComHardware_t qt_meta_stringdata_CComHardware = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CComHardware"
QT_MOC_LITERAL(1, 13, 11), // "onReadyRead"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 14), // "onBytesWritten"
QT_MOC_LITERAL(4, 41, 5) // "bytes"

    },
    "CComHardware\0onReadyRead\0\0onBytesWritten\0"
    "bytes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CComHardware[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    1,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    4,

       0        // eod
};

void CComHardware::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CComHardware *_t = static_cast<CComHardware *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onReadyRead(); break;
        case 1: _t->onBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CComHardware::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CComHardware.data,
      qt_meta_data_CComHardware,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CComHardware::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CComHardware::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CComHardware.stringdata0))
        return static_cast<void*>(const_cast< CComHardware*>(this));
    return QObject::qt_metacast(_clname);
}

int CComHardware::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
