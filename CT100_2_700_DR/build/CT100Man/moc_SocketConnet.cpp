/****************************************************************************
** Meta object code from reading C++ file 'SocketConnet.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CT100Man/qtSocketServer/SocketConnet.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SocketConnet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SocketConnet[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   14,   13,   13, 0x05,
      47,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      71,   13,   13,   13, 0x09,
      92,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_SocketConnet[] = {
    "SocketConnet\0\0,\0GetDataFromClient(QString,int)\0"
    "ClientDisConnected(int)\0ReceivedClientData()\0"
    "slotDisconnected()\0"
};

void SocketConnet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SocketConnet *_t = static_cast<SocketConnet *>(_o);
        switch (_id) {
        case 0: _t->GetDataFromClient((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->ClientDisConnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ReceivedClientData(); break;
        case 3: _t->slotDisconnected(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SocketConnet::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SocketConnet::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_SocketConnet,
      qt_meta_data_SocketConnet, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SocketConnet::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SocketConnet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SocketConnet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SocketConnet))
        return static_cast<void*>(const_cast< SocketConnet*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int SocketConnet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SocketConnet::GetDataFromClient(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SocketConnet::ClientDisConnected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
