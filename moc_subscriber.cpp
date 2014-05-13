/****************************************************************************
** Meta object code from reading C++ file 'subscriber.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "subscriber.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'subscriber.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Subscriber_t {
    QByteArrayData data[7];
    char stringdata[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Subscriber_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Subscriber_t qt_meta_stringdata_Subscriber = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 12),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 9),
QT_MOC_LITERAL(4, 35, 12),
QT_MOC_LITERAL(5, 48, 7),
QT_MOC_LITERAL(6, 56, 14)
    },
    "Subscriber\0messageReady\0\0connected\0"
    "disconnected\0errored\0startListening\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Subscriber[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06,
       3,    0,   42,    2, 0x06,
       4,    0,   43,    2, 0x06,
       5,    1,   44,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       6,    0,   47,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Subscriber::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Subscriber *_t = static_cast<Subscriber *>(_o);
        switch (_id) {
        case 0: _t->messageReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnected(); break;
        case 3: _t->errored((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->startListening(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Subscriber::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Subscriber::messageReady)) {
                *result = 0;
            }
        }
        {
            typedef void (Subscriber::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Subscriber::connected)) {
                *result = 1;
            }
        }
        {
            typedef void (Subscriber::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Subscriber::disconnected)) {
                *result = 2;
            }
        }
        {
            typedef void (Subscriber::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Subscriber::errored)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject Subscriber::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Subscriber.data,
      qt_meta_data_Subscriber,  qt_static_metacall, 0, 0}
};


const QMetaObject *Subscriber::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Subscriber::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Subscriber.stringdata))
        return static_cast<void*>(const_cast< Subscriber*>(this));
    return QObject::qt_metacast(_clname);
}

int Subscriber::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Subscriber::messageReady(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Subscriber::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Subscriber::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Subscriber::errored(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
