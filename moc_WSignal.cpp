/****************************************************************************
** Meta object code from reading C++ file 'WSignal.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WSignal.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WSignal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WSignal_t {
    QByteArrayData data[11];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WSignal_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WSignal_t qt_meta_stringdata_WSignal = {
    {
QT_MOC_LITERAL(0, 0, 7), // "WSignal"
QT_MOC_LITERAL(1, 8, 8), // "listened"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 8), // "WSignal*"
QT_MOC_LITERAL(4, 27, 9), // "clickedWS"
QT_MOC_LITERAL(5, 37, 25), // "refreshStellwerkstechnikW"
QT_MOC_LITERAL(6, 63, 11), // "std::string"
QT_MOC_LITERAL(7, 75, 3), // "sig"
QT_MOC_LITERAL(8, 79, 4), // "stat"
QT_MOC_LITERAL(9, 84, 10), // "listenToFS"
QT_MOC_LITERAL(10, 95, 12) // "zugpassiertW"

    },
    "WSignal\0listened\0\0WSignal*\0clickedWS\0"
    "refreshStellwerkstechnikW\0std::string\0"
    "sig\0stat\0listenToFS\0zugpassiertW"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WSignal[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    2,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   42,    2, 0x0a /* Public */,
      10,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Bool,    7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WSignal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WSignal *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->listened((*reinterpret_cast< WSignal*(*)>(_a[1]))); break;
        case 1: _t->refreshStellwerkstechnikW((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->listenToFS(); break;
        case 3: _t->zugpassiertW(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< WSignal* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WSignal::*)(WSignal * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WSignal::listened)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WSignal::*)(std::string , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WSignal::refreshStellwerkstechnikW)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WSignal::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_WSignal.data,
    qt_meta_data_WSignal,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WSignal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WSignal::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WSignal.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "VSignal"))
        return static_cast< VSignal*>(this);
    return QObject::qt_metacast(_clname);
}

int WSignal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void WSignal::listened(WSignal * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WSignal::refreshStellwerkstechnikW(std::string _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
