/****************************************************************************
** Meta object code from reading C++ file 'segment.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../segment.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'segment.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Segment_t {
    QByteArrayData data[6];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Segment_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Segment_t qt_meta_stringdata_Segment = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Segment"
QT_MOC_LITERAL(1, 8, 13), // "cycleComplete"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 19), // "parametersEstimated"
QT_MOC_LITERAL(4, 43, 20), // "segmentationComplete"
QT_MOC_LITERAL(5, 64, 19) // "classificationValue"

    },
    "Segment\0cycleComplete\0\0parametersEstimated\0"
    "segmentationComplete\0classificationValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Segment[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    6,   37,    2, 0x06 /* Public */,
       4,    0,   50,    2, 0x06 /* Public */,
       5,    1,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    2,    2,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void Segment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Segment *_t = static_cast<Segment *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->cycleComplete((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->parametersEstimated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        case 2: _t->segmentationComplete(); break;
        case 3: _t->classificationValue((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Segment::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Segment::cycleComplete)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Segment::*_t)(double , double , double , double , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Segment::parametersEstimated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Segment::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Segment::segmentationComplete)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Segment::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Segment::classificationValue)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject Segment::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Segment.data,
      qt_meta_data_Segment,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Segment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Segment::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Segment.stringdata0))
        return static_cast<void*>(const_cast< Segment*>(this));
    return QThread::qt_metacast(_clname);
}

int Segment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Segment::cycleComplete(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Segment::parametersEstimated(double _t1, double _t2, double _t3, double _t4, double _t5, double _t6)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Segment::segmentationComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Segment::classificationValue(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
