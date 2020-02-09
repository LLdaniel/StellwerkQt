/****************************************************************************
** Meta object code from reading C++ file 'HSignal.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "HSignal.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HSignal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HSignal_t {
    QByteArrayData data[15];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HSignal_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HSignal_t qt_meta_stringdata_HSignal = {
    {
QT_MOC_LITERAL(0, 0, 7), // "HSignal"
QT_MOC_LITERAL(1, 8, 24), // "refreshStellwerkstechnik"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 11), // "std::string"
QT_MOC_LITERAL(4, 46, 3), // "sig"
QT_MOC_LITERAL(5, 50, 4), // "stat"
QT_MOC_LITERAL(6, 55, 8), // "listened"
QT_MOC_LITERAL(7, 64, 8), // "HSignal*"
QT_MOC_LITERAL(8, 73, 9), // "clickedHS"
QT_MOC_LITERAL(9, 83, 18), // "illuminateSpeicher"
QT_MOC_LITERAL(10, 102, 2), // "sp"
QT_MOC_LITERAL(11, 105, 3), // "str"
QT_MOC_LITERAL(12, 109, 11), // "zugpassiert"
QT_MOC_LITERAL(13, 121, 10), // "listenToFS"
QT_MOC_LITERAL(14, 132, 15) // "recieveSpeicher"

    },
    "HSignal\0refreshStellwerkstechnik\0\0"
    "std::string\0sig\0stat\0listened\0HSignal*\0"
    "clickedHS\0illuminateSpeicher\0sp\0str\0"
    "zugpassiert\0listenToFS\0recieveSpeicher"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HSignal[] = {

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
       1,    2,   44,    2, 0x06 /* Public */,
       6,    1,   49,    2, 0x06 /* Public */,
       9,    2,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   57,    2, 0x0a /* Public */,
      13,    0,   58,    2, 0x0a /* Public */,
      14,    2,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 3,   10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 3,   10,   11,

       0        // eod
};

void HSignal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HSignal *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refreshStellwerkstechnik((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->listened((*reinterpret_cast< HSignal*(*)>(_a[1]))); break;
        case 2: _t->illuminateSpeicher((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2]))); break;
        case 3: _t->zugpassiert(); break;
        case 4: _t->listenToFS(); break;
        case 5: _t->recieveSpeicher((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< HSignal* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HSignal::*)(std::string , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HSignal::refreshStellwerkstechnik)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HSignal::*)(HSignal * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HSignal::listened)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HSignal::*)(bool , std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HSignal::illuminateSpeicher)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HSignal::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_HSignal.data,
    qt_meta_data_HSignal,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HSignal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HSignal::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HSignal.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HSignal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void HSignal::refreshStellwerkstechnik(std::string _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HSignal::listened(HSignal * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HSignal::illuminateSpeicher(bool _t1, std::string _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
